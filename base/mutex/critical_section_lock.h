#ifndef _CRITICAL_SECTION_LOCK_H
#define _CRITICAL_SECTION_LOCK_H
#include <windows.h>

namespace base
{
	namespace mutex
	{
		class critical_section_lock
		{
		public:
			inline critical_section_lock(CRITICAL_SECTION& cs)
				:m_cs(cs)
			{
				EnterCriticalSection(&m_cs);
			}

			inline ~critical_section_lock()
			{
				LeaveCriticalSection(&m_cs);
			}
		private:
			CRITICAL_SECTION m_cs;
		};
	}
}

#endif //_CRITICAL_SECTION_MUTEX_H
