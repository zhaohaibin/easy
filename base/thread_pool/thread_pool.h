#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H
#include "task_queue.h"
#include <boost/thread.hpp>
namespace base
{
	namespace thread
	{
		class thread_pool
		{
		public:
			thread_pool(int thread_count);
			virtual ~thread_pool();

		public:
			//启动线程池
			void start();

			//停止线程池
			void stop();

			//向线程池中投递任务
			void post_task(task_ptr sp_task);
		private:
			void work_thread();

		private:
			task_queue m_taks_queue;
			int m_thread_count;
			bool m_stop;
			boost::thread_group m_thread_group;
		};
	}
}
#endif //_THREAD_POOL_H
