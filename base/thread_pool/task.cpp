#include "task.h"

namespace base
{
	namespace thread
	{

		task::task()
		{
		}

		task::~task()
		{
		}

		void task::run()
		{
			task_start();
			handler();
			taks_end();
		}

		void task::task_start()
		{

		}

		void task::taks_end()
		{

		}
	}
}


