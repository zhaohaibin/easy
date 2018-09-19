#include "Connection.h"

namespace easy {
	namespace net {
		namespace tcp {

			Connection::Connection(asio::ip::tcp::socket socket)
				: m_socket(std::move(socket))
			{
			}

			Connection::Connection(
				asio::io_service& io_service)
				: m_socket(io_service)
			{

			}

			Connection::~Connection()
			{

			}

			void Connection::AsyncConnect(const std::string& ip, unsigned int port, ConnectedHandlerType connectedHandler)
			{
				m_spResolver = std::make_shared<asio::ip::tcp::resolver>(m_socket.get_io_service());

				boost::asio::ip::address add;
				add.from_string(ip);
				asio::ip::tcp::endpoint endpoint(add, port);
				boost::asio::async_connect(
					m_socket, m_spResolver->resolve(endpoint),
					std::bind(&Connection::ConnectDone, this, std::placeholders::_1, connectedHandler, shared_from_this()));
			}

			void Connection::AsyncWrite(const unsigned char* data, size_t length, WriteHandlerType writeHandler)
			{
				m_wBuffer.Reset(data, length);
				DoWrite(writeHandler);
			}

			void Connection::PostTask(TaskType taskType)
			{
				m_socket.get_io_service().post(taskType);
			}

			void Connection::DoWrite(WriteHandlerType writeHandler)
			{
				if (m_wBuffer.Length() > 0)
				{
 					boost::asio::async_write(m_socket, boost::asio::buffer(m_wBuffer.Data(),
 						m_wBuffer.Length()), std::bind(&Connection::WriteDone, this, std::placeholders::_1, std::placeholders::_2, writeHandler, shared_from_this()));
				}
			}

			void Connection::WriteDone(boost::system::error_code ec, std::size_t length, WriteHandlerType writeHandler, ConnectionSharedPtr spConnection)
			{
				if (length != m_wBuffer.SrcLength())
				{

				}
				else
				{
					if (writeHandler)
					{
						writeHandler(m_wBuffer.SrcData(), m_wBuffer.SrcLength(), ec);
					}
				}
			}

			void Connection::ReadDone(boost::system::error_code ec, std::size_t length, ReadHandlerType readHandler, ConnectionSharedPtr spConnection)
			{
				if (readHandler)
				{
					readHandler(m_readBuffer, length, ec);
				}
			}

			void Connection::ConnectDone(boost::system::error_code ec, ConnectedHandlerType connectedHandler, ConnectionSharedPtr spConnection)
			{
				if (connectedHandler)
				{
					connectedHandler(ec);
				}
			}

			void Connection::AsyncRead(ReadHandlerType readHandler)
			{
				m_socket.async_read_some(boost::asio::buffer(m_readBuffer, READ_BUFFER_SIZE),
					std::bind(&Connection::ReadDone, this, std::placeholders::_1, std::placeholders::_2, readHandler, shared_from_this()));
			}

		}
	}
}