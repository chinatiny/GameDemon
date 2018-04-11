#pragma once
#include "Spirit.h"
#include "MainScence.h"
#include "Tank.h"
#include "Weapon.h"

class Hero :
	public Spirit
{
public:
	//默认的两个玩家坐标
	const static int c_nDefaultLeftX = 15;
	const static int c_nDefaultLeftY = GAME_AREA_HEIGHT - 3;
	const static int c_nDefaultRightX = GAME_AREA_WIDTH - 15;
	const static int c_nDefaultRightY = GAME_AREA_HEIGHT - 3;
	//主角的默认血值
	const static int c_nDefaultHp = 100;
	//玩家数据
	typedef struct 
	{
		Tank *pTank;
		int nScore;
	}HeroData;

public:
	Hero();
	~Hero();

	virtual void InitSpirit(Scence *pScence);
	virtual void BeforDestorySpirit();
	virtual void BeforEnterScence();
	virtual void RcvKeyBoardNotify(char chKey);
	void SetGameMod(EHeroNum eMod);

	bool BeShot(Weapon::Bullet & bullet); //被打
	void TankDead(Tank *pTank); //坦克死了
	void AddScore(EHeroNum eHero, Tank::TankData &tankData); //加分
	void AddScore(HeroData &heroData, Tank::TankData &tankData);//加分
	void GetHeroLeftData(HeroData &leftHero, HeroData &rightHero);  //获取主角信息

private:
	void Fire(HeroData &heroData, EHeroNum eHero); //开火
	void MoveUp(HeroData &heroData);
	void MoveDown(HeroData &heroData);
	void MoveLeft(HeroData &heroData);
	void MoveRight(HeroData &heroData);
	void ShowScore();
	void MissionComplete(); //通关
private:
	HeroData m_leftHero;  //第一个玩家
	HeroData m_rightHero;  //第二个玩家
	EHeroNum m_eMod; //游戏模式
};

