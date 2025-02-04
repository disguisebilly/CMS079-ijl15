#include "stdafx.h"
#include "Utils.h"
#include "psapi.h"
#include <locale>
#include <iostream>

std::wstring StringToWString(const std::string& s) {
	std::string curLocale = setlocale(LC_ALL, "chs");
	const char* _Source = s.c_str();
	size_t _Dsize = s.size() + 1;
	wchar_t* _Dest = new wchar_t[_Dsize];
	wmemset(_Dest, 0, _Dsize);
	size_t convertedChars = 0;
	mbstowcs_s(&convertedChars, _Dest, _Dsize, _Source, _TRUNCATE);
	std::wstring ws = _Dest;
	delete[] _Dest;
	setlocale(LC_ALL, curLocale.c_str());
	return ws;
}

int MsgBox_X;
int MsgBox_Y;

static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_CREATEWND)
	{
		CBT_CREATEWND* s = (CBT_CREATEWND*)lParam;
		if (s->lpcs->hwndParent == NULL)
		{
			s->lpcs->x = MsgBox_X - s->lpcs->cx / 2;
			s->lpcs->y = MsgBox_Y - s->lpcs->cy / 2;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int MessageBoxPos(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, int x, int y)
{
	HHOOK hHook = SetWindowsHookEx(WH_CBT, &CBTProc, NULL, GetCurrentThreadId());
	MsgBox_X = x;
	MsgBox_Y = y;
	int result = MessageBox(hWnd, lpText, lpCaption, uType);
	if (hHook) UnhookWindowsHookEx(hHook);
	return result;
}

HWND getMapleStoryHWND()
{
	return FindWindow(L"MapleStoryClass", nullptr);
}

std::string GetCurrentProcessName()
{
	char buffer[MAX_PATH];
	DWORD size = GetModuleFileNameA(NULL, buffer, MAX_PATH);
	if (size == 0) {
		return "";
	}
	std::string processPath(buffer);
	size_t pos = processPath.find_last_of('\\');
	if (pos != std::string::npos) {
		return processPath.substr(pos + 1);
	}
	return "";
}

DWORD GetCurrentMemoryUsage()
{
	MEMORYSTATUSEX MS;
	MS.dwLength = sizeof(MS);
	PROCESS_MEMORY_COUNTERS pmc;
	GlobalMemoryStatusEx(&MS);
	GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
	DWORD CurrentMem = pmc.WorkingSetSize / 1048576;
	DWORD TotalMem = MS.ullTotalPhys / 1048576;
	return CurrentMem;
}

void TimerTask(std::function<boolean()> task, unsigned int interval)
{
	std::thread([task, interval]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			if (task())
				break;
		}
		}).detach();
}

StringList splitstr(const std::string& str, const std::string& pattern)
{
	StringList  li;
	std::string subStr;
	std::string tPattern;
	size_t      patternLen = pattern.length();
	size_t      strLen = str.length();

	for (size_t i = 0; i < str.length(); i++)
	{
		if (pattern[0] == str[i] && ((strLen - i) >= patternLen))
		{
			if (memcmp(&pattern[0], &str[i], patternLen) == 0)
			{
				i += patternLen - 1;
				if (!subStr.empty())
				{
					li.push_back(subStr);
					subStr.clear();
				}
			}
			else
			{
				subStr.push_back(str[i]);
			}
		}
		else
		{
			subStr.push_back(str[i]);
		}
	}

	if (!subStr.empty())
	{
		li.push_back(subStr);
	}

	return li;
}

void SetMonitorRefreshRate(int refreshRate) {
	DEVMODE dm;
	memset(&dm, 0, sizeof(dm));
	dm.dmSize = sizeof(dm);
	dm.dmFields = DM_PELSWIDTH | DM_PELSHEIGHT | DM_DISPLAYFREQUENCY;
	EnumDisplaySettings(NULL, ENUM_CURRENT_SETTINGS, &dm);
	dm.dmDisplayFrequency = refreshRate;
	LONG result = ChangeDisplaySettings(&dm, CDS_TEST);
	if (result == DISP_CHANGE_SUCCESSFUL) {
		result = ChangeDisplaySettings(&dm, CDS_UPDATEREGISTRY);
		if (result == DISP_CHANGE_SUCCESSFUL) {
			std::cout << "Set Refresh Rate: " << refreshRate << " Hz" << std::endl;
		}
		else {
			std::cout << "Set Refresh Rate Error" << std::endl;
		}
	}
	else {
		std::cout << "Set Refresh Rate Error" << std::endl;
	}
}

void CheckMonitorRefreshRate() {
	HDC hdc = GetDC(NULL);
	int refreshRate = GetDeviceCaps(hdc, VREFRESH);
	ReleaseDC(NULL, hdc);
	std::cout << "CheckMonitorRefreshRate: " << refreshRate << " Hz" << std::endl;
	if (refreshRate < 60) {
		SetMonitorRefreshRate(60);
	}
}