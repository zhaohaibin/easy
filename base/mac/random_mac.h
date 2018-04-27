#ifndef _RANDOM_MAC_H
#define _RANDOM_MAC_H
#include <string>
using namespace std;

//�������mac��ַ

namespace base
{
	namespace mac
	{
		class random_mac
		{
		public:
			static string generate();
		private:
			//�������һ��16����������ֵ��Χ[0-f]
			static char hex_random();
		private:
			static char m_hex_char[16];
		};
	}
}

#endif //_RANDOM_MAC_H
