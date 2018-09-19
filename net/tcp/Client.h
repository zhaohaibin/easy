#ifndef _EASY_NET_TCP_CLIENT_H
#define _EASY_NET_TCP_CLIENT_H

#include "Connection.h"

namespace easy {
	namespace net {
		namespace tcp {
			class Client;
			typedef std::shared_ptr<Client> ClientSharedPtr;

			class Client : public std::enable_shared_from_this<Client>
			{
			public:
				Client(asio::io_service& io_service);

				void AsyncConnect(const std::string& ip, unsigned int port);

			private:
				void ConnectedHandler(boost::system::error_code ec, ClientSharedPtr spClient);
				void WriteHandler(const unsigned char*, size_t length, boost::system::error_code ec, ClientSharedPtr spClient);
				void ReadHandler(const unsigned char*, size_t length, boost::system::error_code ec, ClientSharedPtr spClient);
			private:
				ConnectionSharedPtr m_spConnection;
			};
		}
	}
}
#endif //_EASY_NET_TCP_CLIENT_H
