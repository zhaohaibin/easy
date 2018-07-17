#ifndef _CRASH_H
#define _CRASH_H
#include <string>
#include <memory>

using namespace std;
namespace base
{
	namespace crash
	{
		class crash_handler
		{
		public:
			virtual void enter_crash() = 0;
			virtual void leave_crash(const string& dump_file) = 0;
		};

		void setup(std::shared_ptr<crash_handler> sp_handler = nullptr);
	}
}
#endif //_CRASH_H
