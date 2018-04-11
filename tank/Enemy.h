#pragma once
#include "Spirit.h"
#include "Common.h"
#include "Map.h"
#include "Weapon.h"
#include "Tank.h"
#include "MainScence.h"
class Enemy :
	public Spirit
{
private:
	static const int c_nDisplayInterval = 10;
	static const int c_nDefaultEnemyNum = 7;
public:
	Enemy();
	~Enemy();
	virtual void InitSpirit(Scence *pScence);
	virtual void BeforDestorySpirit();
	virtual void BeforEnterScence();
	virtual void Display();
	bool BeShot(Weapon::Bullet & nowBullet);
	void TankDead(Tank *pTank);  //ÃπøÀÀ¿¡À
	void DoAStar(Tank *&pTank);  //A*
	void DoAStar(unsigned int uID, GamePos &starPos, GamePos &targetPos, Tank* pTank);
	std::vector<Tank*> GenEnemyNode(MainScence *pScence);
private:
	std::vector<Tank*> m_vecTanks;

	unsigned __int64 m_u64Tickets;
	unsigned __int64 m_u64NextTicket;
};

