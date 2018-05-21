#include "task_queue.h"

namespace base
{
	namespace thread
	{

		task_queue::task_queue()
		{
			InitializeCriticalSection(&m_cs);
		}

		task_queue::~task_queue()
		{
			DeleteCriticalSection(&m_cs);
		}

		void task_queue::push_taks(task_ptr sp_task)
		{
			EnterCriticalSection(&m_cs);
			m_task_queue.push(sp_task);
			LeaveCriticalSection(&m_cs);
		}

		base::thread::task_ptr task_queue::pop()
		{
			task_ptr sp_task;
			EnterCriticalSection(&m_cs);
			if (m_task_queue.size() > 0)
			{
				sp_task = m_task_queue.front();
				m_task_queue.pop();
			}
			LeaveCriticalSection(&m_cs);
			return sp_task;
		}
	}
}


