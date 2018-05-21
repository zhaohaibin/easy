#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H
#include <queue>
#include "task.h"
#include<windows.h>
using namespace std;

namespace base
{
	namespace thread
	{
		//task_queue 线程安全
		class task_queue
		{
		public:
			task_queue();
			~task_queue();

		public:
			//将任务添加到队列尾部
			void push_taks(task_ptr sp_task);

			//从队列的头部弹出一个任务
			// 如果队列为空，弹出的为nullptr
			task_ptr pop();
		private:
			queue<task_ptr> m_task_queue;
			CRITICAL_SECTION m_cs;
		};
	}
}
#endif //_TASK_QUEUE_H
