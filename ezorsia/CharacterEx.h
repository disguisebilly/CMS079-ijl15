#pragma once
class CharacterEx
{
public:
	static int m_loginUserId;
	static int m_maxHp;
	static int m_maxMp;
	static tsl::robin_map<int, int> h_userSkin;
	static void InitExpOverride(BOOL bEnable);
	static void InitLevelOverride(BOOL bEnable);
	static void InitDamageSkinOverride(BOOL bEnable);
	static void InitHypontizeFix(BOOL bEnable);
};

