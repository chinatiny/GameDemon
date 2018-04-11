#pragma once
#include "Scence.h"
#include "Common.h"

class MenuScence :
	public Scence
{
	typedef enum
	{
		E_MENU_START_GAME,
		E_MENU_EDIT_FIX_WALL,
		E_MENU_EXIT,
		E_MENU_SELECT_EDIT_MAP,
		E_GAME_EXIT,
		E_MENU_MAX
	}MenuSelect;
public:
	MenuScence();
	~MenuScence();

	virtual void InitScence();
	virtual void DestoryScence();
	virtual void BeforEnterScence();

	//接手键盘回调
	virtual void RcvKeyBoardNotify(char chKey);

private:
	typedef struct
	{
		int nX;
		int nY;
	}SelectPoint;
private:
	void DrawSelect();
	void EraseOldSelct();
private:
	//一共有几个选项
	static const int  c_nSelectNum = E_MENU_MAX;
	MenuSelect m_eSelect;
	SelectPoint m_selPoint[c_nSelectNum];


	//提示字符串
	static const char* c_pszStartGame;
	static const char* c_pszEditFixWall;
	static const char* c_pszExitMenu;
	static const char* c_pszArrow;
	static const char* c_pszExitGame;
	static const char* c_pszSelectMap;
	static const char* c_pszEnterTip;
	static const char* c_pszEscTip;
	//各个提示字符串的位置
	static const int c_nReStartY = WINDOWS_HEIGHT / 3;
	static const int c_nReStartX = WINDOWS_WIDTH / 3 + 5;

	static const int c_nEditWallY = WINDOWS_HEIGHT / 3 + 2;
	static const int c_nEditWallX = WINDOWS_WIDTH / 3 + 5;

	static const int c_nExitMenuY = WINDOWS_HEIGHT / 3 + 4;
	static const int c_nExitMenuX = WINDOWS_WIDTH / 3 + 5;

	static const int c_nSelectMapY = WINDOWS_HEIGHT / 3 + 6;
	static const int c_nSelectMapX = WINDOWS_WIDTH / 3 + 5;

	static const int c_nExitGameY = WINDOWS_HEIGHT / 3 + 8;
	static const int c_nExitGameX = WINDOWS_WIDTH / 3 + 5;

	static const int c_nEnterTipY = WINDOWS_HEIGHT - 3;
	static const int c_nEnterTipX = WINDOWS_WIDTH - 10;
	static const int c_nEscTipY = WINDOWS_HEIGHT - 2;
	static const int c_nEscTipX = WINDOWS_WIDTH - 10;

};

