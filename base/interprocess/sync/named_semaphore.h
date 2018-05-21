#ifndef _NAMED_SEMAPHORE_H
#define _NAMED_SEMAPHORE_H
#include <string>
using namespace std;
#include "boost/interprocess/sync/named_semaphore.hpp"
#include "creation_tags.h"

namespace easy
{
	class named_semaphore
	{
	public:
		named_semaphore(create_only mode, const string& name, unsigned int initial_count);
		named_semaphore(open_only mode, const string& name);
		named_semaphore(open_or_create mode, const string& name, unsigned int initial_count);
		~named_semaphore();
	public:
		void post();

		void wait();

		bool try_wait();

		bool timed_wait(const boost::posix_time::ptime& time);

		bool timed_wait(unsigned int second);

		static void remove(named_semaphore& semaphore);
	private:
		boost::interprocess::named_semaphore m_semaphore;
		string m_name;
	};
}
#endif //_NAMED_SEMAPHORE_H