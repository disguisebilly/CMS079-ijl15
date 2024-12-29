#pragma once

#include "AutoTypes.h"
#include "Memory.h"
#include <WinSock2.h>
#include<iostream>
#include<fstream>
#include<sstream>

static bool ownLoginFrame;
static bool ownCashShopFrame;
static bool EzorsiaV2WzIncluded;
//notes from my knowledge as i have not used these kinds of codes practically well
//function replacement is when you replace the original function in the client with your own fake function, usually to add some extra functionality
//for more complex applications you would also need to define the client's variables and reinterpret_cast those (no void this time)
//you need the right calling convention (match client's original or use _fastcall, i havent tried it much)
//it would help to know the benefits and drawbacks of "reinterpret_cast", as well as how it is hooking to prevent accidents
//hooking to the original function will replace it at all times when it is called by the client
//i personally have not tried it more because it requires a very thorough understanding of how the client code works, re-making the parts here,
//and using it, all together, in a way that doesnt break anything
//it would be the best way to do it for very extensive client edits and if you need to replace entire functions in that context but
//code caving is generally easier for short term, one-time patchwork fixes	//thanks you teto for helping me on this learning journey

bool HookGetModuleFileName(bool bEnable);

/// <summary>
/// Creates a detour for the User32.dll CreateWindowExA function applying the following changes:
/// 1. Enable the window minimize box
/// </summary>
inline void HookCreateWindowExA(bool bEnable);

DWORD GetFuncAddress(LPCSTR lpModule, LPCSTR lpFunc);

bool Hook_CreateMutexA(bool bEnable);

bool Hook_gethostbyname(bool bEnable);

struct KeyValuePair {
	int key;
	std::string value;
};

bool Hook_StringPool__GetString(bool bEnable);

bool Hook_StringPool__GetStringW(bool bEnable);

bool Hook_CItemInfo__GetItemName(bool enable);

bool Hook_CItemInfo__GetItemDesc(bool enable);

int __fastcall _Sub_8C9F01_t(DWORD* pThis, void* edx);

bool Hook_Sub_8C9F01(bool enable);