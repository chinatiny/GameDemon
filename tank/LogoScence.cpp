#include "LogoScence.h"
#include "KeyboardValue.h"
#include "Director.h"
#include "ConsoleUIBuff.h"
#include "ColorDefine.h"
#include <string.h>
#include "TankGame.h"
#include "MainScence.h"
#include "MenuScence.h"
#include <iostream>
#include <fstream>
#include "TankGame.h"

const char* LogoScence::c_pszSingleGame = "单人游戏";
const char* LogoScence::c_pszDoubleGame = "双人游戏";
const char* LogoScence::c_pszArrow = "◆";
const char* LogoScence::c_pszEscTip = "按ESC键进入菜单界面";
const char* LogoScence::c_pszEnterTip = "按Enter进入游戏";




LogoScence::LogoScence(){}
LogoScence::~LogoScence(){}

void LogoScence::InitScence()
{
	//
	m_eHeroNum = E_LEFT_HERO;
	m_selPoint[E_LEFT_HERO] = { c_nSigleGameX - 2, c_nSigleGameY };
	m_selPoint[E_RIGHT_HERO] = { c_nDoubleGameX - 2, c_nDoubleGameY };
	TankGame::GetGameInstance()->SetHeroNum(m_eHeroNum);

	//向键盘注册通知
	RegKeyBoardNotify();
}

void LogoScence::DestoryScence()
{
	ConsoleUIBuff::GetInstance()->CleanBuff();
	//让键盘不再通知
	UnRegKeyBoardNotify();
	//调用父类通用清理函数
	__super::DestoryScence();
}

void LogoScence::RcvKeyBoardNotify(char chKey)
{
	if (chKey == (char)E_KEY_ENTER)  //输入enter键进入游戏主界面
	{
		//获取导演类
		Director *pDirector = Director::GetInstance();
		//开启游戏主界面
		MainScence *pMainGame = new MainScence();
		pDirector->ChangeScence(pMainGame);
	}
	else if (chKey == (char)E_KEY_ESC)
	{
		//获取导演类
		Director *pDirector = Director::GetInstance();
		//开启游戏主菜单
		MenuScence *pMainMenu = new MenuScence();
		pDirector->ChangeScence(pMainMenu);
	}
	else if (chKey == (char)E_KEY_UP || chKey == (char)E_KEY_W || chKey == (char)E_KEY_w)
	{
		EraseOldSelct();
		int nIindex = (int)m_eHeroNum;
		nIindex = (nIindex - 1 + E_HERO_MOD_MAX) % E_HERO_MOD_MAX;
		m_eHeroNum = (EHeroNum)nIindex;
		TankGame::GetGameInstance()->SetHeroNum(m_eHeroNum);
		DrawSelect();

	}
	else if (chKey == (char)E_KEY_DOWN || chKey == (char)E_KEY_s || chKey == (char)E_KEY_S)
	{
		EraseOldSelct();
		int nIindex = (int)m_eHeroNum;
		nIindex = (nIindex + 1) % E_HERO_MOD_MAX;
		m_eHeroNum = (EHeroNum)nIindex;
		TankGame::GetGameInstance()->SetHeroNum(m_eHeroNum);
		DrawSelect();

	}
}

void LogoScence::BeforEnterScence()
{
	std::string strLogoFile = TankGame::GetGameInstance()->GetGameExeDir() + "scence/logo.txt";
	std::ifstream readFile(strLogoFile.c_str(), std::ios::in);
	if (readFile.is_open())
	{
		int nBaseY = 4;
		int nBaseX = 8;
		while (!readFile.eof())  //判断是否达到stream的结尾
		{
			char szReadIn[1024] = { 0 };
			readFile.getline(szReadIn, sizeof(szReadIn), '\n');
			ConsoleUIBuff::GetInstance()->DrawImme(nBaseX, nBaseY, szReadIn, F_H_YELLOW);
			++nBaseY;
		}
		readFile.close();
	}
	ConsoleUIBuff::GetInstance()->DrawImme(c_nSigleGameX, c_nSigleGameY, (char*)c_pszSingleGame, F_H_YELLOW);
	ConsoleUIBuff::GetInstance()->DrawImme(c_nDoubleGameX, c_nDoubleGameY, (char*)c_pszDoubleGame, F_H_YELLOW);
	ConsoleUIBuff::GetInstance()->DrawImme(m_selPoint[m_eHeroNum].nX, m_selPoint[m_eHeroNum].nY, (char*)c_pszArrow, F_H_YELLOW);

	ConsoleUIBuff::GetInstance()->DrawImme(c_nEnterTipX, c_nEnterTipY, (char*)c_pszEnterTip, F_H_RED);
	ConsoleUIBuff::GetInstance()->DrawImme(c_nEscTipX, c_nEscTipY, (char*)c_pszEscTip, F_H_RED);
}


void LogoScence::DrawSelect()
{
	ConsoleUIBuff::GetInstance()->DrawImme(m_selPoint[m_eHeroNum].nX, m_selPoint[m_eHeroNum].nY, (char*)c_pszArrow, F_H_YELLOW);
}

void LogoScence::EraseOldSelct()
{
	ConsoleUIBuff::GetInstance()->DrawImme(m_selPoint[m_eHeroNum].nX, m_selPoint[m_eHeroNum].nY, "  ", 0);
}
