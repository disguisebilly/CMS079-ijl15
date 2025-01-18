#include "stdafx.h"
#include "AddyLocations.h"
#include <cstringt.h>
#include "FixBuddy.h"
#include <Resman.h>
#include "psapi.h"
#include <PetEx.h>

int Client::DefaultResolution = 2;
int Client::MsgAmount = 10;
bool Client::WindowedMode = true;
bool Client::RemoveLogos = true;
bool Client::SkipWorldSelect = true;
bool Client::RemoveSystemMsg = true;
bool Client::RemoveLoginNxIdDialog = true;
int Client::ScreenShotPath = 0;
int Client::ResCheckTime = 1000;
int Client::ResFlushTimeInterval = 5;
int Client::ResManFlushCached = 640;
int Client::SetWorkingSetSize = 768;
int Client::setDamageCap = 199999;
int Client::setMAtkCap = 1999;
int Client::setAccCap = 999;
int Client::setAvdCap = 999;
double Client::setAtkOutCap = 199999;
bool Client::longEXP = false;
bool Client::shortLevel = false;
bool Client::useTubi = false;
int Client::speedMovementCap = 140;
bool Client::noPassword = false;
bool Client::forceExit = true;
bool Client::linkNodeNew = true;
bool Client::debug = false;
bool Client::crashAutoDump = false;
bool Client::climbSpeedAuto = false;
bool Client::canInjected = false;
std::mutex Client::injected;
std::condition_variable Client::injectedCondition;
bool Client::exit = false;
bool Client::ServerIP_Address_hook = true;
float Client::climbSpeed = 1.0;
std::string Client::ServerName = "MapleStory";
std::string Client::WelcomeMessage = "";
std::string Client::ServerIP_AddressFromINI = "127.0.0.1";
int Client::serverIP_Port = 8484;
bool Client::talkRepeat = false;
int Client::talkTime = 2000;
bool Client::longSlots = false;
int Client::longSlotsKey = 1;
bool Client::showItemID = false;
bool Client::showWeaponSpeed = true;
bool Client::minimizeMaptitleColor = false;
bool Client::meleePunching = true;
bool Client::holdAttack = false;
bool Client::spLimit = true;
int Client::StatDetailBackgrndWidthEx = 23;
int Client::DamageSkin = 0;
bool Client::RemoteDamageSkin = false;
bool Client::tamingMobUnlock = false;
bool Client::tamingMob198Effect = false;
bool Client::replacePetEquipCheck = false;
int Client::downJumpLimitHeight = 300;
bool Client::unlockPanelLimit = false;
bool Client::unlockPaneMaplLimit = false;
bool Client::s14101004 = true;
bool Client::s14101004up = false;
bool Client::s4221001 = false;
bool Client::s4221007 = false;
bool Client::s5221009 = false;

void Client::UpdateGameStartup() {
	Resolution::Init();
	PetEx::HookPetCheckCanEquip(Client::replacePetEquipCheck);
	if (Client::forceExit)
		Memory::CodeCave(fExit, 0x009FC170, 12);
	Memory::CodeCave(onExit, 0x00A7355C, 5);

	FixBuddy::Hook();  //unkown

	if (useTubi) { Memory::FillBytes(0x00485173, 0x90, 2); }

	Memory::WriteInt(0x007868CF + 1, 2147483646); // 物攻PAD 相关具体不明，默认值1999，int 4字节
	Memory::WriteInt(0x007869A9 + 1, 2147483646); // 技能 相关具体不明，默认值1999，int 4字节
	Memory::WriteInt(0x008C8BAE + 1, setDamageCap); // 物攻面板，默认值199999，int 4字节
	Memory::WriteInt(0x00786A8F + 1, setMAtkCap); // 魔法防御力面板
	Memory::WriteInt(0x0078876B + 1, setMAtkCap); // 魔攻面板
	Memory::WriteInt(0x0078881B + 1, setAccCap); // 命中，默认999
	Memory::WriteInt(0x0078884D + 1, setAvdCap); // 回避，默认999
	Memory::WriteInt(0x0079617F + 1, 2147483646); // 计算物理伤害相关，意义不明，默认1999，int 4字节
	Memory::WriteInt(0x00796BF2 + 1, 2147483646); // 计算魔攻MDamage的，默认值1999，int 4字节，注意：这里不改的话，打怪输出计算的魔法伤害就是按1999计算的
	Memory::WriteInt(0x007971CC + 1, 2147483646); // 计算魔攻MDamage的，默认值1999，int 4字节，注意：这里不改似乎也不影响输出计算
	Memory::WriteInt(0x007942B1 + 1, 2147483646); //CalcDamage::PDamage 999，意义不明，int 4字节
	Memory::WriteInt(0x007948AF + 1, 2147483646); //CalcDamage::PDamage 999，意义不明，int 4字节
	Memory::WriteInt(0x00796DE6 + 1, 2147483646); //CalcDamage::MDamage 999，意义不明，int 4字节

	Memory::WriteDouble(0x00B064B8, setAtkOutCap);	// 输出显示上限，默认199999，double 8字节


	Memory::WriteInt(0x0078888E + 3, speedMovementCap); //set speed cap //ty ronan
	Memory::WriteInt(0x008C9B1A + 1, speedMovementCap); //set speed cap //ty ronan
	Memory::WriteInt(0x00954534 + 1, speedMovementCap); //set speed cap //ty ronan

	if (Client::ServerIP_Address_hook)
		Memory::CodeCave(changerPort, 0x0075F186, 5);
	//Memory::WriteInt(0x0075F142 + 1, serverIP_Port);
	if (WindowedMode) {
		unsigned char forced_window[] = { 0xB8, 0x00, 0x00, 0x00, 0x00 }; //force window mode	//thanks stelmo for showing me how to do this
		Memory::WriteByteArray(0x00A00EF2, forced_window, sizeof(forced_window));//force window mode
	}
	if (RemoveLogos) {
		Memory::FillBytes(0x0065A398, 0x90, 20);	//no Logo @launch
	}
	if (SkipWorldSelect) {
		Memory::CodeCave(skipWorldSelect, 0x005BB9C1, 5);
		Memory::CodeCave(skipWorldSConnectError, 0x004948BC, 5);
	}
	if (RemoveLoginNxIdDialog) {
		Memory::FillBytes(0x0062C650, 0x90, 5);	//屏蔽登录蓝色弹窗
	}
	Memory::FillBytes(0x009FB8AD, 0x90, 5);     //移除启动广告(remove start ads )
	Memory::WriteByte(0x009FC0CB, 0xEB);	//jz 009FC13A ; jmp 移除退出广告(remove exit ads)
	Memory::CodeCave(unlockPacket, 0x007DADB8, 5);

	//修改截图位置
	if (Client::ScreenShotPath < 0 || Client::ScreenShotPath > 2)
		Client::ScreenShotPath = 0;
	Memory::CodeCave(screenShotPath, 0x00751AD9, 5);

	//修复部分装备没有Canvas闪退
	Memory::CodeCave(fixSomeEuipmentNotCanvasCrash, 0x00401D52, 6);
}

void Client::EnableNewIGCipher() {//??not called //no idea what cipher is
	const int nCipherHash = m_nIGCipherHash;
	Memory::WriteInt(dwIGCipherHash + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherVirtual1 + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherVirtual2 + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherDecrypt + 3, nCipherHash);
	Memory::WriteInt(dwIGCipherDecryptStr + 3, nCipherHash);
}

void Client::UpdateLogin() {	//un-used //may still contain some useful addresses for custom login
	Memory::CodeCave(PositionLoginDlg, dwLoginCreateDlg, 14);
	Memory::CodeCave(PositionLoginUsername, dwLoginUsername, 11);
	Memory::CodeCave(PositionLoginPassword, dwLoginPassword, 8);
	Memory::WriteInt(dwLoginInputBackgroundColor + 3, 0xFFF8FAFF); // ARGB value
	Memory::WriteByte(dwLoginInputFontColor + 3, 1); // bool: true=black, false=white
	Memory::WriteInt(dwLoginLoginBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginFindPasswordBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginQuitBtn + 1, -127); // x-pos
	Memory::WriteInt(dwLoginFindIDBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginFindIDBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginWebHomeBtn + 1, -127); // x-pos
	Memory::WriteByte(dwLoginWebRegisterBtn + 1, -127); // x-pos
}

void Client::FixMouseWheel() {
	Memory::CodeCave(fixMouseWheelHook, 0x009F1AD5, 5);
}

void Client::LongQuickSlot() {
	if (!longSlots)
		return;
	// CUIStatusBar::OnCreate
	Memory::WriteByte(0x008D6547 + 1, 0xF0); // Draw rest of quickslot bar
	Memory::WriteByte(0x008D6547 + 2, 0x03);
	Memory::WriteByte(0x008D6819 + 1, 0xF0); // Draw rest of hotkeys
	Memory::WriteByte(0x008D6819 + 2, 0x03);
	Memory::WriteByte(0x008D6AAB + 1, 0xF0); // Draw rest of cooldowns, who tf knows why. TY Rulax
	Memory::WriteByte(0x008D6AAB + 2, 0x03);

	//----CQuickslotKeyMappedMan::CQuickslotKeyMappedMan?????
	//Memory::WriteInt(0x0072B7CE + 1, (DWORD)&Array_aDefaultQKM_0);  //??
	//Memory::WriteInt(0x0072B8EB + 1, (DWORD)&Array_aDefaultQKM_0);  //??

	//----CUIStatusBar::CQuickSlot::CompareValidateFuncKeyMappedInfo
	Memory::WriteByte(0x008E269D, 0x1A); //increase 8 --> 26
	Memory::WriteByte(0x008E243D, 0x1A); //increase 8 --> 26
	Memory::WriteByte(0x008E2485, 0xBB);
	Memory::WriteInt(0x008E2485 + 1, (DWORD)&Array_Expanded);
	Memory::WriteByte(0x008E2485 + 5, 0x90); //Errant byte
	Memory::WriteByte(0x008E242B, 0xB8);
	Memory::WriteInt(0x008E242B + 1, (DWORD)&Array_Expanded);
	Memory::WriteByte(0x008E242B + 5, 0x90); //Errant Byte

	//----CUIStatusBar::CQuickSlot::Draw
	Memory::WriteByte(0x008E3239 + 3, 0x1A);
	Memory::WriteByte(0x008E2AA5, 0xB8);
	Memory::WriteInt(0x008E2AA5 + 1, (DWORD)&Array_Expanded);
	Memory::FillBytes(0x008E2AA5 + 5, 0x90, 3); // Nopping errant operations

	//----CUIStatusBar::OnMouseMove                  
	Memory::WriteByte(0x008DCA28 + 1, 0x34);
	Memory::WriteByte(0x008DCA28 + 2, 0x85);
	Memory::WriteInt(0x008DCA28 + 3, (DWORD)&Array_Expanded);

	//----CUIStatusBar::CQuickSlot::GetPosByIndex
	Memory::WriteInt(0x008E3428 + 2, (DWORD)&Array_ptShortKeyPos);
	Memory::WriteInt(0x008E3430 + 2, (DWORD)&Array_ptShortKeyPos + 4);
	Memory::WriteByte(0x008E341C + 2, 0x1A); //change cmp 8 --> cmp 26

	//CUIStatusBar::GetShortCutIndexByPos
	Memory::WriteInt(0x008E33CF + 1, (DWORD)&Array_ptShortKeyPos_Fixed_Tooltips + 4);
	Memory::WriteInt(0x008E3401 + 2, (DWORD)&Array_ptShortKeyPos_Fixed_Tooltips + 416);//26*2*8
	//Memory::WriteByte(0x008E3401 + 1, 0x3E);
	//Memory::CodeCave(removeKeyToolTips, 0x005CAF32, 5); 屏蔽Tooltips

	//CUIStatusBar::CQuickSlot::DrawSkillCooltime
	Memory::WriteByte(0x008E5BE8 + 3, 0x1A);
	Memory::WriteByte(0x008E58E6, 0xBE);
	Memory::WriteInt(0x008E58E6 + 1, (DWORD)&cooldown_Array); //Pass enlarged FFFFF array
	Memory::WriteByte(0x008E58E6 + 5, 0x90); //Errant byte
	Memory::WriteByte(0x008E58EC, 0xBF);
	Memory::WriteInt(0x008E58EC + 1, (DWORD)&Array_Expanded + 1);
	Memory::WriteByte(0x008E58EC + 5, 0x90);

	//----CDraggableMenu::OnDropped   ??
	//Memory::WriteByte(0x004F928A + 2, 0x1A); //change cmp 8 --> cmp 26
	//----CDraggableMenu::MapFuncKey
	Memory::WriteByte(0x00508417 + 2, 0x1A); //change cmp 8 --> cmp 26
	////----CUIKeyConfig::OnDestroy      ??
	//Memory::WriteByte(0x00833797 + 2, 0x6C); // Updates the offset to 108 (triple) (old->24h)    
	//Memory::WriteByte(0x00833841 + 2, 0x6C); // Updates the offset to 108 (triple) (old->24h)
	//Memory::WriteByte(0x00833791 + 1, 0x68); // push 68h (triple)
	//Memory::WriteByte(0x0083383B + 1, 0x68); // push 68h (triple)
	////----CUIKeyConfig::~CUIKeyConfig  ??
	//Memory::WriteByte(0x0083287F + 2, 0x6C); // triple the base value at this hex (old->24h)
	//Memory::WriteByte(0x00832882 + 1, 0x68); // push 68h (triple)
	////----CQuickslotKeyMappedMan::SaveQuickslotKeyMap  ??
	//Memory::WriteByte(0x0072B8C0 + 2, 0x6C); // triple the base value at this hex (old->24h)
	//Memory::WriteByte(0x0072B8A0 + 1, 0x68); // push 68h, (triple) //CQuickslotKeyMappedMan::SaveQuickslotKeyMap
	//Memory::WriteByte(0x0072B8BD + 1, 0x68); // push 68h, (triple) //CQuickslotKeyMappedMan::SaveQuickslotKeyMap
	////----CQuickslotKeyMappedMan::OnInit  ??
	//Memory::WriteByte(0x0072B861 + 1, 0x68); // push 68h (triple) (these ones might have to be just 60)
	//Memory::WriteByte(0x0072B867 + 2, 0x6C); // triple the base value at this hex (old->24h)
	////----CUIKeyConfig::CNoticeDlg::OnChildNotify     ??
	//Memory::WriteByte(0x00836A1E + 1, 0x68); // push 68h (triple)
	//Memory::WriteByte(0x00836A21 + 2, 0x6C); // triple the base value at this hex (old->24h)


	//// CODECAVES CLIENT EDITS ---- 
	Memory::CodeCave(CompareValidateFuncKeyMappedInfo_cave, 0x008E2448, 5);
	//Memory::CodeCave(sub_9FA0CB_cave, 0x9FA0DB, 5);                          //??
	//Memory::CodeCave(sDefaultQuickslotKeyMap_cave, 0x72B7BC, 5);             //??
	//Memory::CodeCave(DefaultQuickslotKeyMap_cave, 0x72B8E6, 5);              //??
	Memory::CodeCave(Restore_Array_Expanded, 0x006282FC, 6); //restores the skill array to 0s
	// KeyMap
	switch (Client::longSlotsKey) {
	case 2:
		slotKeyMap = (DWORD)Array_keyMap_offset2;
		break;
	case 3:
		slotKeyMap = (DWORD)Array_keyMap_offset3;
		break;
	default:
		slotKeyMap = (DWORD)Array_keyMap_offset;
		break;
	}
	Memory::CodeCave(DefaultQuickslotKeyMap79_cave, 0x005BA1C6, 7);
}

DWORD Client::jumpCap = 123;

void Client::JumpCap() {
	Memory::CodeCave(customJumpCapHook1, 0x007888E2, 10);
	Memory::CodeCave(customJumpCapHook2, 0x008C9B37, 10);
	Memory::CodeCave(customJumpCapHook3, 0x00954558, 5);

	Memory::WriteInt(0x009D48AC + 2, (DWORD)&climbSpeedSave);
	Memory::WriteInt(0x009D8158 + 2, (DWORD)&climbSpeedSave);  //商场绳子
	if (climbSpeedAuto)
	{
		Memory::CodeCave(calcSpeedHook, 0x00954552, 6);
		Memory::CodeCave(calcSpeedHook2, 0x00954416, 6);
	}
	else {
		Memory::WriteDouble((DWORD)&climbSpeedSave, climbSpeed * 3.0);
	}
}

void Client::CRCBypass() {
	Memory::WriteInt(0x00D02B4B + 1, 0x0752976C);
	Memory::WriteByte(0x00A5A464, 0xC3);
	Memory::WriteInt(0x00A5A949, 0xC3);
	Memory::WriteByte(0x009FE755, 0x75);
}

void Client::NoPassword() {
	if (noPassword)
	{
		Memory::WriteInt(0x0064C529 + 2, 0);
	}
}

bool Hook_Sub_8C9F01(bool enable);

void Client::MoreHook() {

	if (!Client::showWeaponSpeed)
		Memory::WriteByte(0x008F2EFF, 0xEB);//hide draw weapons speed

	if (!Client::meleePunching) {
		Memory::FillBytes(0x00958D81, 0x90, 2);
	}

	if (Client::holdAttack) {
		byte holdAttackBytes[] = { 0x0C ,0xEB };
		Memory::WriteByteArray(0x0095C4F1, holdAttackBytes, sizeof(holdAttackBytes));
	}

	Memory::CodeCave(faceHairCave, 0x005FCEFF, 10);
	Memory::CodeCave(faceHairNpcCave, 0x009B7E99, 18);
	Memory::CodeCave(canSendPkgTimeCave, 0x00485169, 10);

	// Worldmap Center on open codecave
	Memory::CodeCave(CodeCave_CWorldMapDlg__OnCreate, 0x009F51ED, 13);


	if (talkRepeat)
	{
		Memory::WriteByte(0x004900C9 + 1, 5);
	}
	Memory::WriteInt(0x00490127 + 2, talkTime);

	// 装备面板
	//Memory::WriteInt(0x00815A5E + 1, Client::StatBackgrndWidth); // 装备面板面板初始x
	//Memory::WriteInt(0x00816786 + 1, Client::StatBackgrndWidth); // 装备面板切换x

	if (!Client::spLimit) {
		Memory::WriteByte(0x008BE236, 0xEB);
		byte spLimitBytes[] = { 0xE9 ,0xBC,0x00,0x00,0x00,0x90 };
		Memory::WriteByteArray(0x008BE18D, spLimitBytes, sizeof(spLimitBytes));
		byte spLimitBytes2[] = { 0xE9 ,0x67,0x01,0x00,0x00,0x90 };
		Memory::WriteByteArray(0x008BE0E2, spLimitBytes2, sizeof(spLimitBytes2));
	}

	if (Client::tamingMob198Effect) {
		//TamingMob支持198
		Memory::CodeCave(tamingMob198, 0x00407A4C, 5);
		Memory::CodeCave(tamingMob198_1, 0x00413FAC, 5);
		Memory::CodeCave(tamingMob198_2, 0x0041437A, 7);
		Memory::CodeCave(tamingMob198_3, 0x00451935, 5);
		Memory::CodeCave(tamingMob198_4, 0x0045279F, 5);
		Memory::CodeCave(tamingMob198_5, 0x00452810, 5);
		Memory::CodeCave(tamingMob198_6, 0x00452C5C, 5);
		Memory::CodeCave(tamingMob198_7, 0x0045451B, 5);
		Memory::CodeCave(tamingMob198_8, 0x00454543, 5);
		Memory::CodeCave(tamingMob198_9, 0x004545FF, 5);
		Memory::CodeCave(tamingMob198_10, 0x00454631, 5);
		Memory::CodeCave(tamingMob198_11, 0x004546D0, 5);
		Memory::CodeCave(tamingMob198_12, 0x0045475B, 5);
		Memory::CodeCave(tamingMob198_13, 0x0045499C, 5);
		Memory::CodeCave(tamingMob198_14, 0x0045653B, 5);
		Memory::CodeCave(tamingMob198_15, 0x004B1B8C, 5);
		Memory::CodeCave(tamingMob198_16, 0x0066FB1E, 5);
		Memory::CodeCave(tamingMob198_17, 0x0066FCA6, 5);
		Memory::CodeCave(tamingMob198_18, 0x007AAA50, 5);
		Memory::CodeCave(tamingMob198_19, 0x008C9EF1, 5);
		Memory::CodeCave(tamingMob198_20, 0x008C9F5B, 5);
		Memory::CodeCave(tamingMob198_21, 0x0093A8E3, 5);
		Memory::CodeCave(tamingMob198_22, 0x0093CED9, 5);
		Memory::CodeCave(tamingMob198_23, 0x0094220F, 5);
		Memory::CodeCave(tamingMob198_24, 0x0096494E, 5);
		Memory::CodeCave(tamingMob198_25, 0x0096CA51, 6);
		/*Memory::CodeCave(tamingMob198_26, 0x0097452E, 5);*/
		Memory::CodeCave(tamingMob198_27, 0x00A129EC, 7);
		Memory::CodeCave(tamingMob198_28, 0x0096CA91, 7);
		Memory::CodeCave(tamingMob198_29, 0x00413E3C, 5);
		Memory::CodeCave(tamingMob198_30, 0x005FD1BA, 6);
	}

	if (Client::tamingMobUnlock || Client::tamingMob198Effect) {
		Memory::CodeCave(tamingMob198_26, 0x0097452E, 5);
	}

	if (Client::tamingMobUnlock) {
		Memory::FillBytes(0x00413E8C, 0x90, 2);
		//byte unlockBytes[] = { 0xEB ,0x26 };
		//Memory::WriteByteArray(0x008C9F0B, unlockBytes, sizeof(unlockBytes));
		Hook_Sub_8C9F01(true);
		Memory::FillBytes(0x0093CF16, 0x90, 2);
	}

	Memory::WriteByte(0x00942A01, 0xEB);

	if (Client::WelcomeMessage.empty()) {
		Memory::FillBytes(0x00A0CD7D, 0x90, 5);
	}

	if (Client::RemoveSystemMsg) {
		Memory::FillBytes(0x00A0C8CA, 0x90, 5);
	}

	Memory::WriteInt(0x00953224 + 2, Client::downJumpLimitHeight);

	if (Client::unlockPanelLimit) {
		Memory::WriteByte(0x00A1AACD, 0xEB);  //unlock attack during 
	}

	if (Client::unlockPaneMaplLimit) {
		Memory::WriteByte(0x00A1AB9B, 0xEB);  //auction
		Memory::WriteByte(0x00A0E14F, 0xEB);  //shop
	}

}

void Client::Skill() {
	//快捷二段跳
	if (Client::s14101004) {
		Memory::CodeCave(doubleJump, 0x00955F51, 5);
	}
	if (Client::s14101004up) {
		Memory::CodeCave(upDoubleJump, 0x0097990A, 7);
	}
	//暗杀
	if (Client::s4221001) {
		Memory::FillBytes(0x007964CB, 0x90, 5);
		Memory::FillBytes(0x00796502, 0x90, 5);
		Memory::WriteByte(0x0097642E, 0xEB);
	}
	//一出双击
	if (Client::s4221007) {
		byte s4221001[] = { 0xB8,0xFF,0xFF ,0xFF ,0xFF };
		Memory::WriteByteArray(0x0097A28F, s4221001, sizeof(s4221001));
	}
}

std::chrono::system_clock::time_point start = std::chrono::system_clock::now();

bool Client::EmptyMemory()
{
	try {
		int memory = Client::GetCurrentMemoryUsage();
		std::chrono::system_clock::time_point end = std::chrono::system_clock::now();
		std::chrono::duration<double> diff = end - start;
		if (memory >= Client::ResManFlushCached && diff.count() >= Client::ResFlushTimeInterval) {
			if (Resman::getIWzResMan())
			{
				Resman::getIWzResMan()->raw_FlushCachedObjects(0);
			}
			autoFlushCacheTime(0);
			callFlushcache();
			autoFlushCacheTime(10000);
			std::cout << "Try flushcache:" << memory << std::endl;
			start = std::chrono::system_clock::now();;
		}
		if (memory >= Client::SetWorkingSetSize)
		{
			HANDLE hProcess = OpenProcess(2035711, 0, GetCurrentProcessId());
			if (hProcess) {
				std::cout << "Save Memory:" << memory << " " << GetCurrentProcessId()
					<< " " << SetProcessWorkingSetSize(hProcess, -1, -1) << std::endl;
				CloseHandle(hProcess);
			}
			else {
				std::cout << "Save Memory OpenProcess error!" << std::endl;
			}
		}
	}
	catch (...) {}
	return false;
}

std::string Client::GetCurrentProcessName()
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

DWORD Client::GetCurrentMemoryUsage()
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

void Client::TimerTask(std::function<boolean()> task, unsigned int interval)
{
	std::thread([task, interval]() {
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(interval));
			if (task())
				break;
		}
		}).detach();
}

int MsgBox_X;
int MsgBox_Y;

static LRESULT CALLBACK CBTProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HCBT_CREATEWND)
	{
		CBT_CREATEWND* s = (CBT_CREATEWND*)lParam;
		if (((LPCBT_CREATEWND)lParam)->lpcs->hwndParent == NULL)
		{
			((LPCBT_CREATEWND)lParam)->lpcs->x = MsgBox_X;
			((LPCBT_CREATEWND)lParam)->lpcs->y = MsgBox_Y;
		}
	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int Client::MessageBoxPos(HWND hWnd, LPCTSTR lpText, LPCTSTR lpCaption, UINT uType, int x, int y)
{
	HHOOK hHook = SetWindowsHookEx(WH_CBT, &CBTProc, NULL, GetCurrentThreadId());
	MsgBox_X = x;
	MsgBox_Y = y;
	int result = MessageBox(hWnd, lpText, lpCaption, uType);
	if (hHook) UnhookWindowsHookEx(hHook);
	return result;
}
