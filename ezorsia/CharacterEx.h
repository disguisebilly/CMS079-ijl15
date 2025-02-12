#pragma once
class CharacterEx
{
public:
	static int m_maxHp;
	static int m_maxMp;
	static void Init();
	static int getLoginUserId();
	static tsl::robin_map<int, int> h_userSkin;
	static void updateDamgeLimit(int damgeLimit);
protected:
	static double m_atkOutCap;
	static void InitExpOverride();
	static void InitLevelOverride();
	static void InitDamageSkinOverride(BOOL bEnable);
	static void InitHypontizeFix(BOOL bEnable);
	static void SetOverrideExp(std::vector<LONGLONG> exp);
};

