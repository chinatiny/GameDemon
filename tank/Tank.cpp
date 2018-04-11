#include "Tank.h"
#include "Map.h"
#include "FixWall.h"
#include "Hero.h"
#include "Enemy.h"
#include "ColorDefine.h"
#include <assert.h>

//坦克在各种方向上的形状
const int Tank::c_nArryTankShape[E_DIR_MAX][c_nTankHeight][c_nTankWidth] =
{
	{
		0, 1, 0,
		1, 1, 1,
		1, 0, 1
	},
	{
		1, 0, 1,
		1, 1, 1,
		0, 1, 0
	},
	{
		0, 1, 1,
		1, 1, 0,
		0, 1, 1
	},
	{
		1, 1, 0,
		0, 1, 1,
		1, 1, 0
	}
};

Tank::Tank(MainScence *pScence, int nCenterX, int nCenterY, EWallType eWallType, EDir eDir, const char *pszPic, int nColor, EFightCamp eCamp, int nHp, int nSpeed)
{
	m_u64Tickets = 0;
	m_u64NextTicket = m_u64Tickets + nSpeed;
	//
	m_pMainScence = pScence;
	//
	m_tankData.uID = MainScence::GetActorID(eWallType);
	m_tankData.nCenterX = nCenterX;
	m_tankData.nCenterY = nCenterY;
	m_tankData.eWalltype = eWallType;
	m_tankData.eDir = eDir;
	strcpy_s(m_tankData.szPic, sizeof(m_tankData.szPic), pszPic);
	m_tankData.nColor = nColor;
	m_tankData.eCamp = eCamp;
	m_tankData.nHP = nHp;
	m_tankData.nSpeed = nSpeed;
	m_tankData.walkPath.nStep = 0;
	m_tankData.walkPath.nWaitTimes = RUN_PATH_WAIT_TIMES;
	m_tankData.walkPath.vecWalkPath.clear();
	m_tankData.nAstarNiceValue = GAME_AREA_HEIGHT * GAME_AREA_WIDTH;
	
}
Tank::~Tank(){}

void Tank::BodayPos2RealPos(TankData &tankData, std::vector<GamePos> &vecPos)
{
	for (int i = 0; i < c_nTankHeight; i++)
	{
		for (int j = 0; j < c_nTankWidth; j++)
		{
			if (1 == c_nArryTankShape[tankData.eDir][i][j])
			{
				GamePos bodayPos = { j, i };
				GamePos realPos;
				GamePos centerPos = { tankData.nCenterX, tankData.nCenterY};
				BodayPos2RealPos(centerPos, bodayPos, realPos);
				vecPos.push_back(realPos);
			}
		}
	}
}

void Tank::BodayPos2RealPos(GamePos &centerPos, GamePos bodayPos, GamePos &realPos)
{
	int nOffsetX = bodayPos.nX - 1;
	int nOffsetY = bodayPos.nY - 1;
	realPos.nX = centerPos.nX + nOffsetX;
	realPos.nY = centerPos.nY + nOffsetY;
}

void Tank::BodayPos2RealPos(GamePos &centerPos, EDir eDir, std::vector<GamePos> &vecPos)
{
	int nArryBody[c_nTankHeight][c_nTankWidth] = { 0 };
	memcpy(nArryBody, c_nArryTankShape[eDir], sizeof(nArryBody));
	//
	for (int i = 0; i < c_nTankHeight; i++)
	{
		for (int j = 0; j < c_nTankWidth; j++)
		{
			if (1 == nArryBody[i][j])
			{
				GamePos bodayPos = { j, i };
				GamePos realPos;
				BodayPos2RealPos(centerPos, bodayPos, realPos);
				vecPos.push_back(realPos);
			}
		}
	}
}

void Tank::ShowTank()
{
	std::vector<GamePos> vecPos;
	BodayPos2RealPos(m_tankData, vecPos);
	for (size_t i = 0; i < vecPos.size(); i++)
	{
		m_pMainScence->ChangePosAttr(m_tankData.uID, m_tankData.eCamp, vecPos[i].nX, vecPos[i].nY, E_TANK, m_tankData.szPic, m_tankData.nColor);
	}
}

void Tank::EraseTank()
{
	std::vector<GamePos> vecPos;
	BodayPos2RealPos(m_tankData, vecPos);
	for (size_t i = 0; i < vecPos.size(); i++)
	{
		m_pMainScence->ErasePos(vecPos[i].nX, vecPos[i].nY);
	}
}

void Tank::MoveDir(EDir eDir)
{
	TankData preData = m_tankData;
	TankData tmpData = m_tankData;
	if (m_tankData.eDir == eDir) 	//方向相同移动距离
	{
		UpdateTankDataByDir(tmpData);
		//如果移动的位置不能移动直接返回
		if (!IsTankCanStep(tmpData)) return;
	}
	else //方向不同移动方向（如果卡死调整下身体）
	{
		//调整方向
		tmpData.eDir = eDir;
		//判断调整方向是否卡在墙里面,卡死调整下
		if (!IsTankCanStep(tmpData))
		{
			UpdateTankDataByDir(tmpData);
		}
		//如果还是不行就卡死了
		if (!IsTankCanStep(tmpData)) return;
	}
	//
	EraseTank();
	m_tankData = tmpData;
	ShowTank();

	//通知障碍物我来了
	FixWall *pFixWall = (FixWall*)(m_pMainScence->FindMap("mainGameFixWall"));
	pFixWall->TankStep(preData, tmpData);
}

bool Tank::IsTankCanStep(TankData &tankData)
{
	return IsTankCanStep(tankData, E_ID_INVALID);
}
bool Tank::IsTankCanStep(TankData &tankData, unsigned int uIgnoreID)
{
	//边界判断
	if (!(tankData.nCenterX >= 2 && tankData.nCenterX <= GAME_AREA_WIDTH - 3 &&
		tankData.nCenterY >= 2 && tankData.nCenterY <= GAME_AREA_HEIGHT - 3))
		return false;
	//判断坦克周边的坐标
	for (int i = 0; i < c_nTankHeight; i++)
	{
		for (int j = 0; j < c_nTankWidth; j++)
		{
			if (1 == c_nArryTankShape[tankData.eDir][i][j])
			{
				GamePos bodayPos = { j, i };
				GamePos realPos;
				GamePos centerPos = { tankData.nCenterX, tankData.nCenterY };
				BodayPos2RealPos(centerPos, bodayPos, realPos);
				ScenceActor scenceActor = m_pMainScence->DirtyWallActor(realPos.nX, realPos.nY);
				if (E_EMPTY != scenceActor.eWalltype) //不为空且不是自己身体
				{
					//不是草地或者自己或者指定忽略
					if (!(E_GRASS == scenceActor.eWalltype || scenceActor.uID == tankData.uID || scenceActor.uID == uIgnoreID))
					{
						return false;
					}
				}
			}
		}
	}
	return true;
}

void Tank::UpdateTankDataByDir(TankData &tankData)
{
	switch (tankData.eDir)
	{
	case E_DIR_UP:
		--tankData.nCenterY;
		break;
	case E_DIR_DOWN:
		++tankData.nCenterY;
		break;
	case E_DIR_LEFT:
		--tankData.nCenterX;
		break;
	case E_DIR_RIGHT:
		++tankData.nCenterX;
		break;
	default:
		break;
	}
}

Tank::TankData Tank::GetTankData()
{
	return m_tankData;
}

bool Tank::BeShot(GamePos &gamePos, EFightCamp eBulletCamp, EHeroNum eFromHero)
 {
	if (IsBeShot(gamePos, eBulletCamp))
	{
		m_tankData.nHP -= 1;
		if (m_tankData.nHP <= 0)
		{
			EraseTank();
			FixWall *pFixWall = (FixWall*)(m_pMainScence->FindMap("mainGameFixWall"));
			pFixWall->TankDead(m_tankData);
			//根据阵营选择对应的处理
			Hero *pHero = (Hero*)(m_pMainScence->FindMap("mainGameHero"));
			Enemy *pEnemy = (Enemy*)(m_pMainScence->FindMap("mainGameEnemy"));
			if (E_HERO_CAMP == m_tankData.eCamp)
			{
				pHero->TankDead(this);
			}
			else if (E_ENEMY_CAMP == m_tankData.eCamp)
			{
  				pEnemy->TankDead(this);
				pHero->AddScore(eFromHero, m_tankData);
			}
		}
		return true;
	}
	else
	{
		return false;
	}
}


bool Tank::IsBeShot(GamePos &gamePos, EFightCamp eBulletCamp)
{
	if (m_tankData.eCamp != eBulletCamp)
	{
		std::vector<GamePos> vecBodayPos;
		BodayPos2RealPos(m_tankData, vecBodayPos);
		for (size_t i = 0; i < vecBodayPos.size(); i++)
		{
			if (gamePos.nX = vecBodayPos[i].nX && gamePos.nY == vecBodayPos[i].nY)
			{
				return true;
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

void Tank::Fire(EHeroNum eHeroNum)
{
	int nBulletX = m_tankData.nCenterX;
	int nBulletY = m_tankData.nCenterY;
	//
	switch (m_tankData.eDir)
	{
	case E_DIR_UP:
		nBulletY -= 2;
		break;
	case E_DIR_DOWN:
		nBulletY += 2;
		break;
	case E_DIR_LEFT:
		nBulletX -= 2;
		break;
	case E_DIR_RIGHT:
		nBulletX += 2;
		break;
	default:
		break;
	}
	if (nBulletX >= 1 && nBulletX <= GAME_AREA_WIDTH - 2 &&
		nBulletY >= 1 && nBulletY <= GAME_AREA_HEIGHT - 2)  //主角呼出武器然后开火
	{
		Weapon *pWeapon = (Weapon*)(m_pMainScence->FindMap("mainGameWeapon"));
		if (E_HERO_CAMP == m_tankData.eCamp)
		{
			pWeapon->HeroFire(nBulletX, nBulletY, m_tankData.eDir, m_tankData.nColor, eHeroNum);
		}
		else if (E_ENEMY_CAMP == m_tankData.eCamp)
		{
			pWeapon->EnemyFire(nBulletX, nBulletY, g_WallPic[E_WEAPON], m_tankData.eDir, F_H_RED); //敌人开火了
		}
	}
}

void Tank::Action()
{
	++m_u64Tickets;
	if (m_u64Tickets < m_u64NextTicket) return;
	m_u64NextTicket = m_u64Tickets + m_tankData.nSpeed;
	Walk();
	RandomFire();


}

void Tank::RandomMove()
{
	//5%的可能修改方向
	int  nRandomValue = (int)(rand() % 100) + 1;
	if (nRandomValue < 5)
	{
		EDir eDir = (EDir)(rand() % E_DIR_MAX);
		MoveDir(eDir);
		return;
	}
	//
	TankData tmpTank = m_tankData;
	UpdateTankDataByDir(tmpTank);
	if (!IsTankCanStep(tmpTank))
	{
		EDir eDir = (EDir)(rand() % E_DIR_MAX);
		MoveDir(eDir);
	}
	else
	{
		MoveDir(m_tankData.eDir);
	}
}

void Tank::RandomFire()
{
	//30%的可能开火
	int  nRandomValue = (int)(rand() % 100) + 1;
	if (nRandomValue < 30)
	{
		Fire(E_HERO_MOD_MAX);
	}
}

void Tank::RunPath()
{
	//1. 检测路径数据是否合法
	m_tankData.walkPath.nStep++;
	assert((size_t)m_tankData.walkPath.nStep < m_tankData.walkPath.vecWalkPath.size() && m_tankData.walkPath.nStep >= 0);
	//2. 取出目标点和当前点
	GamePos targetPos = m_tankData.walkPath.vecWalkPath[m_tankData.walkPath.nStep];
	int nCurrentX = m_tankData.nCenterX;
	int nCurrentY = m_tankData.nCenterY;
	//3.检测目标点是否合法
	assert(targetPos.nX == nCurrentX || targetPos.nY == nCurrentY);
	//4. 判断目标点在当前点的什么方向
	EDir eTargetDir = E_DIR_MAX;
	eTargetDir = GetDirByPosOffset(targetPos.nX, targetPos.nY, nCurrentX, nCurrentY);
	assert(eTargetDir != E_DIR_MAX);
	//5.如果当前方向和目标不一致仅仅修改方向，等待下一次调度
	if (eTargetDir != m_tankData.eDir)
	{
		RunPathJustChangeDir(eTargetDir);
	}
	//6. 如果方向一致需要检测是否可以走过去
	else
	{
		RunPathImee();
	}
}

void Tank::Walk()
{
	//如果行走路径为空随机行走
	if (m_tankData.walkPath.vecWalkPath.empty())
	{
		RandomMove();
	}
	else
	{
		RunPath();
	}
}

void Tank::RunPathJustChangeDir(EDir edir)
{
	//执行转向检测
	TankData tmpData = m_tankData;
	tmpData.eDir = edir;
	if (!IsTankCanStep(tmpData)) //如果修改方向不能通过
	{
		//等待次数减1
		m_tankData.walkPath.nWaitTimes--;
		//
		if (m_tankData.walkPath.nWaitTimes < 0)
		{
			//清理完数据然后返回
			m_tankData.walkPath.nWaitTimes = RUN_PATH_WAIT_TIMES;
			m_tankData.walkPath.nStep = 0;
			m_tankData.walkPath.vecWalkPath.clear();
		}
		else
		{
			//退回去
			m_tankData.walkPath.nStep--;
		}
	}
	else//如果修改方向可以通过
	{
		TankData preData = m_tankData;
		EraseTank();
		m_tankData = tmpData;
		ShowTank();
		//通知障碍物我来了
		FixWall *pFixWall = (FixWall*)(m_pMainScence->FindMap("mainGameFixWall"));
		pFixWall->TankStep(preData, tmpData);
		//退回去
		m_tankData.walkPath.nStep--;
	}
}

void Tank::RunPathImee()
{
	//执行移动检测
	TankData tmpData = m_tankData;
	UpdateTankDataByDir(tmpData);
	//如果修改方向不能通过
	if (!IsTankCanStep(tmpData))
	{
		//等待次数减1
		m_tankData.walkPath.nWaitTimes--;
		//
		if (m_tankData.walkPath.nWaitTimes < 0)
		{
			//清理完数据然后返回
			m_tankData.walkPath.nWaitTimes = RUN_PATH_WAIT_TIMES;
			m_tankData.walkPath.nStep = 0;
			m_tankData.walkPath.vecWalkPath.clear();
		}
		else
		{
			//退回去
			m_tankData.walkPath.nStep--;
		}
	}
	else//如果修改方向可以通过
	{
		MoveDir(m_tankData.eDir);
	}
	//如果是目标清空路径
	if (m_tankData.walkPath.nStep == m_tankData.walkPath.vecWalkPath.size() - 1)
	{
		m_tankData.walkPath.nWaitTimes = RUN_PATH_WAIT_TIMES;
		m_tankData.walkPath.nStep = 0;
		m_tankData.walkPath.vecWalkPath.clear();
	}
}

bool Tank::AStartCheck(unsigned int uTargetID, GamePos &preGamePos, GamePos &nowGamePos)
{
	TankData tmpTankData = m_tankData;
	tmpTankData.nCenterX = nowGamePos.nX;
	tmpTankData.nCenterY = nowGamePos.nY;
	//1.身体检测
	if (IsTankCanStep(tmpTankData, uTargetID))
	{
		//2. 转向检测
		EDir eTargetDir = E_DIR_MAX;
		eTargetDir = GetDirByPosOffset(nowGamePos.nX, nowGamePos.nY, preGamePos.nX, preGamePos.nY);
		if (eTargetDir == E_DIR_MAX) return false;
		//如果方向和坦克本身一致直接返回true
		if (m_tankData.eDir == eTargetDir) return true;
		//如果方向不一致需要跳转方向
		tmpTankData = m_tankData;
		tmpTankData.nCenterX = preGamePos.nX;
		tmpTankData.nCenterY = preGamePos.nY;
		tmpTankData.eDir = eTargetDir;
		if (IsTankCanStep(tmpTankData, uTargetID))
		{
			return true;
		}
	}
	return false;
}

void Tank::SetAstarPath(std::vector<GamePos> &vecPath)
{
	m_tankData.walkPath.vecWalkPath = vecPath;
	m_tankData.walkPath.nWaitTimes = RUN_PATH_WAIT_TIMES;
	m_tankData.walkPath.nStep = 0;
}

EDir Tank::GetDirByPosOffset(int nTargetX, int nTargetY, int nSrcX, int nSrcY)
{
	EDir eTargetDir = E_DIR_MAX;
	int nDiffX = nTargetX - nSrcX;
	int nDiffY = nTargetY - nSrcY;
	assert(abs(nDiffX) <= 1 && abs(nDiffY) <= 1);
	switch (nDiffX)
	{
	case -1:
		eTargetDir = E_DIR_LEFT;
		break;
	case 1:
		eTargetDir = E_DIR_RIGHT;
		break;
	default:
		break;
	}
	switch (nDiffY)
	{
	case -1:
		eTargetDir = E_DIR_UP;
		break;
	case 1:
		eTargetDir = E_DIR_DOWN;
		break;
	default:
		break;
	}
	return eTargetDir;
}

void Tank::ReduceAstarNiceValue()
{
	FixWall *pFixWall = (FixWall*)(m_pMainScence->FindMap("mainGameFixWall"));
	m_tankData.nAstarNiceValue -= (pFixWall->GetComplex());
}

void Tank::ResetAstarFailTimes()
{
	m_tankData.nAstarNiceValue = c_nTankNiceValue;
}
