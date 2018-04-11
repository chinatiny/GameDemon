#include "FixWall.h"
#include "TankGame.h"
#include "Common.h"
#include "ColorDefine.h"
#include "EditScence.h"
#include <fstream>
#include "Tank.h"
#include "SelectMapScence.h"

FixWall::FixWall(){}
FixWall::~FixWall(){}




void FixWall::InitSpirit(Scence *pScence)
{
	//1. 初始化边框
	for (int i = 0; i < GAME_AREA_HEIGHT; i++)
	{
		for (int j = 0; j < GAME_AREA_WIDTH; j++)
		{
			if (0 == i || 0 == j || GAME_AREA_HEIGHT - 1 == i || GAME_AREA_WIDTH - 1 == j)
			{
				FixNode fixNode;
				ScenceActor borderActor;
				borderActor.uID = MainScence::GetActorID(E_BORDER);
				strcpy_s(borderActor.szPic, sizeof(borderActor.szPic), g_WallPic[E_BORDER]);
				borderActor.nColor = B_H_WHITE;
				borderActor.eWalltype = E_BORDER;
				//
				fixNode.nX = j;
				fixNode.nY = i;
				fixNode.actor = borderActor;
				m_vecBorder.push_back(fixNode);
			}
		}
	}
	//2. 文件中读取草地、水、土墙、铁块
	std::string strFullPath = TankGame::GetGameInstance()->GetGameExeDir() + "map\\" + SelectMapScence::s_strMapName;
	std::ifstream readFile(strFullPath.c_str(), std::ios::in | std::ios::binary);
	if (readFile.is_open())
	{
		int nLen;
		readFile.read((char*)&nLen, 4);
		if (nLen > 0)
		{
			EditScence::EditNode editNode;
			int nCount = nLen / sizeof(editNode);
			for (int i = 0; i < nCount; i++)
			{
				readFile.read((char*)&editNode, sizeof(editNode));
				FixNode fixNode;
				//
				fixNode.nX = editNode.nX;
				fixNode.nY = editNode.nY;
				ScenceActor scenceActor;
				scenceActor.eWalltype = editNode.eWalltype;
				//
				if (E_WATER == scenceActor.eWalltype)
				{
					scenceActor.uID = MainScence::GetActorID(E_WATER);
					scenceActor.nColor = F_H_BLUE;
					strcpy_s(scenceActor.szPic, sizeof(scenceActor.szPic), g_WallPic[E_WATER]);
					fixNode.actor = scenceActor;
					m_vecWater.push_back(fixNode);
				}
				else if (E_GRASS == scenceActor.eWalltype)
				{
					scenceActor.uID = MainScence::GetActorID(E_GRASS);
					scenceActor.nColor = F_H_GREEN;
					strcpy_s(scenceActor.szPic, sizeof(scenceActor.szPic), g_WallPic[E_GRASS]);
					fixNode.actor = scenceActor;
					m_vecGrass.push_back(fixNode);
				}
				else if (E_SOIL == scenceActor.eWalltype)
				{
					scenceActor.uID = MainScence::GetActorID(E_SOIL);
					scenceActor.nColor = F_H_YELLOW;
					strcpy_s(scenceActor.szPic, sizeof(scenceActor.szPic), g_WallPic[E_SOIL]);
					fixNode.actor = scenceActor;
					m_vecSoil.push_back(fixNode);
				}
				else if (E_IRON == scenceActor.eWalltype)
				{
					scenceActor.uID = MainScence::GetActorID(E_IRON);
					scenceActor.nColor = F_WHITE;
					strcpy_s(scenceActor.szPic, sizeof(scenceActor.szPic), g_WallPic[E_IRON]);
					fixNode.actor = scenceActor;
					m_vecIron.push_back(fixNode);
				}
			}
		}
		else
		{
			m_vecWater.clear();
			m_vecGrass.clear();
			m_vecSoil.clear();
			m_vecIron.clear();

		}
		readFile.close();
	}

	m_nComplex = m_vecWater.size() + m_vecGrass.size() + m_vecSoil.size() + m_vecIron.size();
	__super::InitSpirit(pScence);
}

void FixWall::BeforEnterScence()
{
	ShowBorder();
	ShowWater();
	ShowGrass();
	ShowSoil();
	ShowIron();
}


void FixWall::ShowFixNode(FixNode &fixNode)
{
	((MainScence*)m_pScence)->ChangePosAttr(fixNode.actor.uID, E_CAMP_MAX, fixNode.nX, fixNode.nY, fixNode.actor.eWalltype, fixNode.actor.szPic, fixNode.actor.nColor);
}

void FixWall::ShowFixWall(std::vector<FixNode> vecWall)
{
	for (size_t i = 0; i < vecWall.size(); i++)
	{
		ShowFixNode(vecWall[i]);
	}
}

void FixWall::ShowBorder()
{
	ShowFixWall(m_vecBorder);
}

void FixWall::ShowWater()
{
	ShowFixWall(m_vecWater);
}

void FixWall::ShowGrass()
{
	ShowFixWall(m_vecGrass);
}

void FixWall::ShowSoil()
{
	ShowFixWall(m_vecSoil);
}

void FixWall::ShowIron()
{
	ShowFixWall(m_vecIron);
}

void FixWall::TankStep(Tank::TankData &preTank, Tank::TankData &nowTank)
{
	//把重合点重新恢复成草地
	ChangeCoveringPosAttr(m_vecGrass, preTank, MainScence::GetActorID(E_GRASS), E_GRASS, g_WallPic[E_GRASS], F_H_GREEN);
	//把现在的重合点修改为坦克
	ChangeCoveringPosAttr(m_vecGrass, nowTank, nowTank.uID, E_TANK, nowTank.szPic, F_H_GREEN);
}

void FixWall::TankDead(Tank::TankData &preTank)
{
	//把重合点重新恢复成草地
	ChangeCoveringPosAttr(m_vecGrass, preTank, MainScence::GetActorID(E_GRASS), E_GRASS, g_WallPic[E_GRASS], F_H_GREEN);
}




bool FixWall::IsCoveringPos(std::vector<FixNode> &beCheckPos, GamePos &gamePos)
{
	for (size_t i = 0; i < beCheckPos.size(); i++)
	{
		if (beCheckPos[i].nX == gamePos.nX && beCheckPos[i].nY == gamePos.nY)
		{
			return true;
		}
	}
	return false;
}

void FixWall::GetCoveringPos(std::vector<FixNode> &beCheckPos, std::vector<GamePos> &checkGivePos, std::vector<GamePos> &getPos)
{
	for (size_t i = 0; i < checkGivePos.size(); i++)
	{
		if (IsCoveringPos(beCheckPos, checkGivePos[i]))
		{
			getPos.push_back(checkGivePos[i]);
		}
	}
}

void FixWall::ChangePosAttr(std::vector<FixNode> &beChangedPos, unsigned int uID, int nX, int nY, EWallType eWallType, char *pszPic, int nColor)
{
	for (size_t i = 0; i < beChangedPos.size(); i++)
	{
		if (beChangedPos[i].nX == nX && beChangedPos[i].nY == nY)
		{
			strcpy_s(beChangedPos[i].actor.szPic, sizeof(beChangedPos[i].actor.szPic) - 1, pszPic);
			beChangedPos[i].actor.nColor = nColor;
			beChangedPos[i].actor.eWalltype = eWallType;
			beChangedPos[i].actor.uID = uID;
			ShowFixNode(beChangedPos[i]);
			break;
		}
	}
}

void FixWall::ChangeCoveringPosAttr(std::vector<FixNode> &fixPos, Tank::TankData &tankData, unsigned int uID, EWallType eWallType, char *pszPic, int nColor)
{
	//获取坦克身体的点
	std::vector<GamePos> vecBodayPos;
	Tank::BodayPos2RealPos(tankData, vecBodayPos);
	//检测和草地的重合点
	std::vector<GamePos> vecCoveringPos;
	GetCoveringPos(fixPos, vecBodayPos, vecCoveringPos);
	//把重合点修改属性
	for (size_t i = 0; i < vecCoveringPos.size(); i++)
	{
		ChangePosAttr(m_vecGrass, uID, vecCoveringPos[i].nX, vecCoveringPos[i].nY, eWallType, pszPic, nColor);
	}
}


void FixWall::WaterBeShot(Weapon::Bullet &bullet)
{
	ChangePosAttr(m_vecWater, bullet.uID, bullet.nX, bullet.nY, bullet.eWalltype, bullet.szPic, bullet.nColor);
}

void FixWall::GrassBeShot(Weapon::Bullet &bullet)
{
	ChangePosAttr(m_vecGrass, bullet.uID, bullet.nX, bullet.nY, bullet.eWalltype, bullet.szPic, bullet.nColor);
}

void FixWall::SoilBeShot(Weapon::Bullet &bullet)
{
	DeleteFixNode(m_vecSoil, bullet.nX, bullet.nY);
}

bool FixWall::IsWater(GamePos &gamePos)
{
	return IsCoveringPos(m_vecWater, gamePos);
}

bool FixWall::IsGrass(GamePos &gamePos)
{
	return IsCoveringPos(m_vecGrass, gamePos);
}

bool FixWall::SetWater(GamePos &gamePos)
{
	ChangePosAttr(m_vecWater, MainScence::GetActorID(E_WATER), gamePos.nX, gamePos.nY, E_WATER, g_WallPic[E_WATER], F_H_BLUE);
	return true;
}

bool FixWall::SetGrass(GamePos &gamePos)
{
	ChangePosAttr(m_vecGrass, MainScence::GetActorID(E_GRASS), gamePos.nX, gamePos.nY, E_GRASS, g_WallPic[E_GRASS], F_H_GREEN);
	return true;
}

void FixWall::DeleteFixNode(std::vector<FixNode> &fixPos, int nX, int nY)
{
	std::vector<FixNode> tmpVec;
	for (size_t i = 0; i < fixPos.size(); i++)
	{
		if (fixPos[i].nX == nX && fixPos[i].nY == nY)
		{
			((MainScence*)m_pScence)->ErasePos(nX, nY);
		}
		else
		{
			tmpVec.push_back(fixPos[i]);
		}
	}
	fixPos.clear();
	fixPos = tmpVec;
}

int FixWall::GetComplex()
{
	return m_nComplex;
}

