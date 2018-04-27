#ifndef _RANDOM_MAC_H
#define _RANDOM_MAC_H
#include <string>
using namespace std;

//随机生成mac地址

namespace base
{
	namespace mac
	{
		class random_mac
		{
		public:
			static string generate();
		private:
			//随机生成一个16进制数，数值范围[0-f]
			static char hex_random();
		private:
			static char m_hex_char[16];
		};
	}
}

#endif //_RANDOM_MAC_H
