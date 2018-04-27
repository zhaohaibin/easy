#include "crash.h"
#include <Windows.h>
#include <DbgHelp.h>
#include <boost/filesystem.hpp>
#include <boost/lexical_cast.hpp>
#include "../uuid/random_uuid.h"

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
	case ModuleCallback:{
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

void base::crash::setup()
{
	SetUnhandledExceptionFilter(un_handled_exception_filter);
}

