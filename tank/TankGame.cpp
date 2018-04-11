#include "TankGame.h"
#include "Director.h"
#include "TimeWheel.h"
#include "ConsoleUIBuff.h"
#include "Common.h"
#include "LogoScence.h"
#include <time.h>
#include <windows.h>
#pragma  comment(lib, "winmm.lib")


TankGame * TankGame::s_pTankGame = nullptr;

TankGame::TankGame()
{
	m_heroNum = E_LEFT_HERO;
}

TankGame* TankGame::GetGameInstance()
{
	if (nullptr == s_pTankGame)
	{
		s_pTankGame = new TankGame;
	}
	return s_pTankGame;
}

void TankGame::DestoryGameInstance()
{
	if (nullptr != s_pTankGame)
	{
		delete s_pTankGame;
		s_pTankGame = nullptr;
	}
}

void TankGame::DoBeforGameStart()
{
	//随机数种子
	srand((unsigned int)time(0));

	//初始化UI缓冲区(没有集成到GameApp中是因为UI这个东西现在是控制台的以后可能是windows界面的)
	ConsoleUIBuff *pUIBuff =  ConsoleUIBuff::GetInstance();
	pUIBuff->SetWindowBuff(GAME_NAME, WINDOWS_WIDTH, WINDOWS_HEIGHT, ConsoleUIBuff::c_nDefaultDrawIntervaltickets);
	//把UIbuff向时间轮注册
	TimeWheel::RegRunFunc(ConsoleUIBuff::UIDrawCallBack, ConsoleUIBuff::c_nDefaultCallBackIntervaltickets);
	//播放音乐

	std::string strFullPath = TankGame::GetGameInstance()->GetGameExeDir() + "sound/MainSound.wav";
	//播放音乐
	PlaySoundA(strFullPath.c_str(), NULL, SND_ASYNC | SND_NODEFAULT | SND_LOOP);
}

void TankGame::StartGame()
{
	//获取导演类
	Director *pDirector = Director::GetInstance();
	//第一个场景
	LogoScence *pLogoScence = new LogoScence();
	//导演让整个舞台开始
	pDirector->FistRunScence(pLogoScence);
	//开始游戏循环
	GameApp::GameLoop(pDirector);
}

void TankGame::OnGameStop()
{
	//当游戏结束时候清理UI缓冲区
	ConsoleUIBuff::Destory();
	//清理掉导演类占用的资源
	Director::Destory();
}

void TankGame::SetHeroNum(EHeroNum eHeroNum)
{
	m_heroNum = eHeroNum;
}

EHeroNum TankGame::GetHeroNum()
{
	return m_heroNum;
}

