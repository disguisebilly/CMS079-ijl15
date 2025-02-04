#pragma once

HWND getMapleStoryHWND();

std::string GetCurrentProcessName();

std::string GetCurrentProcessName();

DWORD GetCurrentMemoryUsage();

void TimerTask(std::function<boolean()> task, unsigned int interval);

std::wstring StringToWString(const std::string& input);

int MessageBoxPos(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, int x, int y);

typedef std::vector<std::string>  StringList;
StringList splitstr(const std::string& str, const std::string& pattern);

void SetMonitorRefreshRate(int refreshRate);

void CheckMonitorRefreshRate();
