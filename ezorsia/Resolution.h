#pragma once
class Resolution
{
public:
	static int D3Dptr;
	static int m_nGameHeight;
	static int m_nGameWidth;
	static void Init();
	static void UpdateResolution(unsigned int nScreenWidth, unsigned int nScreenHeight);
	static void updateStatResolution(unsigned int backgrndWidth, unsigned int  backgrnd2Width);
	static void UpdateSlotPosition(int width);
	static void UpdateBarWidth(int width);
};

static inline IWzGr2DPtr& getIWzGr2DPtr() {
	return *reinterpret_cast<IWzGr2DPtr*>(0x00BE2788);
}