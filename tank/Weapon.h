#pragma once
#include "Spirit.h"
#include "MainScence.h"
#include "Common.h"
#include "Map.h"
#include <vector>

class Tank;

class Weapon :
	public Spirit
{
private:
	static const int c_nWeaponSpeed = 2;
public:
	typedef struct
	{
		unsigned int uID;
		int nX;
		int nY;
		EDir eEir;
		EWallType eWalltype;
		EFightCamp eCamp;
		char szPic[PIC_LEN];
		int nColor;
		EHeroNum eHero;
	}Bullet;

public:
	Weapon();
	~Weapon();

	virtual void InitSpirit(Scence *pScence);
	virtual void Display();
	void DoBulletDisplay();
	void HeroFire(int nX, int nY, EDir eDir, int nColor, EHeroNum eNum); 	//主角开火了
	void EnemyFire(int nX, int  nY, char *szPic, EDir eDir, int nColor); //敌人开火了
private:
	void MoveBullet(int nIndex, Bullet &bullet);  //移动子弹
	void ShowBullet(Bullet &bullet);  //在场景中显示子弹
	void EraseBullet(Bullet &bullet);  //在场景中删除子弹
	void ChangeByDir(Bullet &bullet); //根据方向进行变动
	void Dofire(std::vector<Bullet> vecBullets, EFightCamp eCamp, int nX, int  nY, char *szPic, EDir eDir, int nColor, EHeroNum eNum);  //专用开火函数
	bool IsNeedDelete(int nIndex); //判断子弹是否需要被删除

private:
	std::vector<Bullet> m_vecBullets;
	std::vector<int> m_vecNeedDelete;
	__int64 m_u64Tickets;
	__int64 m_u64NextTickets;
};

