#pragma once

class ConsoleUIBuff

{
private:
	ConsoleUIBuff(){}
public:
	const static int  c_nDefaultDrawIntervaltickets = 20;  //默认多少ticket进行一次绘制
	const static int  c_nDefaultCallBackIntervaltickets = 1; //定时回调多长时间调用一次

public:
	static ConsoleUIBuff* GetInstance();
	static void Destory();
	bool SetWindowBuff(const char* pszTitle, int nWidth, int nHeight, int nDrawIntervaltickets);
	void SetDrawIntervalTickets(int nDrawIntervaltickets);
	void DrawImme(int nX, int nY, char* pszText, unsigned short usColor);  //直接画，效率比较低
	void DrawDelay(int nX, int nY, char *pszText, unsigned short usColor);  //延时画，仅仅会重绘变动的部分
	void CleanBuff(); //清空缓冲区
	void CleanBuff(int nX, int nY, int nLen);
private:
	void SetWindowsBaseInfo(const char* pszTitle, int nWidth, int nHeight);
	void DoDrawDelay();
	void WriteChar(int nX, int nY, const char* pszBuff, unsigned short usColor);

public:
	static void UIDrawCallBack();
	static void SetCursorUnVisable();

private:
	typedef struct
	{
		unsigned short usColor;
		char *pszBuff;
	}UIBuffData, *PUIBuffData;
private:
	PUIBuffData m_pUIBuff;
	PUIBuffData m_pOldUIBuff;
	char *pszUIBuffBegin;
	char *pszOldUIBuffBegin;
	int m_nPerBuffLen; //这个通过动态计算出来
	int m_nWidth;
	int m_nHeight;
	int  m_nDrawIntervalTickets;
	unsigned __int64 m_u64NextDrawTicket;
	unsigned __int64 m_u64Tickets;
	static ConsoleUIBuff *s_pConsoleUIBuff;
};