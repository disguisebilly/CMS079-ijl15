#include "stdafx.h"
#include "AutoDump.h"
#include <windows.h>
#include "detours.h"
#include "CreateDump.h"
#include <iostream>
#include <Client.h>
#include <signal.h>

inline void terminator()
{
	int* z = 0; *z = 13;
}

inline void signal_handler(int)
{
	terminator();
}

inline void __cdecl invalid_parameter_handler(const wchar_t*, const wchar_t*, const wchar_t*, unsigned int, uintptr_t)
{
	terminator();
}

LONG WINAPI DumpCallback(_EXCEPTION_POINTERS* excp) {
	std::cout << "DumpCallback" << std::endl;
	CreateDump(excp);
	return EXCEPTION_EXECUTE_HANDLER;
}

//LPVOID g_lp = NULL;
LONG WINAPI NewUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	PEXCEPTION_RECORD ExceptionRecord = ExceptionInfo->ExceptionRecord;
	PCONTEXT ContextRecord = ExceptionInfo->ContextRecord;
	std::cout << "Exception code: " << ExceptionRecord->ExceptionCode << std::endl;
	std::cout << "Exception address: " << ExceptionRecord->ExceptionAddress << std::endl;
	if ((int)ExceptionRecord->ExceptionAddress != 0x00402EFF && !Client::exit) {  //ignore exit crash
		CreateDump(ExceptionInfo);
	}
	//MessageBox( NULL, L"",L"", MB_OK);
	return EXCEPTION_EXECUTE_HANDLER;
}

CAutoDump::CAutoDump(void)
{
	SetUnhandledExceptionFilter(DumpCallback);
	//Enhance error catching start
	signal(SIGABRT, signal_handler);
	_set_abort_behavior(0, _WRITE_ABORT_MSG | _CALL_REPORTFAULT);
	set_terminate(&terminator);
	set_unexpected(&terminator);
	_set_purecall_handler(&terminator);
	_set_invalid_parameter_handler(&invalid_parameter_handler);
	//Enhance error catching end
	m_lpUnhandledExceptionFilter = NULL;
	do {
		SetErrorMode(SEM_NOGPFAULTERRORBOX | SEM_NOOPENFILEERRORBOX);

		m_lpUnhandledExceptionFilter = DetourFindFunction("KERNEL32.DLL", "UnhandledExceptionFilter");
		// 
		// 		g_lp = m_lpUnhandledExceptionFilter;

		if (NULL == m_lpUnhandledExceptionFilter) {
			break;
		}
		LONG lRes = NO_ERROR;
		lRes = DetourTransactionBegin();
		if (NO_ERROR != lRes) {
			break;
		}

		lRes = DetourAttach(&m_lpUnhandledExceptionFilter, NewUnhandledExceptionFilter);
		if (NO_ERROR != lRes) {
			break;
		}

		lRes = DetourTransactionCommit();
		if (NO_ERROR != lRes) {
			break;
		}
	} while (0);
}


CAutoDump::~CAutoDump(void)
{
	if (m_lpUnhandledExceptionFilter) {
		do {
			LONG lRes = NO_ERROR;
			lRes = DetourTransactionBegin();
			if (NO_ERROR != lRes) {
				break;
			}

			lRes = DetourDetach(&m_lpUnhandledExceptionFilter, NewUnhandledExceptionFilter);
			if (NO_ERROR != lRes) {
				break;
			}

			lRes = DetourTransactionCommit();
			if (NO_ERROR != lRes) {
				break;
			}
		} while (0);
	}
}
