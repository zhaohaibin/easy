#include "crash.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "easy/base/misc/uuid/random_uuid.h"
#include <new.h>
#include <signal.h>

namespace base
{
	namespace crash
	{
		//format excepton info
		wstring format_exception_info(EXCEPTION_RECORD* pExceptionRecord)
		{
			// ModuleName 属性。
			WCHAR szModuleName[MAX_PATH] = { 0 };
			::GetModuleFileNameW(NULL, szModuleName, MAX_PATH);
			// ExceptionDescription 属性
			LPCWSTR szExceptionDescription;
			switch (pExceptionRecord->ExceptionCode)
			{
			case EXCEPTION_ACCESS_VIOLATION:
				szExceptionDescription = L"EXCEPTION_ACCESS_VIOLATION";
				break;
			case EXCEPTION_DATATYPE_MISALIGNMENT:
				szExceptionDescription = L"EXCEPTION_DATATYPE_MISALIGNMENT";
				break;
			case EXCEPTION_BREAKPOINT:
				szExceptionDescription = L"EXCEPTION_BREAKPOINT";
				break;
			case EXCEPTION_SINGLE_STEP:
				szExceptionDescription = L"EXCEPTION_SINGLE_STEP";
				break;
			case EXCEPTION_ARRAY_BOUNDS_EXCEEDED:
				szExceptionDescription = L"EXCEPTION_ARRAY_BOUNDS_EXCEEDED";
				break;
			case EXCEPTION_FLT_DENORMAL_OPERAND:
				szExceptionDescription = L"EXCEPTION_FLT_DENORMAL_OPERAND";
				break;
			case EXCEPTION_FLT_DIVIDE_BY_ZERO:
				szExceptionDescription = L"EXCEPTION_FLT_DIVIDE_BY_ZERO";
				break;
			case EXCEPTION_FLT_INEXACT_RESULT:
				szExceptionDescription = L"EXCEPTION_FLT_INEXACT_RESULT";
				break;
			case EXCEPTION_FLT_INVALID_OPERATION:
				szExceptionDescription = L"EXCEPTION_FLT_INVALID_OPERATION";
				break;
			case EXCEPTION_FLT_OVERFLOW:
				szExceptionDescription = L"EXCEPTION_FLT_OVERFLOW ";
				break;
			case EXCEPTION_FLT_STACK_CHECK:
				szExceptionDescription = L"EXCEPTION_FLT_STACK_CHECK";
				break;
			case EXCEPTION_FLT_UNDERFLOW:
				szExceptionDescription = L"EXCEPTION_FLT_UNDERFLOW";
				break;
			case EXCEPTION_INT_DIVIDE_BY_ZERO:
				szExceptionDescription = L"EXCEPTION_INT_DIVIDE_BY_ZERO";
				break;
			case EXCEPTION_INT_OVERFLOW:
				szExceptionDescription = L"EXCEPTION_INT_OVERFLOW";
				break;
			case EXCEPTION_PRIV_INSTRUCTION:
				szExceptionDescription = L"EXCEPTION_PRIV_INSTRUCTION";
				break;
			case EXCEPTION_IN_PAGE_ERROR:
				szExceptionDescription = L"EXCEPTION_IN_PAGE_ERROR";
				break;
			case EXCEPTION_ILLEGAL_INSTRUCTION:
				szExceptionDescription = L"EXCEPTION_ILLEGAL_INSTRUCTION";
				break;
			case EXCEPTION_NONCONTINUABLE_EXCEPTION:
				szExceptionDescription = L"EXCEPTION_NONCONTINUABLE_EXCEPTION";
				break;
			case EXCEPTION_STACK_OVERFLOW:
				szExceptionDescription = L"EXCEPTION_STACK_OVERFLOW";
				break;
			case EXCEPTION_INVALID_DISPOSITION:
				szExceptionDescription = L"EXCEPTION_INVALID_DISPOSITION";
				break;
			case EXCEPTION_GUARD_PAGE:
				szExceptionDescription = L"EXCEPTION_GUARD_PAGE";
				break;
			case EXCEPTION_INVALID_HANDLE:
				szExceptionDescription = L"EXCEPTION_INVALID_HANDLE";
				break;
			default:
				szExceptionDescription = L"EXCEPTION_UNKNOWN";   // 未知的异常。
				break;
			}

			wchar_t buffer[2048];
			swprintf(buffer,
				L"Module=%s\nCode=%x\nDescription=%s\nAdddress=%x",
				szModuleName,
				pExceptionRecord->ExceptionCode,
				szExceptionDescription,
				pExceptionRecord->ExceptionAddress);
			return buffer;
		}

		// The following code gets exception pointers using a workaround found in CRT code.
		void get_exception_pointers(DWORD dwExceptionCode,
			EXCEPTION_POINTERS** ppExceptionPointers)
		{
			// The following code was taken from VC++ 8.0 CRT (invarg.c: line 104)
			EXCEPTION_RECORD ExceptionRecord;
			CONTEXT ContextRecord;
			memset(&ContextRecord, 0, sizeof(CONTEXT));

#ifdef _X86_

			__asm {
				mov dword ptr[ContextRecord.Eax], eax
				mov dword ptr[ContextRecord.Ecx], ecx
				mov dword ptr[ContextRecord.Edx], edx
				mov dword ptr[ContextRecord.Ebx], ebx
				mov dword ptr[ContextRecord.Esi], esi
				mov dword ptr[ContextRecord.Edi], edi
				mov word ptr[ContextRecord.SegSs], ss
				mov word ptr[ContextRecord.SegCs], cs
				mov word ptr[ContextRecord.SegDs], ds
				mov word ptr[ContextRecord.SegEs], es
				mov word ptr[ContextRecord.SegFs], fs
				mov word ptr[ContextRecord.SegGs], gs
				pushfd
				pop[ContextRecord.EFlags]
			}

			ContextRecord.ContextFlags = CONTEXT_CONTROL;
#pragma warning(push)
#pragma warning(disable:4311)
			ContextRecord.Eip = (ULONG)_ReturnAddress();
			ContextRecord.Esp = (ULONG)_AddressOfReturnAddress();
#pragma warning(pop)
			ContextRecord.Ebp = *((ULONG *)_AddressOfReturnAddress() - 1);


#elif defined (_IA64_) || defined (_AMD64_)

			/* Need to fill up the Context in IA64 and AMD64. */
			RtlCaptureContext(&ContextRecord);

#else  /* defined (_IA64_) || defined (_AMD64_) */

			ZeroMemory(&ContextRecord, sizeof(ContextRecord));

#endif  /* defined (_IA64_) || defined (_AMD64_) */

			ZeroMemory(&ExceptionRecord, sizeof(EXCEPTION_RECORD));

			ExceptionRecord.ExceptionCode = dwExceptionCode;
			ExceptionRecord.ExceptionAddress = _ReturnAddress();

			///

			EXCEPTION_RECORD* pExceptionRecord = new EXCEPTION_RECORD;
			memcpy(pExceptionRecord, &ExceptionRecord, sizeof(EXCEPTION_RECORD));
			CONTEXT* pContextRecord = new CONTEXT;
			memcpy(pContextRecord, &ContextRecord, sizeof(CONTEXT));

			*ppExceptionPointers = new EXCEPTION_POINTERS;
			(*ppExceptionPointers)->ExceptionRecord = pExceptionRecord;
			(*ppExceptionPointers)->ContextRecord = pContextRecord;
		}

		BOOL is_data_section_needed(const PWCHAR p_module_name)
		{
			if (p_module_name == NULL)
				return FALSE;
			wchar_t file_name[_MAX_FNAME] = L"";
			_wsplitpath(p_module_name, NULL, NULL, file_name, NULL);
			if (_wcsicmp(file_name, L"ntdll") == 0)
				return TRUE;
			return FALSE;
		}

		BOOL WINAPI mini_dump_call_back(PVOID pParam,
			const PMINIDUMP_CALLBACK_INPUT pInput,
			PMINIDUMP_CALLBACK_OUTPUT pOutput)
		{
			if (pInput == 0 || pOutput == 0)
				return FALSE;
			switch (pInput->CallbackType)
			{
			case ModuleCallback: {
				if (pOutput->ModuleWriteFlags & ModuleWriteDataSeg)
					if (!is_data_section_needed(pInput->Module.FullPath))
						pOutput->ModuleWriteFlags &= (~ModuleWriteDataSeg);
			}break;
			case IncludeModuleCallback:
			case IncludeThreadCallback:
			case ThreadCallback:
			case ThreadExCallback:
				return TRUE;

			default:break;

			}
			return FALSE;
		}

		void create_mini_dump(struct _EXCEPTION_POINTERS *pExceptionPointers, const wstring& file_name)
		{
			HANDLE hFile = CreateFileW(file_name.c_str(),
				GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if ((hFile != NULL) && (hFile != INVALID_HANDLE_VALUE))
			{
				MINIDUMP_EXCEPTION_INFORMATION mdei;
				mdei.ThreadId = GetCurrentThreadId();
				mdei.ExceptionPointers = pExceptionPointers;
				mdei.ClientPointers = TRUE;

				MINIDUMP_CALLBACK_INFORMATION mci;
				mci.CallbackRoutine = (MINIDUMP_CALLBACK_ROUTINE)mini_dump_call_back;
				mci.CallbackParam = 0;

				MINIDUMP_TYPE mdt = (MINIDUMP_TYPE)(MiniDumpNormal
					| MiniDumpWithHandleData
					| MiniDumpWithUnloadedModules
					| MiniDumpWithIndirectlyReferencedMemory
					| MiniDumpScanMemory
					| MiniDumpWithProcessThreadData
					| MiniDumpWithThreadInfo);
				MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, mdt, &mdei, NULL, NULL);
				CloseHandle(hFile);
			}
		}

		wstring generate_dump_file_name()
		{
			SYSTEMTIME local_time;
			GetLocalTime(&local_time);
			wstring name = boost::lexical_cast<wstring>(local_time.wYear);
			name.append(L"-");
			name.append(boost::lexical_cast<wstring>(local_time.wMonth));
			name.append(L"-");
			name.append(boost::lexical_cast<wstring>(local_time.wDay));
			name.append(L" ");
			name.append(boost::lexical_cast<wstring>(local_time.wHour));
			name.append(L"-");
			name.append(boost::lexical_cast<wstring>(local_time.wMinute));
			name.append(L"-");
			name.append(boost::lexical_cast<wstring>(local_time.wSecond));
			name.append(L"_");
			name.append(random_uuid::wgenerate_t());
			name.append(L".dmp");
			return name;
		}

		LONG WINAPI un_handled_exception_filter(struct _EXCEPTION_POINTERS *pExceptionPointers)
		{
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPointers, parent_path.generic_wstring());
			return 0;
		}

		//CRT Pure virtual method call handler
		void __cdecl pure_virtual_exception_handler()
		{
			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}

		// CRT invalid parameter handler
		int __cdecl new_exception_handler(size_t)
		{
			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);

			// Unreacheable code
			return 0;
		}

		// CRT invalid parameter handler
		void __cdecl invalid_parameter_handler(
			const wchar_t* expression,
			const wchar_t* function,
			const wchar_t* file,
			unsigned int line,
			uintptr_t pReserved)
		{
			// Invalid parameter exception

			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}


		// CRT SIGABRT signal handler
		void sigabrt_handler(int)
		{
			// Caught SIGABRT C++ signal

			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}

		// CRT sigint signal handler
		void sigint_handler(int)
		{
			// Interruption (SIGINT)

			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}

		// CRT SIGTERM signal handler
		void sigterm_handler(int)
		{
			// Termination request (SIGTERM)

			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());

			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}


		void set_process_exception_handler()
		{
			// Install top-level SEH handler
			SetUnhandledExceptionFilter(un_handled_exception_filter);

			// Catch pure virtual function calls.
			// Because there is one _purecall_handler for the whole process, 
			// calling this function immediately impacts all threads. The last 
			// caller on any thread sets the handler. 
			// http://msdn.microsoft.com/en-us/library/t296ys27.aspx
			_set_purecall_handler(pure_virtual_exception_handler);

			// Catch new operator memory allocation exceptions
			_set_new_handler(new_exception_handler);

			// Catch invalid parameter exceptions.
			_set_invalid_parameter_handler(invalid_parameter_handler);

			/************************************************************************/
			// Set up C++ signal handlers

			_set_abort_behavior(_WRITE_ABORT_MSG | _CALL_REPORTFAULT, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
			_set_abort_behavior(0, _WRITE_ABORT_MSG);

			// Catch an abnormal program termination
			signal(SIGABRT, sigabrt_handler);

			// Catch illegal instruction handler
			signal(SIGINT, sigint_handler);

			// Catch a termination request
			signal(SIGTERM, sigterm_handler);
		}


		// CRT terminate() call handler
		void __cdecl terminate_handler()
		{
			// Abnormal program termination (terminate() function was called)

			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}

		// CRT unexpected() call handler
		void __cdecl unexpected_handler()
		{
			// Unexpected error (unexpected() function was called)

			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}

		// CRT SIGFPE signal handler
		void sigfpe_handler(int /*code*/, int subcode)
		{
			// Floating point exception (SIGFPE)

			EXCEPTION_POINTERS* pExceptionPtrs = (PEXCEPTION_POINTERS)_pxcptinfoptrs;

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);

		}

		// CRT sigill signal handler
		void sigill_handler(int)
		{
			// Illegal instruction (SIGILL)

			// Retrieve exception information
			EXCEPTION_POINTERS* pExceptionPtrs = NULL;
			get_exception_pointers(0, &pExceptionPtrs);

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);

		}

		// CRT SIGSEGV signal handler
		void sigsegv_handler(int)
		{
			// Invalid storage access (SIGSEGV)

			PEXCEPTION_POINTERS pExceptionPtrs = (PEXCEPTION_POINTERS)_pxcptinfoptrs;

			// Write minidump file
			wchar_t file_name[MAX_PATH];
			::GetModuleFileNameW(NULL, file_name, MAX_PATH);
			boost::filesystem::path file_path(file_name);
			boost::filesystem::path parent_path = file_path.parent_path();
			wstring dump_name = generate_dump_file_name();
			parent_path.append(dump_name);
			create_mini_dump(pExceptionPtrs, parent_path.generic_wstring());
			// Terminate process
			TerminateProcess(GetCurrentProcess(), 1);
		}

		void set_thread_exception_handler()
		{
			// Catch terminate() calls. 
			// In a multithreaded environment, terminate functions are maintained 
			// separately for each thread. Each new thread needs to install its own 
			// terminate function. Thus, each thread is in charge of its own termination handling.
			// http://msdn.microsoft.com/en-us/library/t6fk7h29.aspx
			set_terminate(terminate_handler);

			// Catch unexpected() calls.
			// In a multithreaded environment, unexpected functions are maintained 
			// separately for each thread. Each new thread needs to install its own 
			// unexpected function. Thus, each thread is in charge of its own unexpected handling.
			// http://msdn.microsoft.com/en-us/library/h46t5b69.aspx  
			set_unexpected(unexpected_handler);

			// Catch a floating point error
			typedef void(*sigh)(int);
			signal(SIGFPE, (sigh)sigfpe_handler);

			// Catch an illegal instruction
			signal(SIGILL, sigill_handler);

			// Catch illegal storage access errors
			signal(SIGSEGV, sigsegv_handler);
		}
	}
}


void base::crash::setup()
{
	set_process_exception_handler();
	set_thread_exception_handler();
}