// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "NMCO.h"
#include "ijl15.h"
#include "INIReader.h"
#include "ReplacementFuncs.h"
#include <comutil.h>
#include "BossHP.h"
#include <Resman.h>
#include <CharacterEx.h>
#include "ChairRelMove.h"
#include <HeapCreateEx.h>
#include "dllmain.h"

bool isCreate = false;

void Init()
{
	INIReader reader("config.ini");
	if (reader.ParseError() == 0) {
		Client::DefaultResolution = reader.GetInteger("general", "DefaultResolution", Client::DefaultResolution);
		Client::MsgAmount = reader.GetInteger("general", "MsgAmount", Client::MsgAmount);
		Client::WindowedMode = reader.GetBoolean("general", "WindowedMode", Client::WindowedMode);
		Client::RemoveLogos = reader.GetBoolean("general", "RemoveLogos", Client::RemoveLogos);
		Client::SkipWorldSelect = reader.GetBoolean("general", "SkipWorldSelect", Client::SkipWorldSelect);
		Client::RemoveLoginNxIdDialog = reader.GetBoolean("general", "RemoveLoginNxIdDialog", Client::RemoveLoginNxIdDialog);
		Client::ScreenShotPath = reader.GetBoolean("general", "ScreenShotPath", Client::ScreenShotPath);
		Client::RemoveSystemMsg = reader.GetBoolean("general", "RemoveSystemMsg", Client::RemoveSystemMsg);
		Memory::UseVirtuProtect = reader.GetBoolean("general", "UseVirtuProtect", Memory::UseVirtuProtect);
		Client::ResCheckTime = reader.GetInteger("general", "ResCheckTime", Client::ResCheckTime);
		Client::ResManFlushCached = reader.GetInteger("general", "ResManFlushCached", Client::ResManFlushCached);
		Client::SetWorkingSetSize = reader.GetInteger("general", "SetWorkingSetSize", Client::SetWorkingSetSize);
		Client::setDamageCap = reader.GetReal("optional", "setDamageCap", Client::setDamageCap);
		Client::setMAtkCap = reader.GetReal("optional", "setMAtkCap", Client::setMAtkCap);
		Client::setAccCap = reader.GetReal("optional", "setAccCap", Client::setAccCap);
		Client::setAvdCap = reader.GetReal("optional", "setAvdCap", Client::setAvdCap);
		Client::setAtkOutCap = reader.GetReal("optional", "setAtkOutCap", Client::setAtkOutCap);
		Client::longEXP = reader.GetBoolean("optional", "longEXP", Client::longEXP);
		Client::shortLevel = reader.GetBoolean("optional", "shortLevel", Client::shortLevel);
		Client::useTubi = reader.GetBoolean("optional", "useTubi", Client::useTubi);
		Client::speedMovementCap = reader.GetInteger("optional", "speedMovementCap", Client::speedMovementCap);
		Client::jumpCap = reader.GetInteger("optional", "jumpCap", Client::jumpCap);
		Client::debug = reader.GetBoolean("debug", "debug", Client::debug);
		Client::noPassword = reader.GetBoolean("debug", "noPassword", Client::noPassword);
		Client::forceExit = reader.GetBoolean("debug", "forceExit", Client::forceExit);
		Client::ServerIP_AddressFromINI = reader.Get("general", "ServerIP_Address", Client::ServerIP_AddressFromINI);
		Client::ServerIP_Address_hook = reader.GetBoolean("general", "ServerIP_Address_hook", Client::ServerIP_Address_hook);
		Client::climbSpeedAuto = reader.GetBoolean("optional", "climbSpeedAuto", Client::climbSpeedAuto);
		Client::climbSpeed = reader.GetFloat("optional", "climbSpeed", Client::climbSpeed);
		Client::serverIP_Port = reader.GetInteger("general", "serverIP_Port", Client::serverIP_Port);
		Client::talkRepeat = reader.GetBoolean("optional", "talkRepeat", Client::talkRepeat);
		Client::talkTime = reader.GetInteger("optional", "talkTime", Client::talkTime);
		Client::meleePunching = reader.GetBoolean("optional", "meleePunching", Client::meleePunching);
		Client::holdAttack = reader.GetBoolean("optional", "holdAttack", Client::holdAttack);
		Client::spLimit = reader.GetBoolean("optional", "spLimit", Client::spLimit);
		Client::tamingMobUnlock = reader.GetBoolean("optional", "tamingMobUnlock", Client::tamingMobUnlock);
		Client::longSlots = reader.GetBoolean("ui", "LongSlots", Client::longSlots);
		Client::longSlotsKey = reader.GetInteger("ui", "longSlotsKey", Client::longSlotsKey);
		Client::showItemID = reader.GetBoolean("ui", "showItemID", Client::showItemID);
		Client::showWeaponSpeed = reader.GetBoolean("ui", "showWeaponSpeed", Client::showWeaponSpeed);
		Client::minimizeMaptitleColor = reader.GetBoolean("ui", "minimizeMaptitleColor", Client::minimizeMaptitleColor);
		Client::StatDetailBackgrndWidthEx = reader.GetInteger("ui", "StatDetailBackgrndWidthEx", Client::StatDetailBackgrndWidthEx);
		Client::DamageSkin = reader.GetInteger("ui", "DamageSkin", Client::DamageSkin);
		Client::RemoteDamageSkin = reader.GetBoolean("ui", "RemoteDamageSkin", Client::RemoteDamageSkin);
		Client::tamingMob198Effect = reader.GetBoolean("ui", "tamingMob198", Client::tamingMob198Effect);
		Client::s4221001 = reader.GetBoolean("skill", "s4221001", Client::s4221001);
		Client::s4221007 = reader.GetBoolean("skill", "s4221007", Client::s4221007);
		Client::s14101004 = reader.GetBoolean("skill", "s14101004", Client::s14101004);
		Client::s14101004up = reader.GetBoolean("skill", "s14101004up", Client::s14101004up);
		Client::s5221009 = reader.GetBoolean("skill", "s5221009", Client::s5221009);
	}
	if (Client::debug)
		CreateConsole();	//console for devs, use this to log stuff if you want
	Hook_CreateMutexA(true); //multiclient //ty darter, angel, and alias!
	HeapCreateEx::HOOK_HeapCreate();
	Hook_gethostbyname(true);
	Client::TimerTask(CreateHook, 10);
}

void CreateConsole() {
	AllocConsole();
	FILE* stream;
	freopen_s(&stream, "CONOUT$", "w", stdout); //CONOUT$
}

bool CreateHook()
{
	if (!Client::canInjected)
		return false;
	if (isCreate)
		return true;
	isCreate = true;
	std::cout << "GetModuleFileName hook created" << std::endl;
	Hook_StringPool__GetString(true); //hook stringpool modification //ty !! popcorn //ty darter
	Hook_StringPool__GetStringW(true);
	std::string processName = Client::GetCurrentProcessName();
	std::cout << "Current process name: " << processName << std::endl;
	Client::CRCBypass();
	//Client::EnableNewIGCipher();
	Resman::Hook_InitializeResMan();
	Resman::Hook_InitInlinkOutlink();
	Hook_SetActiveChairRelMove(true);
	CharacterEx::InitExpOverride(Client::longEXP);
	CharacterEx::InitLevelOverride(Client::shortLevel);
	CharacterEx::InitDamageSkinOverride(Client::DamageSkin > 0 || Client::RemoteDamageSkin);
	CharacterEx::InitHypontizeFix(Client::s5221009);
	HeapCreateEx::MemoryOptimization();
	Client::UpdateGameStartup();
	//Client::LongQuickSlot();
	Client::FixMouseWheel();
	Client::JumpCap();
	Client::NoPassword();
	BossHP::Hook();
	Client::MoreHook();
	Client::Skill();
	//Hook_CItemInfo__GetItemName(Client::showItemID);
	Hook_CItemInfo__GetItemDesc(Client::showItemID);
	ijl15::CreateHook(); //NMCO::CreateHook();
	Client::TimerTask(Client::EmptyMemory, Client::ResCheckTime);
	std::cout << "NMCO hook initialized" << std::endl;
	Client::injected = true;
	return true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	DisableThreadLibraryCalls(hModule);
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
	{
		Init();
		break;
	}
	default: break;
	case DLL_PROCESS_DETACH:
		ExitProcess(0);
	}
	return TRUE;
}