#ifndef _TASK_H
#define _TASK_H

#include <memory>

namespace base
{
	namespace thread
	{
		class task : public std::enable_shared_from_this<task>
		{
		public:
			task();
			virtual ~task();
		public:
			void run();

		protected:
			virtual void task_start();
			virtual void taks_end();
			virtual void handler() = 0;
		};

		typedef  std::shared_ptr<task> task_ptr;
	}
}
#endif //_TASK_H
