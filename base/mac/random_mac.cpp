#include "random_mac.h"
#include <ctime>
#include <windows.h>
#include "boost\detail\winapi\timers.hpp"

namespace base
{
	namespace mac
	{
		char random_mac::m_hex_char[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };

		char random_mac::hex_random()
		{
			LARGE_INTEGER seed;
			QueryPerformanceFrequency(&seed);
			QueryPerformanceCounter(&seed);
			srand(seed.QuadPart);
			int n = rand() % 16;
			return m_hex_char[n];
		}

		std::string random_mac::generate()
		{
			string mac_addr;
			string split = ":";
			for (int i = 0; i < 6; ++i)
			{
				char c1 = hex_random();
				char c2 = hex_random();
				mac_addr += c1;
				mac_addr += c2;
				if (i != 5)
					mac_addr += split;
			}
			return mac_addr;
		}
	}
}