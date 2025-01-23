#pragma once
class CharacterEx
{
public:
	static int m_maxHp;
	static int m_maxMp;
	static tsl::robin_map<int, int> h_userSkin;
	static void Init();
	static int getLoginUserId();
protected:
	static void InitExpOverride();
	static void InitLevelOverride();
	static void InitDamageSkinOverride(BOOL bEnable);
	static void InitHypontizeFix(BOOL bEnable);
	static void SetOverrideExp(std::vector<LONGLONG> exp);
};

