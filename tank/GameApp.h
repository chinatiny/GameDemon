#pragma once
#pragma  once
#include <string>
#include "Director.h"
#include "Scence.h"

class GameApp
{
public:
	GameApp();
	~GameApp(){};

public:
	virtual void DoBeforGameStart() = 0; //游戏开始之前做的初始化工作
	virtual void StartGame() = 0; //开始游戏

	virtual void OnGameStop() = 0;  //结束游戏时候做的善后工作

	std::string GetGameExeDir();  //获取exe执行的目录

	void StopGame(); //调用后结束游戏，从StartGame阻塞中解除
	void GameLoop(Director *pDirector);
	void GamePause(); //暂停游戏
	void GameResume(); //游戏继续执行


private:
	std::string m_strGameDir; //游戏exe所在的目录
	bool m_bPause;
	Director *m_pDirector;
};

