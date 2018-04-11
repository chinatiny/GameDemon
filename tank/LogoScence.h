#pragma once
#include "Scence.h"
#include "Common.h"

class LogoScence :
	public Scence
{
private:
	typedef struct
	{
		int nX;
		int nY;
	}SelectPoint;
public:
	LogoScence();
	~LogoScence();
	virtual void InitScence();
	virtual void DestoryScence();
	virtual void BeforEnterScence();

	//接收键盘回调
	virtual void RcvKeyBoardNotify(char chKey);

private:
	void DrawSelect();
	void EraseOldSelct();

private:
	EHeroNum m_eHeroNum;
	SelectPoint m_selPoint[E_HERO_MOD_MAX];
	//提示字符串
	static const char* c_pszSingleGame;
	static const char* c_pszDoubleGame;
	static const char* c_pszArrow;
	static const char* c_pszEnterTip;
	static const char* c_pszEscTip;
	//各个提示字符串的位置
	static const int c_nSigleGameY = WINDOWS_HEIGHT / 5 * 2;
	static const int c_nSigleGameX = WINDOWS_WIDTH / 4 + 5;

	static const int c_nDoubleGameY = WINDOWS_HEIGHT / 5 * 2+ 1;
	static const int c_nDoubleGameX = WINDOWS_WIDTH / 4 + 5;

	static const int c_nEnterTipY = WINDOWS_HEIGHT - 3;
	static const int c_nEnterTipX = WINDOWS_WIDTH - 10;
	static const int c_nEscTipY = WINDOWS_HEIGHT - 2;
	static const int c_nEscTipX = WINDOWS_WIDTH -10;


};

