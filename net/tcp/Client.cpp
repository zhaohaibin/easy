#include "Client.h"

namespace easy {
	namespace net {
		namespace tcp {

			Client::Client(asio::io_service& io_service) : m_spConnection(new Connection(io_service))
			{
			}

			void Client::AsyncConnect(const std::string& ip, unsigned int port)
			{
				m_spConnection->AsyncConnect(ip, port, std::bind(&Client::ConnectedHandler, this, std::placeholders::_1, shared_from_this()));
			}

			void Client::ConnectedHandler(boost::system::error_code ec, ClientSharedPtr spClient)
			{
				if (!ec)
				{
					m_spConnection->AsyncRead(std::bind(&Client::ReadHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, shared_from_this()));
				}
			}

			void Client::WriteHandler(const unsigned char*, size_t length, boost::system::error_code ec, ClientSharedPtr spClient)
			{

			}

			void Client::ReadHandler(const unsigned char*, size_t length, boost::system::error_code ec, ClientSharedPtr spClient)
			{
				if (!ec)
				{
					m_spConnection->AsyncRead(std::bind(&Client::ReadHandler, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3, shared_from_this()));
				}
			}

		}
	}
}