#include "thread_pool.h"
#include <boost/bind.hpp>
namespace base
{
	namespace thread
	{

		thread_pool::thread_pool(int thread_count)
			: m_thread_count(thread_count)
		{
		}

		thread_pool::~thread_pool()
		{

		}

		void thread_pool::start()
		{
			for (int i = 0; i < m_thread_count; ++i)
			{
				m_thread_group.create_thread(boost::bind(&thread_pool::work_thread, this));
			}
		}

		void thread_pool::stop()
		{
			m_stop = true;
			m_thread_group.join_all();
		}

		void thread_pool::post_task(task_ptr sp_task)
		{
			m_taks_queue.push_taks(sp_task);
		}

		void thread_pool::work_thread()
		{
			while (m_stop == false)
			{
				task_ptr sp_task = m_taks_queue.pop();
				if (sp_task)
				{
					sp_task->run();
				}
				else
				{
					Sleep(200);
				}
			}
		}
	}
}