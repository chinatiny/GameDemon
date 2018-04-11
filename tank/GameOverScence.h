#pragma once
#include "Scence.h"
class GameOverScence :
	public Scence
{
public:
	GameOverScence();
	~GameOverScence();

	virtual void InitScence();
	virtual void DestoryScence();
	virtual void BeforEnterScence();

	//接收键盘回调
	virtual void RcvKeyBoardNotify(char chKey);
};

