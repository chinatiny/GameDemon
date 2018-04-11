#pragma once
#include "Scence.h"
#include "Map.h"
#include "Common.h"


class MainScence :
	public Scence
{
public:
	MainScence();
	~MainScence();

	virtual void InitScence();
	virtual void BeforEnterScence();
	virtual void DestoryScence();
	virtual void ShowScence();  //把当前场景在界面上展示出来
	virtual void RcvKeyBoardNotify(char chKey); 	//接受键盘回调
	void ChangePosAttr(unsigned int uID, EFightCamp eCamp, int nX, int nY, EWallType eWalltype, const char* szPic, int nColor); //修改点信息
	void ErasePos(int nX, int nY); //清空掉点
	ScenceActor DirtyWallActor(int nX, int nY); //脏读取
	static unsigned int GetActorID(EWallType eWallType); //根据墙体类型获取ID
	void DirtyGetEmptyPos(std::vector<GamePos> &vecEmptyPos); //获取空闲点
	void GetTankCanUseEmptyPos(std::vector<GamePos> &vecCanUsePos);  //获取可以放置坦克的点
private:
	ScenceActor  m_scenceArryPos[GAME_AREA_HEIGHT][GAME_AREA_WIDTH];
	//场景的id
	static unsigned int s_nLastID;
};

