#ifndef _EASY_NET_TCP_SESSION_H
#define _EASY_NET_TCP_SESSION_H

#include <memory>
#include <deque>
#include <tuple>
#include <boost/asio.hpp>
#include <boost/thread/mutex.hpp>
#include "TransportDataProcessor.h"
#include "Connection.h"

using namespace boost;

namespace easy {
	namespace net {
		namespace tcp {
#define DEFAULT_READ_BUFFER_SIZE 32768  //32KB
			class Session;
			typedef std::shared_ptr<Session> SessionShardPtr;

			class Session
				: public std::enable_shared_from_this<Session>
			{
			public:
				Session(asio::ip::tcp::socket socket);
				~Session();
			public:
				void SetTransportDataProcessor(TransportDataProcessorSharedPtr spTransportDataProcessor);
				void AsyncRead();
				void AsyncWrite(const unsigned char* data, size_t length);

			private:
				void DoAsyncWrite(const unsigned char* data, size_t length, SessionShardPtr spSession);

			private:
				void ReadHandler(const unsigned char* data, std::size_t length, boost::system::error_code ec, SessionShardPtr spSession);
				void WriteHandler(const unsigned char* data, std::size_t length, boost::system::error_code ec, SessionShardPtr spSession);
			private:
				std::deque<std::tuple<const unsigned char*, size_t>> m_writeQueue;
				TransportDataProcessorSharedPtr m_spTransportDataProcessor;
				ConnectionSharedPtr m_spConnection;
				bool m_writeInProcess;
			};

			
		}
	}
}
#endif _EASY_NET_TCP_SESSION_H
