#include "GameOverScence.h"
#include "ConsoleUIBuff.h"
#include "KeyboardValue.h"
#include "Director.h"
#include "MenuScence.h"
#include "TankGame.h"
#include "ColorDefine.h"
#include "LogoScence.h"
#include "MainScence.h"
#include <iostream>
#include <fstream>


GameOverScence::GameOverScence(){}
GameOverScence::~GameOverScence(){}

void GameOverScence::InitScence()
{
	//向键盘注册通知
	RegKeyBoardNotify();
}

void GameOverScence::DestoryScence()
{
	ConsoleUIBuff::GetInstance()->CleanBuff();
	//让键盘不再通知
	UnRegKeyBoardNotify();
	//调用父类通用清理函数
	__super::DestoryScence();
}

void GameOverScence::BeforEnterScence()
{
	std::string strLogoFile = TankGame::GetGameInstance()->GetGameExeDir() + "scence/game_over.txt";
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
}

void GameOverScence::RcvKeyBoardNotify(char chKey)
{
	if (chKey == (char)E_KEY_ESC)  //输入enter键进入游戏主界面
	{
		//获取导演类
		Director *pDirector = Director::GetInstance();
		//进入游戏菜单
		MenuScence *pMenu = new MenuScence();
		pDirector->ChangeScence(pMenu);
	}
	else if (chKey == (char)E_KEY_ENTER)
	{
		//获取导演类
		Director *pDirector = Director::GetInstance();
		//进入游戏菜单
		MainScence *pMainScence = new MainScence();
		pDirector->ChangeScence(pMainScence);
	}
}
