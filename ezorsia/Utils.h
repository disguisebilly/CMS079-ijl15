#pragma once

HWND getMapleStoryHWND();

std::string GetCurrentProcessName();

std::string GetCurrentProcessName();

DWORD GetCurrentMemoryUsage();

void TimerTask(std::function<boolean()> task, unsigned int interval);

std::wstring StringToWString(const std::string& input);

int MessageBoxPos(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, int x, int y);