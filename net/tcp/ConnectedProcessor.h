#ifndef _EASY_NET_TCP_CONNECTED_PROCESSOR_H
#define _EASY_NET_TCP_CONNECTED_PROCESSOR_H

#include <boost/system/error_code.hpp>

namespace easy {
	namespace net {
		namespace tcp {

			class ConnectedProcessor
			{
			public:
				virtual void ConnectedHandler(boost::system::error_code ec) = 0;
			};
		}
	}
}
#endif //_EASY_NET_TCP_CONNECTED_PROCESSOR_H
