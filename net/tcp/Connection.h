#ifndef _EASY_NET_TCP_CONNECTION_H
#define _EASY_NET_TCP_CONNECTION_H

#include "WriteBuffer.h"
#include <boost/asio.hpp>
#include <boost/system/error_code.hpp>
#include <functional>
using namespace boost;

#define READ_BUFFER_SIZE 32768

namespace easy {
	namespace net {
		namespace tcp {
			class Connection;
			typedef std::shared_ptr<Connection> ConnectionSharedPtr;

			class Connection : public std::enable_shared_from_this<Connection>
			{
			public:

				typedef std::function<void( const unsigned char*, size_t length, boost::system::error_code ec)> WriteHandlerType;
				typedef std::function<void( const unsigned char*, size_t length, boost::system::error_code ec)> ReadHandlerType;
				typedef std::function<void( boost::system::error_code ec)> ConnectedHandlerType;
				typedef std::function<void()> TaskType;

				Connection(asio::ip::tcp::socket socket);
				Connection(asio::io_service& io_service);
				~Connection();

				void AsyncConnect(const std::string& ip, unsigned int port, ConnectedHandlerType connectedHandler);
				void AsyncWrite(const unsigned char* data, size_t length, WriteHandlerType writeHandler);
				void AsyncRead(ReadHandlerType readHandler);
				void PostTask(TaskType taskType);
			private:
				void DoWrite(WriteHandlerType writeHandler);

				void WriteDone(boost::system::error_code ec, std::size_t length, WriteHandlerType writeHandler, ConnectionSharedPtr spConnection);
				void ReadDone(boost::system::error_code ec, std::size_t length, ReadHandlerType readHandler, ConnectionSharedPtr spConnection);
				void ConnectDone(boost::system::error_code ec, ConnectedHandlerType connectedHandler, ConnectionSharedPtr spConnection);
			
			private:
				asio::ip::tcp::socket m_socket;
				WriteBuffer m_wBuffer;
				unsigned char m_readBuffer[READ_BUFFER_SIZE];

				WriteHandlerType m_writeHandler;
				ReadHandlerType m_readHandler;
				ConnectedHandlerType m_connectedHandler;

				std::shared_ptr<asio::ip::tcp::resolver> m_spResolver;
			};
		}
	}
}
#endif //_EASY_NET_TCP_CONNECTION_H
