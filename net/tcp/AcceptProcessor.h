#ifndef _EASY_NET_TCP_ACCEPT_PROCESSOR_H
#define _EASY_NET_TCP_ACCEPT_PROCESSOR_H

#include <memory>
namespace easy {
	namespace net {
		namespace tcp {

			class Session;
			class AcceptProcessor
			{
			public:
				AcceptProcessor();
				virtual ~AcceptProcessor();
				
			public:
				virtual void AcceptHandler(std::shared_ptr<Session> spSession) = 0;
			};

			typedef std::shared_ptr<AcceptProcessor> AcceptProcessorSharedPtr;
		}
	}
}
#endif _EASY_NET_TCP_ACCEPT_PROCESSOR_H
