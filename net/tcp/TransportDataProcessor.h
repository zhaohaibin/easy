#ifndef _EASY_NET_TCP_TRANSPORT_DATA_PROCESSOR_H
#define _EASY_NET_TCP_TRANSPORT_DATA_PROCESSOR_H

#include <string>
#include <memory>
#include <boost/system/error_code.hpp>

namespace easy { 
	namespace net { 
		namespace tcp {

			class Session;
			class TransportDataProcessor
			{
			public:
				TransportDataProcessor();
				virtual ~TransportDataProcessor();

			public:
				virtual void ReadHandler(std::shared_ptr<Session> sp_session, const unsigned char* data, size_t length, boost::system::error_code ec) = 0;

				virtual void WriteHandler(std::shared_ptr<Session> sp_session, const unsigned char* data, size_t length, boost::system::error_code ec) = 0;
			};

			typedef std::shared_ptr<TransportDataProcessor> TransportDataProcessorSharedPtr;
		}
	}
}
#endif // !_EASY_NET_TCP_MESSAGE_HANDLER_H
