#include "AutoDump.h"
#include <windows.h>
#include "detours.h"
#include "CreateDump.h"
#include <iostream>

//LPVOID g_lp = NULL;
LONG WINAPI NewUnhandledExceptionFilter(struct _EXCEPTION_POINTERS* ExceptionInfo) {
	OutputDebugString(L"NewUnhandledExceptionFilter\n");
	std::cout << "NewUnhandledExceptionFilter" << std::endl;
	//MessageBox( NULL, L"",L"", MB_OK);
	// 
	// 	typedef LONG (WINAPI * PNewUnhandledExceptionFilter)( struct _EXCEPTION_POINTERS *ExceptionInfo );
	// 	((PNewUnhandledExceptionFilter)(g_lp))(ExceptionInfo);
	CreateDump(ExceptionInfo);
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
