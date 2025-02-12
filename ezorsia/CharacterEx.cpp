#include "stdafx.h"
#include "CharacterEx.h"
#include <string>
#include<iostream>
#include<fstream>
#include<sstream>

/*
	character data extension class. stores exp for now.
	will eventually store meso and maybe level if im feeling like i dont want a life for a month
*/
struct CharacterDataEx
{
private:
	static CharacterDataEx* m_pInstance;
public:
	LONGLONG m_liExp;
	LONGLONG m_liExpMsg;
	tsl::robin_map<int, int> h_liLevel;

	CharacterDataEx()
	{
		m_liExp = 0;
		m_liExpMsg = 0;
	}

	int GetCharLevel();

	static CharacterDataEx* GetInstance()
	{
		if (!m_pInstance)
		{
			m_pInstance = new CharacterDataEx();
		}

		return m_pInstance;
	}
};


CharacterDataEx* CharacterDataEx::m_pInstance;
int CharacterEx::m_maxHp = 0;
int CharacterEx::m_maxMp = 0;
double CharacterEx::m_atkOutCap = 199999;
tsl::robin_map<int, int> CharacterEx::h_userSkin;

std::vector<LONGLONG> myArrayForCustomEXP;

//const LONGLONG maxLevelForCustomEXP = sizeof(myArrayForCustomEXP);

auto _get_next_level_exp = (int(__cdecl*)(int level))0x00793711;

LONGLONG get_next_level_exp()
{
	int level = CharacterDataEx::GetInstance()->GetCharLevel();
	if (myArrayForCustomEXP.empty()) {
		return _get_next_level_exp(level);
	}
	else {
		int length = myArrayForCustomEXP.size();
		if (level >= length) return myArrayForCustomEXP[length - 1];
		return myArrayForCustomEXP[level];
	}
}

BYTE _GetCharLevel() {
	auto CUserLocal__GetCharacterLevel = (BYTE(__fastcall*)(PVOID pThis, PVOID edx))0x00950DE3;

	PVOID CUserLocal__ms_pInstance = *reinterpret_cast<void**>(0x00BD9028);

	return CUserLocal__GetCharacterLevel(CUserLocal__ms_pInstance, NULL);
}

int CharacterDataEx::GetCharLevel()
{
	return CharacterDataEx::GetInstance()->h_liLevel[CharacterEx::getLoginUserId()];
}

char* __cdecl itoa_ExpSwap(int value, PCHAR buffer, int radix)
{
	_i64toa_s(CharacterDataEx::GetInstance()->m_liExp, buffer, 64, radix);
	// TODO abbreviate large numbers to something like 14.123B or something -- maybe make toggleable through some UI setting??
	return buffer;
}

int __stdcall getUserlevel(int userId) {
	return CharacterDataEx::GetInstance()->h_liLevel[userId];
}

LPWSTR __stdcall _itoa_LevelSwap(int userID, int value, LPWSTR lpWideCharStr, int radix)
{
	CHAR MultiByteStr[64] = {};
	_itoa_s(getUserlevel(userID), MultiByteStr, _countof(MultiByteStr), radix);;
	MultiByteToWideChar(0, 1u, MultiByteStr, -1, lpWideCharStr, 64);
	return lpWideCharStr;
}

__declspec(naked) void itoa_LevelSwap() {
	__asm {
		push[esi]
		call _itoa_LevelSwap
		push 0x0063458F
		ret
	}
}

/* all arguments passed on the stack despite being a member function */
void __cdecl FormatExpString_Hook(ZXString<char>* pThis, const char* originalstring, int curexp, int nextlevelexp)
{
	std::string s = std::to_string(CharacterDataEx::GetInstance()->m_liExp);
	s.append(" / ");
	s.append(std::to_string(get_next_level_exp()));

	pThis->Assign(s.c_str(), s.length());
}

void __cdecl FormatExpMsgString_Hook(ZXString<char>* pThis, const char* originalstring, int expmsg)
{
	std::stringstream ss;
	ss << "得到经验值(+";
	ss << CharacterDataEx::GetInstance()->m_liExpMsg;
	ss << ")";
	std::string s = ss.str();

	pThis->Assign(s.c_str(), s.length());
}

typedef void* (__cdecl* _lpfn_NextLevel_t)(int[]);
static auto _lpfn_NextLevel = reinterpret_cast<_lpfn_NextLevel_t>(0x00793711);


auto CUIStatusBar__SetNumberValue_t = (void(__thiscall*)(void*, int, int, int, int, int, int, int))0x008DD003;
void __fastcall CUIStatusBar__SetNumberValue_Hook(void* pThis, void* edx, int hp, int hpMax, int mp, int mpMax, int exp, int expMax, int tempExp)
{
	LONGLONG liExp = CharacterDataEx::GetInstance()->m_liExp;
	LONGLONG liExpMax = get_next_level_exp();

	/* this adjusts the exp bar gauge -- idk how else to do this lmao, we're essentially scaling the exp down until itll fit in the data type */
	while (liExpMax > INT_MAX || liExp > INT_MAX)
	{
		liExp >>= 2;
		liExpMax >>= 2;
	}

	exp = liExp;
	expMax = liExpMax;
	CharacterEx::m_maxHp = hpMax;
	CharacterEx::m_maxMp = mpMax;

	if (hp > hpMax) {
		hpMax = hp;
	}

	if (mp > mpMax) {
		mpMax = mp;
	}

	CUIStatusBar__SetNumberValue_t(pThis, hp, hpMax, mp, mpMax, exp, expMax, tempExp);
}

//void* __fastcall _lpfn_NextLevel_Hook(LONGLONG expTable[maxLevelForCustomEXP])	 //your max level is the size of your array 
//{
//	memcpy(expTable, myArrayForCustomEXP, maxLevelForCustomEXP);	//ty to creator of github.com/PurpleMadness/CustomExpTable
//	expTable[maxLevelForCustomEXP] = 0;	//insert your own formula or predefined array into this part. MUST MATCH server numbers
//	return expTable;					//currently using predefined array	
//}

int __fastcall ExpSwap__Decode4To8msg(CInPacket* pThis, void* edx)
{
	LONGLONG liExpMsg;

	if (Client::longEXP) {
		pThis->DecodeBuffer(&liExpMsg, sizeof(liExpMsg));
	}
	else {
		liExpMsg = (int)pThis->Decode4();
	}

	CharacterDataEx::GetInstance()->m_liExpMsg = liExpMsg;

	return liExpMsg < INT_MAX ? (INT)liExpMsg : INT_MAX;
}

int __fastcall ExpSwap__Decode4To8(CInPacket* pThis, void* edx)
{
	LONGLONG liExp;

	if (Client::longEXP) {
		pThis->DecodeBuffer(&liExp, sizeof(liExp));
	}
	else {
		liExp = (int)pThis->Decode4();
	}

	CharacterDataEx::GetInstance()->m_liExp = liExp;

	return liExp < INT_MAX ? (INT)liExp : INT_MAX;
}

char __fastcall LevelSwap__Decode(CInPacket* pThis, void* edx, int userID)
{
	int level = 0;

	switch (Client::levelType)
	{
	case 1:
		level = pThis->Decode2();
		break;
	case 2:
		level = pThis->Decode4();
		break;
	default:
		level = (unsigned char)pThis->Decode1();
		break;
	}

	CharacterDataEx::GetInstance()->h_liLevel[userID] = level;

	//std::cout << "UserID: " << userID << " Level: " << level << std::endl;

	return level < UCHAR_MAX ? (char)level : UCHAR_MAX;
}

const char* __fastcall LevelOnCharacterInfoStr(ZXString<char>* pThis, PVOID edx, int userID)
{
	std::string s = std::to_string(getUserlevel(userID));

	pThis->Assign(s.c_str(), s.length());

	return static_cast<const char*>(*pThis);
}

__declspec(naked) void onCharacterInfoStrSwap() {
	__asm {
		push[esi + 0x62C]
		call LevelOnCharacterInfoStr
		push 0x0090892E
		ret
	}
}

const char* __fastcall ZXString__GetConstCharString(ZXString<char>* pThis, PVOID edx)
{
	std::ostringstream oss;

	oss << CharacterDataEx::GetInstance()->m_liExp;

	char pencent[64];
	float value = std::floor((double)CharacterDataEx::GetInstance()->m_liExp / get_next_level_exp() * 10000) / 100;
	sprintf_s(pencent, " (%.2f%%)", min(value, 100.0f));

	oss << pencent;

	std::string s = oss.str();

	pThis->Assign(s.c_str(), s.length());

	return static_cast<const char*>(*pThis);
}

const char* __fastcall ZXString_LevelString(ZXString<char>* pThis, PVOID edx)
{
	std::string s = std::to_string(CharacterDataEx::GetInstance()->GetCharLevel());

	pThis->Assign(s.c_str(), s.length());

	return static_cast<const char*>(*pThis);
}

const DWORD sub_429411 = 0x00429411;
__declspec(naked) void drawLevelString() {
	__asm {
		call sub_429411
		call CharacterDataEx::GetCharLevel
		push eax
		push 0x008DCE4B
		ret
	}
}

__declspec(naked) void decodeChangeStatStatDecode() {
	__asm {
		push[esi]
		call LevelSwap__Decode
		push 0x004F26FD
		ret
	}
}

__declspec(naked) void characterLevelStatDecode() {
	__asm {
		push[esi]
		call LevelSwap__Decode
		push 0x004F2269
		ret
	}
}

__declspec(naked) void onCharacterInfoLevelStatDecode() {
	__asm {
		push[ebp - 0x30]
		call LevelSwap__Decode
		push 0x00A2CE3F
		ret
	}
}

__declspec(naked) void remoteInitLevelStatDecode() {
	__asm {
		push[esi]
		call LevelSwap__Decode
		push 0x0098CE22
		ret
	}
}

__declspec(naked) void onPartyCreateResultLevel() {
	__asm {
		call CharacterDataEx::GetCharLevel
		mov[ebx + 0x2E86], eax
		push 0x00A485DC
		ret
	}
}

unsigned char* __fastcall _guildName_Decode(CInPacket* pThis, void* edx, int userId, ZXString<char>* p)
{
	unsigned char* result = pThis->DecodeStr(p);

	const char* name = static_cast<const char*>(*p);

	std::string s = name;

	StringList res = splitstr(s, "$$");
	if (res.size() == 2) {
		if (res[0] == "#") {
			p->Empty();
		}
		else {
			p->Assign(res[0].c_str(), res[0].length());
		}
		int skin = std::stoi(res[1]);
		//std::cout << pThis << " " << res.size() << " user:" << userId << " skin:" << skin << std::endl;
		CharacterEx::h_userSkin[userId] = skin;
	}
	return result;
}

const DWORD GuildNameDecodeRet = 0x0098CE63;
__declspec(naked) void GuildNameDecode() {
	__asm {
		push[eax + 0x1C]
		call _guildName_Decode
		jmp dword ptr[GuildNameDecodeRet]
	}
}

//int __UserRemoteID = 0;
//int __fastcall CUserPoolOnUserRemotePacket_DecodeID(CInPacket* pThis, void* edx)
//{
//
//	__UserRemoteID = pThis->Decode4();
//
//	return __UserRemoteID;
//}

int __fastcall OnUserLeave_DecodeID(CInPacket* pThis, void* edx)
{

	int userId = pThis->Decode4();

	CharacterEx::h_userSkin.erase(userId);
	CharacterDataEx::GetInstance()->h_liLevel.erase(userId);

	return userId;
}

const DWORD GuildNameDecodeRet2 = 0x009912EC;
__declspec(naked) void GuildNameDecode2() {
	__asm {
		push[esi + 0xF2C]
		call _guildName_Decode
		jmp dword ptr[GuildNameDecodeRet2]
	}
}

void __fastcall CharacterStatSkin(CInPacket* pThis, void* edx, int userId)
{
	CharacterEx::h_userSkin[userId] = pThis->Decode4();
}

void CharacterEx::updateDamgeLimit(int damgeLimit) {
	CharacterEx::m_atkOutCap = damgeLimit;
	Memory::WriteDouble(0x00B064B8, damgeLimit);	// 输出显示上限
	//Memory::WriteInt(0x008C8BAE + 1, damgeLimit); // 物攻面板
	//Memory::WriteInt(0x00786A8F + 1, damgeLimit); // 魔法防御力面板
	//Memory::WriteInt(0x0078876B + 1, damgeLimit); // 魔攻面板
}

void __fastcall CharacterStatDamageLimit(CInPacket* pThis, void* edx, int userId)
{
	int damgeLimit = pThis->Decode4();
	if (damgeLimit > 0) {
		CharacterEx::updateDamgeLimit(damgeLimit);
	}
}

const DWORD CharacterStatRet = 0x004F28BB;
__declspec(naked) void CharacterStat() {
	__asm {
		test byte ptr[ebp + 0x0A], 0x40
		je label_damagelimit
		mov ecx, edi
		push[esi]
		call CharacterStatSkin
		label_damagelimit :
		test byte ptr[ebp + 0x0A], 0x80
			je label_ret
			mov ecx, edi
			push[esi]
			call CharacterStatDamageLimit
			label_ret :
		mov eax, [ebp + 0x08]
			pop edi
			pop esi
			pop ebx
			pop ebp
			jmp dword ptr[CharacterStatRet]
	}
}

void __stdcall _exitcleart() {
	CharacterEx::h_userSkin.clear();
	CharacterDataEx::GetInstance()->h_liLevel.clear();
	CharacterEx::updateDamgeLimit(Client::setAtkOutCap);
}

_declspec(naked) void exitCleart()
{
	_asm {
		lea eax, [esi + 358h]
		pushad
		pushfd
		call _exitcleart
		popfd
		popad
		push 0x00628318
		ret
	}
}

void __stdcall _changerMapCleart() {
	int userId = CharacterEx::getLoginUserId();
	if (userId > 0) {
		int skin = -1;
		if (CharacterEx::h_userSkin.find(userId) != CharacterEx::h_userSkin.end())
			skin = CharacterEx::h_userSkin[userId];
		int level = -1;
		if (CharacterDataEx::GetInstance()->h_liLevel.find(userId) != CharacterDataEx::GetInstance()->h_liLevel.end())
			level = CharacterDataEx::GetInstance()->h_liLevel[userId];
		CharacterEx::h_userSkin.clear();
		CharacterDataEx::GetInstance()->h_liLevel.clear();
		if (skin > 0) {
			CharacterEx::h_userSkin[userId] = skin;
		}
		if (level > 0) {
			CharacterDataEx::GetInstance()->h_liLevel[userId] = level;
		}
	}
	else {
		CharacterEx::h_userSkin.clear();
		CharacterDataEx::GetInstance()->h_liLevel.clear();
	}
}

const DWORD changerMapCleartOrginCall = 0x00666186;
_declspec(naked) void changerMapCleart()
{
	_asm {
		pushad
		pushfd
		call _changerMapCleart
		popfd
		popad
		call changerMapCleartOrginCall
		push 0x005348FF
		ret
	}
}

int hypontizeId = 0;
const DWORD hypontizeCInpacket__Decode4 = 0x004066FF;
const DWORD hypontize_INT128__operator_bool = 0x00884E00;
_declspec(naked) void hookHypontizePacket()
{
	_asm {
		mov hypontizeId, 0x0
		call hypontizeCInpacket__Decode4
		cmp eax, -1
		jnz label_ret
		call hypontizeCInpacket__Decode4
		mov hypontizeId, eax
		call hypontizeCInpacket__Decode4
		label_ret :
		ret
	}
}

_declspec(naked) void hookHypontizeSet()
{
	_asm {
		call hypontize_INT128__operator_bool
		test al, al
		push ebx
		jz label_ret
		mov ebx, [hypontizeId]
		cmp ebx, 0x0
		je label_ret
		mov ebx, 0x00BE4F14
		mov ebx, [ebx]
		cmp ebx, [hypontizeId]
		je label_ret
		mov eax, 0x0
		label_ret :
		pop ebx
			ret
	}
}

_declspec(naked) void hookHypontizeCancel()
{
	_asm {
		call hypontize_INT128__operator_bool
		test al, al
		push ebx
		jz label_ret
		mov ebx, 0x00BDD49C
		mov ebx, [ebx]
		cmp ebx, 0x0
		je label_ret
		add ebx, 0x68
		mov ebx, [ebx]
		cmp ebx, esi
		je label_ret
		mov eax, 0x0
		label_ret :
		pop ebx
			ret
	}
}

__declspec(naked) void fixMaxHP() {
	__asm {
		push 0x0A
		push eax
		push CharacterEx::m_maxHp
		push 0x008DF657
		ret
	}
}

__declspec(naked) void fixMaxMp() {
	__asm {
		push 0x0A
		push eax
		push CharacterEx::m_maxMp
		push 0x008DF982
		ret
	}
}

void CharacterEx::Init()
{
	Memory::SetHook(true, reinterpret_cast<void**>(&CUIStatusBar__SetNumberValue_t), CUIStatusBar__SetNumberValue_Hook);
	Memory::CodeCave(CharacterStat, 0x004F28B4, 7);
	Memory::CodeCave(fixMaxHP, 0x008DF651, 6);
	Memory::CodeCave(fixMaxMp, 0x008DF97C, 6);
	Memory::CodeCave(changerMapCleart, 0x005348FA, 5);
	Memory::CodeCave(exitCleart, 0x00628312, 6);
	CharacterEx::InitExpOverride();
	CharacterEx::InitLevelOverride();
	CharacterEx::InitDamageSkinOverride(Client::DamageSkin > 0 || Client::RemoteDamageSkin);
	CharacterEx::InitHypontizeFix(Client::s5221009);
	Memory::PatchCall(0x0097F8CD, OnUserLeave_DecodeID);
}

int CharacterEx::getLoginUserId()
{
	return *reinterpret_cast<int*>(0x00BE4F14);
}

void CharacterEx::InitExpOverride()
{
	//修改升级EXP
	if (!Client::levelExpOverride.empty()) {
		try {
			StringList res = splitstr(Client::levelExpOverride, ",");
			std::vector<LONGLONG> overrideExp;
			for (std::string& s : res) {
				long long num = std::stoll(s);
				overrideExp.push_back(num);
			}
			CharacterEx::SetOverrideExp(overrideExp);
			std::cout << "Enable longEXPOverride success, EXP size:" << overrideExp.size() << std::endl;
		}
		catch (const std::exception& e) {
			Client::levelExpOverride = "";
			std::cout << "Enable longEXPOverride failed: " << e.what() << ". restory to default" << std::endl;
		}
	}

	//if (!Client::longEXP && Client::levelExpOverride.empty())
	//	return;
	//Memory::SetHook(true, reinterpret_cast<void**>(&_lpfn_NextLevel), _lpfn_NextLevel_Hook);
	/* GW_CharacterStat::DecodeChangeStat -> hijack decode4 call and switch to decode8, then return int value */
	Memory::PatchCall(0x004F283C, ExpSwap__Decode4To8);

	/* GW_CharacterStat::Decode -> hijack decode4 call and switch to decode8, then return int value */
	Memory::PatchCall(0x004F2360, ExpSwap__Decode4To8);

	// GainExpShow
	Memory::PatchCall(0x00A2B20D, ExpSwap__Decode4To8msg);
	Memory::PatchCall(0x00A2B337, FormatExpMsgString_Hook);

	///* CWvsContext::OnStatChanged -> jmping over a segment that looks at exp and then makes pet talk if at a certain % -> cbf fixing this */
	Memory::WriteByte(0x00A297A3, 0xEB);

	///* CUIStat::OnMouseMove -> hijack displayed exp in tooltip when hovering in stat window */
	Memory::PatchCall(0x008CAA15, FormatExpString_Hook);

	///* CUIStat::Draw -> hijack displayed exp in stat window 0x008C602E*/
	Memory::PatchCall(0x008CB685, ZXString__GetConstCharString);

	///* CUIStatusBar::ProcessToolTip -> hijack displayed exp in tooltip when hovering exp gauge in stat bar */
	Memory::PatchCall(0x008DC3ED, FormatExpString_Hook);
	Memory::PatchCall(0x008DC3A9, FormatExpString_Hook);

	///* CUIStatusBar::SetNumberValue -> hijack displayed exp above exp gauge */
	Memory::WriteByte(0x008DEEFE + 1, 64); // increase string size allocation -- v207 = alloca(32)
	Memory::PatchCall(0x008DEF10, itoa_ExpSwap);
}

void CharacterEx::InitLevelOverride()
{
	/* GW_CharacterStat::DecodeChangeStat */
	Memory::CodeCave(decodeChangeStatStatDecode, 0x004F26F8, 5);

	/* GW_CharacterStat::Decode */
	Memory::CodeCave(characterLevelStatDecode, 0x004F2264, 5);
	//Memory::PatchCall(0x004F2264, LevelSwap__Decode);

	///* CUIStat::Draw */
	Memory::PatchCall(0x008CB2AF, ZXString_LevelString);

	Memory::WriteByte(0x00634565 + 1, 64);
	//Memory::PatchCall(0x00634587, _itoa_LevelSwap);
	Memory::CodeCave(itoa_LevelSwap, 0x00634587, 8);

	Memory::CodeCave(drawLevelString, 0x008DCE43, 5);

	//CWvsContext::OnCharacterInfo
	Memory::CodeCave(onCharacterInfoLevelStatDecode, 0x00A2CE3A, 5);   //0x0090740B pic init
	Memory::CodeCave(onCharacterInfoStrSwap, 0x00908929, 5);

	//CUserRemote::Init
	Memory::CodeCave(remoteInitLevelStatDecode, 0x0098CE1D, 5);

	//CWvsContext::OnPartyResult  create party
	Memory::CodeCave(onPartyCreateResultLevel, 0x00A485D6, 5);
}

void CharacterEx::InitDamageSkinOverride(BOOL bEnable)
{
	if (!bEnable)
		return;
	//Memory::PatchCall(0x0097FAD3, CUserPoolOnUserRemotePacket_DecodeID);
	Memory::CodeCave(GuildNameDecode, 0x0098CE5E, 5);
	Memory::CodeCave(GuildNameDecode2, 0x009912E7, 5);
	Memory::WriteByte(0x00437D44 + 1, 0x60);  // 0x39->0x60  57 -> 96  expand display range
	Memory::WriteByte(0x0043803A + 1, 0x60);
	Memory::WriteByte(0x006978DD + 2, 0xCA);  //0xF1
	Memory::WriteByte(0x006978E8 + 1, 0xCA);
	Memory::WriteByte(0x00437F04 + 1, 0x27);  //0x00
}

void CharacterEx::InitHypontizeFix(BOOL bEnable)
{
	if (!bEnable)
		return;
	Memory::PatchCall(0x007927DE, hookHypontizePacket);
	Memory::PatchCall(0x0069AA65, hookHypontizeSet);
	Memory::PatchCall(0x0069AF76, hookHypontizeCancel);
}

void CharacterEx::SetOverrideExp(std::vector<LONGLONG> exp)
{
	myArrayForCustomEXP.clear();
	myArrayForCustomEXP.insert(myArrayForCustomEXP.end(), exp.begin(), exp.end());
}
