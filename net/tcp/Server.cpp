#include "Server.h"
#include "Session.h"

namespace easy {
	namespace net {
		namespace tcp {

			Server::Server(
				asio::io_service& io_service, 
				const asio::ip::tcp::endpoint& endpoint,
				AcceptProcessorSharedPtr spAcceptProcessor)
				: m_acceptor(io_service, endpoint)
				, m_socket(io_service)
				, m_spAcceptProcessor(spAcceptProcessor)
			{

			}

			void Server::Start()
			{
				DoAccept();
			}

			void Server::DoAccept()
			{
				m_acceptor.async_accept(m_socket,
					[this](boost::system::error_code ec) {
					if (!ec)
					{
						SessionShardPtr sp_session = std::make_shared<Session>(std::move(m_socket));
						m_spAcceptProcessor->AcceptHandler(sp_session);
					}
					DoAccept();
				});
			}

		}
	}
}