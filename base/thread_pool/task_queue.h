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
		//task_queue �̰߳�ȫ
		class task_queue
		{
		public:
			task_queue();
			~task_queue();

		public:
			//��������ӵ�����β��
			void push_taks(task_ptr sp_task);

			//�Ӷ��е�ͷ������һ������
			// �������Ϊ�գ�������Ϊnullptr
			task_ptr pop();
		private:
			queue<task_ptr> m_task_queue;
			CRITICAL_SECTION m_cs;
		};
	}
}
#endif //_TASK_QUEUE_H
