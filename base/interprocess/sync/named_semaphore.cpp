#include "named_semaphore.h"
#include "boost/date_time/posix_time/posix_time_types.hpp"
namespace easy
{

	named_semaphore::named_semaphore(create_only mode, const string& name, unsigned int initial_count)
		: m_semaphore(boost::interprocess::create_only_t(), name.c_str(), initial_count)
		, m_name(name)
	{
	}

	named_semaphore::named_semaphore(open_only mode, const string& name)
		: m_semaphore(boost::interprocess::open_only_t(), name.c_str())
		, m_name(name)
	{
	}

	named_semaphore::named_semaphore(
		open_or_create mode, const string& name, unsigned int initial_count)
		: m_semaphore(boost::interprocess::open_or_create_t(), name.c_str(), initial_count)
		, m_name(name)
	{
	}

	named_semaphore::~named_semaphore()
	{
	}

	void named_semaphore::post()
	{
		m_semaphore.post();
	}

	void named_semaphore::wait()
	{
		m_semaphore.wait();
	}

	bool named_semaphore::try_wait()
	{
		return m_semaphore.try_wait();
	}

	bool named_semaphore::timed_wait(const boost::posix_time::ptime& time)
	{
		return m_semaphore.timed_wait(time);
	}

	bool named_semaphore::timed_wait(unsigned int second)
	{
		boost::posix_time::ptime pt(
			boost::posix_time::second_clock::universal_time()+ boost::posix_time::seconds(second));
		return timed_wait(pt);
	}

	void named_semaphore::remove(named_semaphore& semaphore)
	{
		boost::interprocess::named_semaphore::remove(semaphore.m_name.c_str());
	}
}