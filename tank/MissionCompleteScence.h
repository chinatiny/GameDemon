#pragma once
#include "Scence.h"
class MissionCompleteScence :
	public Scence
{
public:
	MissionCompleteScence();
	~MissionCompleteScence();

	virtual void InitScence();
	virtual void DestoryScence();
	virtual void BeforEnterScence();

	//接收键盘回调
	virtual void RcvKeyBoardNotify(char chKey);
};

