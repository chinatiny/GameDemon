#include "Weapon.h"
#include "ConsoleUIBuff.h"
#include "Enemy.h"
#include "FixWall.h"
#include <assert.h>
#include "Tank.h"
#include "Hero.h"

Weapon::Weapon(){}
Weapon::~Weapon(){}

void Weapon::InitSpirit(Scence *pScence)
{
	m_u64Tickets = 0;
	m_u64NextTickets = m_u64Tickets + c_nWeaponSpeed;
	__super::InitSpirit(pScence);
}

void Weapon::Display()
{
	++m_u64Tickets;
	if (m_u64Tickets >= m_u64NextTickets)
	{
		DoBulletDisplay();
		m_u64NextTickets = m_u64Tickets + c_nWeaponSpeed;
	}
}

void Weapon::DoBulletDisplay()
{
	for (size_t i = 0; i < m_vecBullets.size(); i++)
	{
		MoveBullet(i, m_vecBullets[i]);
	}
	//删除需要被删除的子弹
	std::vector<Bullet> tmpVec;
	for (size_t i = 0; i < m_vecBullets.size(); i++)
	{
		bool bIsNeedDelete = IsNeedDelete(i);
		if (!bIsNeedDelete)
		{
			tmpVec.push_back(m_vecBullets[i]);
		}
	}
	m_vecNeedDelete.clear();
	m_vecBullets.clear();
	m_vecBullets = tmpVec;
}



void Weapon::MoveBullet(int nIndex, Bullet &bullet)
{
	//1.处理旧的子弹
	ScenceActor preTargetActor = ((MainScence*)m_pScence)->DirtyWallActor(bullet.nX, bullet.nY);
	if (E_WEAPON == preTargetActor.eWalltype)
	{
		FixWall *pFixWall = (FixWall*)(m_pScence->FindMap("mainGameFixWall"));
		GamePos gamePos = {bullet.nX, bullet.nY};
		if (pFixWall->IsWater(gamePos))
		{
			pFixWall->SetWater(gamePos);
		} 
		else if (pFixWall->IsGrass(gamePos))
		{
			pFixWall->SetGrass(gamePos);
		}
		else
		{
			EraseBullet(bullet);
		}
	}

	//2.判断下目标点是什么东西
	Bullet tmpBullet = bullet;
	ChangeByDir(tmpBullet);
	ScenceActor targetActor = ((MainScence*)m_pScence)->DirtyWallActor(tmpBullet.nX, tmpBullet.nY);
	//3. 各种情况分开处理
	if (E_EMPTY == targetActor.eWalltype)  //如果是空地
	{
		bullet = tmpBullet;
		ShowBullet(bullet);
	}
	else if (E_BORDER == targetActor.eWalltype || E_IRON == targetActor.eWalltype) //边框或者铁板
	{
		m_vecNeedDelete.push_back(nIndex);
	}
	else if (E_TANK == targetActor.eWalltype)
	{
		Hero *pHero = (Hero*)(m_pScence->FindMap("mainGameHero"));
		bool bHeroHandled = pHero->BeShot(tmpBullet);
		bool bEnemyHandled = false;
		if (!bHeroHandled)
		{
			Enemy *pEnemy = (Enemy*)(m_pScence->FindMap("mainGameEnemy"));
			bEnemyHandled = pEnemy->BeShot(tmpBullet);
		}
		//如果被处理了，直接销毁子弹
		if (bHeroHandled || bEnemyHandled)
		{
			m_vecNeedDelete.push_back(nIndex);
		}
		else
		{
			bullet = tmpBullet;
		}

	}
	else if (E_WATER == targetActor.eWalltype)
	{
		FixWall *pFixWall = (FixWall*)(m_pScence->FindMap("mainGameFixWall"));
		pFixWall->WaterBeShot(tmpBullet);
		bullet = tmpBullet;
	}
	else if (E_GRASS == targetActor.eWalltype)
	{
		FixWall *pFixWall = (FixWall*)(m_pScence->FindMap("mainGameFixWall"));
		pFixWall->GrassBeShot(tmpBullet);
		bullet = tmpBullet;
	}
	else if (E_SOIL == targetActor.eWalltype)
	{
		FixWall *pFixWall = (FixWall*)(m_pScence->FindMap("mainGameFixWall"));
		pFixWall->SoilBeShot(tmpBullet);
		m_vecNeedDelete.push_back(nIndex);
	}
	else if (E_WEAPON == targetActor.eWalltype)
	{
		//如果两个子弹相汇,
		bullet = tmpBullet;
		ShowBullet(bullet);
	}

}

void Weapon::ShowBullet(Bullet &bullet)
{
	((MainScence*)m_pScence)->ChangePosAttr(bullet.uID, bullet.eCamp, bullet.nX, bullet.nY, bullet.eWalltype, bullet.szPic, bullet.nColor);
}

void Weapon::EraseBullet(Bullet &bullet)
{
	((MainScence*)m_pScence)->ErasePos(bullet.nX, bullet.nY);
}

void Weapon::ChangeByDir(Bullet &bullet)
{
	switch (bullet.eEir)
	{
	case E_DIR_UP:
		--bullet.nY;
		break;
	case E_DIR_DOWN:
		++bullet.nY;
		break;
	case E_DIR_LEFT:
		--bullet.nX;
		break;
	case E_DIR_RIGHT:
		++bullet.nX;
		break;
	default:
		break;
	}
}

void Weapon::HeroFire(int nX, int nY, EDir eDir, int nColor, EHeroNum eNum)
{
	Dofire(m_vecBullets, E_HERO_CAMP, nX, nY, g_WallPic[E_WEAPON], eDir, nColor, eNum);
}

void Weapon::EnemyFire(int nX, int nY, char* szPic, EDir eDir, int nColor)
{
	Dofire(m_vecBullets, E_ENEMY_CAMP, nX, nY, szPic, eDir, nColor, E_HERO_MOD_MAX);
}

void Weapon::Dofire(std::vector<Bullet> vecBullets, EFightCamp eCamp, int nX, int nY, char* szPic, EDir eDir, int nColor, EHeroNum eNum)
{
	Bullet bullet;
	bullet.uID = MainScence::GetActorID(E_WEAPON);
	bullet.nX = nX;
	bullet.nY = nY;
	bullet.eEir = eDir;
	bullet.eWalltype = E_WEAPON;
	bullet.eCamp = eCamp;
	strcpy_s(bullet.szPic, sizeof(bullet.szPic) - 1, szPic);
	bullet.nColor = nColor;
	bullet.eHero = eNum;
	//
	ScenceActor targetActor = ((MainScence*)m_pScence)->DirtyWallActor(bullet.nX, bullet.nY);
	//
	if (E_IRON == targetActor.eWalltype || E_BORDER == targetActor.eWalltype || E_WEAPON  == targetActor.eWalltype) 
	{
		//铁板边框和子弹
		return;
	}
	else if (E_EMPTY == targetActor.eWalltype)
	{
		m_vecBullets.push_back(bullet);
		ShowBullet(bullet);
	}
	else if (E_WATER == targetActor.eWalltype)
	{
		FixWall *pFixWall = (FixWall*)(m_pScence->FindMap("mainGameFixWall"));
		pFixWall->WaterBeShot(bullet);
		m_vecBullets.push_back(bullet);
	}
	else if (E_GRASS == targetActor.eWalltype)
	{
		FixWall *pFixWall = (FixWall*)(m_pScence->FindMap("mainGameFixWall"));
		pFixWall->GrassBeShot(bullet);
		m_vecBullets.push_back(bullet);
	}
	else if (E_SOIL == targetActor.eWalltype)
	{
		FixWall *pFixWall = (FixWall*)(m_pScence->FindMap("mainGameFixWall"));
		pFixWall->SoilBeShot(bullet);
	}
	else if (E_TANK == targetActor.eWalltype)  //同一个阵营的直接忽略
	{
		//不是一个阵营的开火
		if (eCamp != targetActor.eCamp)
		{
 			Hero *pHero = (Hero*)(m_pScence->FindMap("mainGameHero"));
			bool bHeroHandled = pHero->BeShot(bullet);
			bool bEnemyHandled = false;
			if (!bHeroHandled)
			{
				Enemy *pEnemy = (Enemy*)(m_pScence->FindMap("mainGameEnemy"));
				bEnemyHandled = pEnemy->BeShot(bullet);
			}
		}
	}

}

bool Weapon::IsNeedDelete(int nIndex)
{
	for (size_t i = 0; i < m_vecNeedDelete.size(); i++)
	{
		if (nIndex == m_vecNeedDelete[i])
		{
			return true;
		}
	}
	return false;
}
