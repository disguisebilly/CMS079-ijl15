#include "stdafx.h"
#include "AutoDump.h"
#include <windows.h>
#include "detours.h"
#include "CreateDump.h"
#include <iostream>
#include <Client.h>

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
