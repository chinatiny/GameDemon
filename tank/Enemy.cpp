#include "Enemy.h"
#include "Weapon.h"
#include "CommTool.h"
#include "ColorDefine.h"
#include "Hero.h"
#include "AStar.h"


Enemy::Enemy(){}
Enemy::~Enemy(){}


void Enemy::InitSpirit(Scence *pScence)
{
	m_u64Tickets = 0;
	m_u64NextTicket = m_u64Tickets + c_nDisplayInterval;
	GenEnemyNode((MainScence*)pScence);
	//
	__super::InitSpirit(pScence);
}

void Enemy::BeforDestorySpirit()
{
	for (size_t i = 0; i < m_vecTanks.size(); i++)
	{
		delete  m_vecTanks[i];
	}
}

void Enemy::BeforEnterScence()
{
	for (size_t i = 0; i < m_vecTanks.size(); i++)
	{
		m_vecTanks[i]->ShowTank();
	}
}

bool Enemy::BeShot(Weapon::Bullet & nowBullet)
{
	for (size_t i = 0; i < m_vecTanks.size(); i++)
	{
		GamePos bulletPos = {nowBullet.nX, nowBullet.nY};
		//如果有一只坦克被击中会返回
		//nowBullet.eHero
		if (m_vecTanks[i]->BeShot(bulletPos, nowBullet.eCamp, nowBullet.eHero))
		{
  			return true;
		}
	}
	return false;
}

void Enemy::Display()
{
	for (size_t i = 0; i < m_vecTanks.size(); i++)
	{
		m_vecTanks[i]->Action();
	}
	//设置A*算法
	std::vector<int> vecWeight;
	for (size_t i = 0; i < m_vecTanks.size(); i++)
	{
		int nNice = m_vecTanks[i]->GetTankData().nAstarNiceValue > 0 ? m_vecTanks[i]->GetTankData().nAstarNiceValue : 1;
		vecWeight.push_back(nNice);
	}
	int nOutIndex = GetWeightOutput(vecWeight);
	if (nOutIndex >= 0) DoAStar(m_vecTanks[nOutIndex]);
	//
	++m_u64Tickets;
	if (m_u64Tickets < m_u64NextTicket) return;
	m_u64NextTicket = m_u64Tickets + c_nDisplayInterval;
	if (m_vecTanks.size() < c_nDefaultEnemyNum)  //地方坦克如果少于默认值
	{
		std::vector<Tank*> vecNew = GenEnemyNode((MainScence*)m_pScence);
		for (size_t i = 0; i < vecNew.size(); i++)
		{
			vecNew[i]->ShowTank();
		}
	}
	//

}

std::vector<Tank*> Enemy::GenEnemyNode(MainScence *pScence)
{
	//新增加的
	std::vector<Tank*> vecAdd;
	//空闲位置列表
	std::vector<GamePos> vecCanUsePos;
	((MainScence*)(pScence))->GetTankCanUseEmptyPos(vecCanUsePos);
	//如果可选择的位置大于0，随机一个
	if (vecCanUsePos.size() > 0)
	{
		int nNeedNum = c_nDefaultEnemyNum - m_vecTanks.size();
		if (nNeedNum > (int)vecCanUsePos.size())
		{
			nNeedNum = vecCanUsePos.size();
		}
		if (nNeedNum > 0)
		{
			std::vector<int> vecIndex;
			RandomFromList(vecIndex, nNeedNum, vecCanUsePos.size());
			std::vector<GamePos> vecHasUsePos;
			for (size_t i = 0; i < vecIndex.size(); i++)
			{
				int nCenterX = vecCanUsePos[vecIndex[i]].nX;
				int nCenterY = vecCanUsePos[vecIndex[i]].nY;
				EDir eDir = (EDir)(rand() % E_DIR_MAX);
				//
				GamePos centerPos = { nCenterX, nCenterY};
				std::vector<GamePos> bodayRealPos;
				Tank::BodayPos2RealPos(centerPos, eDir, bodayRealPos);
				//如果这个点之前有用过直接忽略
				bool bIsIgnore = false;
				for (size_t i = 0; i < vecHasUsePos.size(); i++)
				{
					for (size_t j = 0; j < bodayRealPos.size(); j++)
					{
						if (vecHasUsePos[i].nX == bodayRealPos[j].nX &&  vecHasUsePos[i].nY == bodayRealPos[j].nY)
						{
							bIsIgnore = true;
							goto __check_out;
						}
					}
				}
				__check_out:
				if (bIsIgnore) continue;
				//
				ERandomTanktype eTkType = (ERandomTanktype)(rand() % E_TANK_MAX);
				Tank *pTank = new Tank(
					(MainScence*)pScence,
					nCenterX, 
					nCenterY,
					E_TANK,
					eDir, 
					g_randomTank[eTkType].szPic, 
					g_randomTank[eTkType].nColor,
					E_ENEMY_CAMP, g_randomTank[eTkType].nHP, g_randomTank[eTkType].nSpeed);
				//当前坦克的坐标点
				for (size_t i = 0; i < bodayRealPos.size(); i++)
				{
					vecHasUsePos.push_back(bodayRealPos[i]);
				}
				m_vecTanks.push_back(pTank);
				vecAdd.push_back(pTank);
			}
		}
	}
	return vecAdd;
}

void Enemy::TankDead(Tank *pTank)
{
	for (auto ite = m_vecTanks.begin(); ite != m_vecTanks.end(); ite++)
	{
		if (*ite == pTank)
		{
			m_vecTanks.erase(ite);
			break;
		}
	}
}

void Enemy::DoAStar(Tank *&pTank)
{
	if (pTank->GetTankData().walkPath.vecWalkPath.empty())
	{
		GamePos startPos = { pTank->GetTankData().nCenterX, pTank->GetTankData().nCenterY };
		GamePos targetPos;
		//获取主角数据
		Hero *pHero = (Hero*)(m_pScence->FindMap("mainGameHero"));
		Hero::HeroData leftHero, rightHero;
		pHero->GetHeroLeftData(leftHero, rightHero);
		//
		if (nullptr != leftHero.pTank && nullptr != rightHero.pTank)
		{
			int  nRandomValue = (int)(rand() % 100) + 1;
			if (nRandomValue < 50)
			{
				targetPos.nX = leftHero.pTank->GetTankData().nCenterX;
				targetPos.nY = leftHero.pTank->GetTankData().nCenterY;
				unsigned int uID = leftHero.pTank->GetTankData().uID;
				DoAStar(uID, startPos, targetPos, pTank);
			}
			else
			{
				targetPos.nX = rightHero.pTank->GetTankData().nCenterX;
				targetPos.nY = rightHero.pTank->GetTankData().nCenterY;
				unsigned int uID = rightHero.pTank->GetTankData().uID;
				DoAStar(uID, startPos, targetPos, pTank);
			}
		}
		else if (nullptr != leftHero.pTank)
		{
			targetPos.nX = leftHero.pTank->GetTankData().nCenterX;
			targetPos.nY = leftHero.pTank->GetTankData().nCenterY;
			unsigned int uID = leftHero.pTank->GetTankData().uID;
			DoAStar(uID, startPos, targetPos, pTank);

		}
		else if (nullptr != rightHero.pTank)
		{
			targetPos.nX = rightHero.pTank->GetTankData().nCenterX;
			targetPos.nY = rightHero.pTank->GetTankData().nCenterY;
			unsigned int uID = rightHero.pTank->GetTankData().uID;
			DoAStar(uID, startPos, targetPos, pTank);
		}
	}
}


void Enemy::DoAStar(unsigned int uID, GamePos &startPos, GamePos &targetPos, Tank* pTank)
{
	AStar *pAstar = nullptr;
	pAstar = AStar::GetInstance(uID, startPos, targetPos, pTank);
	if (pAstar->FindPath())
	{
		std::vector<GamePos> vecPath;
		pAstar->GetPath(vecPath);
		pTank->SetAstarPath(vecPath);
		pTank->ResetAstarFailTimes();
	}
	else
	{
		pTank->ReduceAstarNiceValue();
	}
}

