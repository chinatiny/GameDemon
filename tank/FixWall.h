#pragma once

#include "Spirit.h"
#include "MainScence.h"
#include "Common.h"
#include "Tank.h"
#include "Weapon.h"

class FixWall :
	public Spirit
{
private:
	typedef struct  
	{
		int nX;
		int nY;
		ScenceActor actor;
	}FixNode;
public:
	FixWall();
	~FixWall();

	virtual void InitSpirit(Scence *pScence);
	virtual void BeforEnterScence();

	void TankStep(Tank::TankData &preTank, Tank::TankData &nowTank);  //坦克踏过，障碍物的处理 
	void TankDead(Tank::TankData &tankData); //坦克死亡了
	void WaterBeShot(Weapon::Bullet &bullet); //水被打了
	void GrassBeShot(Weapon::Bullet &bullet); //草被打了
	void SoilBeShot(Weapon::Bullet &bullet);  //土墙被打
	int GetComplex(); //获取复杂度
	//几个展示函数
	void ShowBorder();  //画边框
	void ShowWater();   //画水
	void ShowGrass();    //画草地
	void ShowSoil();      //画土墙
	void ShowIron();     //画铁块

	//判断给定的点是什么类型的固定障碍物(需要什么实现什么)
	bool IsWater(GamePos &gamePos);
	bool IsGrass(GamePos &gamePos);
	bool SetWater(GamePos &gamePos);
	bool SetGrass(GamePos &gamePos);



private:
	void ShowFixNode(FixNode &fixNode);
	void ShowFixWall(std::vector<FixNode> vecWall);
	void GetCoveringPos(std::vector<FixNode> &beCheckedPos, std::vector<GamePos> &checkGivePos, std::vector<GamePos> &getPos); //获取覆盖点列表
	bool IsCoveringPos(std::vector<FixNode> &beCheckedPos, GamePos &gamePos); //给定的点是否覆盖
	void ChangePosAttr(std::vector<FixNode> &beChangedPos, unsigned int uID, int nX, int nY, EWallType eWallType, char *pszPic, int nColor); //修改墙的图片和颜色
	void ChangeCoveringPosAttr(std::vector<FixNode> &fixPos, Tank::TankData &tankData, unsigned int uID, EWallType eWallType, char *pszPic, int nColor); //修改重合点属性
	void DeleteFixNode(std::vector<FixNode> &fixPos, int nX, int nY);



private:
	std::vector<FixNode> m_vecBorder;  //边界
	std::vector<FixNode> m_vecWater;   //水
	std::vector<FixNode> m_vecGrass;    //草地
	std::vector<FixNode> m_vecSoil;      //土墙
	std::vector<FixNode> m_vecIron;     //铁块
	int m_nComplex;  //复杂度
};

