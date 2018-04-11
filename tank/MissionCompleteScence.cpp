#include "MissionCompleteScence.h"
#include "ConsoleUIBuff.h"
#include "TankGame.h"
#include "KeyboardValue.h"
#include "MainScence.h"
#include "ColorDefine.h"
#include "SelectMapScence.h"
#include <iostream>
#include <fstream>


MissionCompleteScence::MissionCompleteScence(){}
MissionCompleteScence::~MissionCompleteScence(){}


void MissionCompleteScence::InitScence()
{
	//向键盘注册通知
	RegKeyBoardNotify();
}


void MissionCompleteScence::DestoryScence()
{
	ConsoleUIBuff::GetInstance()->CleanBuff();
	//让键盘不再通知
	UnRegKeyBoardNotify();
	//调用父类通用清理函数
	__super::DestoryScence();
}

void MissionCompleteScence::BeforEnterScence()
{
  	std::string strLogoFile = TankGame::GetGameInstance()->GetGameExeDir() + "scence/mission_complete.txt";
	std::ifstream readFile(strLogoFile.c_str(), std::ios::in);
	if (readFile.is_open())
	{
		int nBaseY = 4;
		int nBaseX = 2;
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

void MissionCompleteScence::RcvKeyBoardNotify(char chKey)
{
	if (chKey == (char)E_KEY_ENTER)
	{
		SelectMapScence::s_strMapName = "AStarTest.map";
		//获取导演类
		Director *pDirector = Director::GetInstance();
		//进入游戏菜单
		MainScence *pMainScence = new MainScence();
		pDirector->ChangeScence(pMainScence);
	}
}


