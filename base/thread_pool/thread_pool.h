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
			//�����̳߳�
			void start();

			//ֹͣ�̳߳�
			void stop();

			//���̳߳���Ͷ������
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
