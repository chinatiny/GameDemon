#pragma once
#include "GameApp.h"
#include "Common.h"
class TankGame:public GameApp
{
private:
	TankGame();
public:
	static TankGame* GetGameInstance();
	static void DestoryGameInstance();

	virtual void DoBeforGameStart(); //游戏开始之前做的初始化工作
	virtual void StartGame(); //开始游戏
	virtual void OnGameStop();  //在游戏结束时候做的善后工作
	void SetHeroNum(EHeroNum eHeroNum);
	EHeroNum GetHeroNum();

private:
	static TankGame* s_pTankGame;
	EHeroNum m_heroNum; //双人模式还是单人模式

};

