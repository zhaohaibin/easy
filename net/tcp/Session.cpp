#include "session.h"

namespace easy {
	namespace net {
		namespace tcp {

			Session::Session(asio::ip::tcp::socket socket)
				: m_spConnection(new Connection(std::move(socket)))
				, m_writeInProcess(false)
			{
				
			}

			Session::~Session()
			{

			}

			void Session::SetTransportDataProcessor(TransportDataProcessorSharedPtr spTransportDataProcessor)
			{
				m_spTransportDataProcessor = spTransportDataProcessor;
			}

			void Session::AsyncRead()
			{
				m_spConnection->AsyncRead(
					std::bind(&Session::ReadHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, shared_from_this()));
			}

			void Session::AsyncWrite(const unsigned char* data, size_t length)
			{
				unsigned char* pBuffer = new unsigned char[length];
				memcpy(pBuffer, data, length);
				m_spConnection->PostTask(std::bind(&Session::DoAsyncWrite, this, pBuffer, length, shared_from_this()));
			}

			void Session::DoAsyncWrite(const unsigned char* data, size_t length, SessionShardPtr spSession)
			{
				m_writeQueue.push_back(std::make_tuple(data, length));
				if (m_writeInProcess == false)
				{
					m_writeInProcess = true;
					std::tuple<const unsigned char*, size_t> dataTuple = m_writeQueue.front();
					m_writeQueue.pop_front();
					m_spConnection->AsyncWrite(std::get<0>(dataTuple), std::get<1>(dataTuple),
						std::bind(&Session::WriteHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, shared_from_this()));
				}
			}

			void Session::WriteHandler(const unsigned char* data, std::size_t length, boost::system::error_code ec, SessionShardPtr spSession)
			{
				delete []data;
				if (!ec)
				{
					if (m_writeQueue.size() > 0)
					{
						std::tuple<const unsigned char*, size_t> dataTuple = m_writeQueue.front();
						m_writeQueue.pop_front();
						m_spConnection->AsyncWrite(std::get<0>(dataTuple), std::get<1>(dataTuple),
							std::bind(&Session::WriteHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, shared_from_this()));
					}
					else
					{
						m_writeInProcess = false;
					}
				}
			}

			void Session::ReadHandler(const unsigned char* data, std::size_t length, boost::system::error_code ec, SessionShardPtr spSession)
			{
				if (!ec)
				{
					if (m_spTransportDataProcessor)
					{
						m_spTransportDataProcessor->ReadHandler(shared_from_this(), data, length, ec);
						m_spConnection->AsyncRead(
							std::bind(&Session::ReadHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, shared_from_this()));
					}
				}
			}
		}
	}
}