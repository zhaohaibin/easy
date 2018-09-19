#ifndef _EASY_NET_TCP_SERVER_H
#define _EASY_NET_TCP_SERVER_H
#include <boost/asio.hpp>
#include "AcceptProcessor.h"
using namespace boost;
namespace easy {
	namespace net {
		namespace tcp {
			
			class Server
			{
			public:
				Server(asio::io_service& io_service, const asio::ip::tcp::endpoint& endpoint, AcceptProcessorSharedPtr spAcceptProcessor);

			public:
				void Start();

			private:
				void DoAccept();

			private:
				asio::ip::tcp::acceptor m_acceptor;
				asio::ip::tcp::socket m_socket;
				AcceptProcessorSharedPtr m_spAcceptProcessor;
			};

			typedef std::shared_ptr<Server> ServerSharedPtr;
		}
	}
}
#endif _EASY_NET_TCP_SERVER_H
