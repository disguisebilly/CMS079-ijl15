#pragma once
class CAutoDump
{
public:
	CAutoDump(void);
	~CAutoDump(void);
private:
	void* m_lpUnhandledExceptionFilter;
};

