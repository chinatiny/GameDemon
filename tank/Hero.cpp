#include "Hero.h"
#include "KeyboardValue.h"
#include "ColorDefine.h"
#include <assert.h>
#include "Weapon.h"
#include "Director.h"
#include "GameOverScence.h"
#include "AStar.h"
#include "ConsoleUIBuff.h"
#include "TankGame.h"
#include "SelectMapScence.h"
#include "MissionCompleteScence.h"

Hero::Hero()
{
	m_eMod = TankGame::GetGameInstance()->GetHeroNum();
}
Hero::~Hero(){}

void Hero::InitSpirit(Scence *pScence)
{
	//根据模式初始化英雄数据(一定有一个玩家被初始化)
	m_leftHero.nScore = 0;
	m_leftHero.pTank = nullptr;
	m_rightHero.nScore = 0;
	m_rightHero.pTank = nullptr;
	//
	m_leftHero.pTank = new Tank((MainScence*)pScence, c_nDefaultLeftX, c_nDefaultLeftY, E_TANK, E_DIR_UP, g_WallPic[E_TANK], F_H_YELLOW, E_HERO_CAMP, c_nDefaultHp, 0);
	if (E_RIGHT_HERO == m_eMod)
	{
		m_rightHero.pTank = new Tank((MainScence*)pScence, c_nDefaultRightX, c_nDefaultRightY, E_TANK, E_DIR_UP, g_WallPic[E_TANK], F_H_PURPLE, E_HERO_CAMP, c_nDefaultHp, 0);
	}
	//注册键盘资源
	RegKeyBoardNotify();
	//
	__super::InitSpirit(pScence);
}

void Hero::BeforDestorySpirit()
{
	//释放坦克
	if (m_leftHero.pTank) delete m_leftHero.pTank;
	if (m_rightHero.pTank) delete m_rightHero.pTank;
	//释放键盘资源
	UnRegKeyBoardNotify();
	__super::BeforDestorySpirit();
}

void Hero::BeforEnterScence()
{
	m_leftHero.pTank->ShowTank();
	if (E_RIGHT_HERO == m_eMod)
	{
		m_rightHero.pTank->ShowTank();
	}
	ShowScore();

}

void Hero::RcvKeyBoardNotify(char chKey)
{
	switch (chKey)
	{
	case E_KEY_UP:
	{
					 MoveUp(m_leftHero);
					 break;
	}

	case E_KEY_DOWN:
	{
					   MoveDown(m_leftHero);
					   break;
	}
	case E_KEY_LEFT:
	{
					   MoveLeft(m_leftHero);
					   break;
	}
	case E_KEY_RIGHT:
	{
						MoveRight(m_leftHero);
						break;
	}
	case E_KEY_SPACE:
	{
						Fire(m_leftHero, E_LEFT_HERO);
						break;
	}
	case E_KEY_o:
	{
					if (m_leftHero.pTank)
					{
						m_leftHero.pTank->Walk();
					}
					break;
	}
	case E_KEY_q:
	{				
					if (m_leftHero.pTank)
					{
						GamePos starPos = { m_leftHero.pTank->GetTankData().nCenterX, m_leftHero.pTank->GetTankData().nCenterY};
						GamePos targetPos = {GAME_AREA_WIDTH - 3, 2};
						AStar *pAStar = AStar::GetInstance(0xFFFFFFFF, starPos, targetPos, m_leftHero.pTank);
						if (pAStar->FindPath())
						{
							std::vector<GamePos> vecPath;
							pAStar->GetPath(vecPath);
							m_leftHero.pTank->SetAstarPath(vecPath);
						}
					}
					break;
	}
	case E_KEY_W:case E_KEY_w:
	{
					 MoveUp(m_rightHero);
					 break;
	}
	case E_KEY_s:case E_KEY_S:
	{
					 MoveDown(m_rightHero);
					 break;
	}
	case E_KEY_a:case E_KEY_A:
	{
					 MoveLeft(m_rightHero);
					 break;
	}
	case E_KEY_d:case E_KEY_D:
	{
					 MoveRight(m_rightHero);
					 break;
	}
	case E_KEY_y:
	{
					Fire(m_rightHero, E_RIGHT_HERO);
					break;
	}
	default:
		break;
	}
}

void Hero::SetGameMod(EHeroNum eMod)
{
	m_eMod = eMod;
}

void Hero::MoveUp(HeroData &heroData)
{
	if (heroData.pTank)
	{
		heroData.pTank->MoveDir(E_DIR_UP);
	}

}

void Hero::MoveDown(HeroData &heroData)
{
	if (heroData.pTank)
	{
		heroData.pTank->MoveDir(E_DIR_DOWN);
	}
	
}

void Hero::MoveLeft(HeroData &heroData)
{
	if (heroData.pTank)
	{
		heroData.pTank->MoveDir(E_DIR_LEFT);
	}

}

void Hero::MoveRight(HeroData &heroData)
{
	if (heroData.pTank)
	{
		heroData.pTank->MoveDir(E_DIR_RIGHT);
	}

}


bool Hero::BeShot(Weapon::Bullet & bullet)
{
	GamePos gamePos = {bullet.nX, bullet.nY};
	if (nullptr != m_leftHero.pTank)
	{
		if (m_leftHero.pTank->BeShot(gamePos, bullet.eCamp, bullet.eHero))
		{
			ShowScore();
			return true;
		}
	}
	if (nullptr != m_rightHero.pTank)
	{
		if (m_rightHero.pTank->BeShot(gamePos, bullet.eCamp, bullet.eHero))
		{
			ShowScore();
			return true;
		}
	}
	return false;
}

void Hero::Fire(HeroData &heroData, EHeroNum eHero)
{
	if (nullptr != heroData.pTank)
	{
		heroData.pTank->Fire(eHero);
	}
}


void Hero::TankDead(Tank *pTank)
{
	//判断两个玩家是否挂掉了
	if (pTank == m_leftHero.pTank)
	{
		delete m_leftHero.pTank;
		m_leftHero.pTank = nullptr;
	}
	if (pTank == m_rightHero.pTank)
	{
		delete m_rightHero.pTank;
		m_rightHero.pTank = nullptr;
	}
	//如果两个同时挂掉了就GameOver吧
	if (nullptr == m_leftHero.pTank && nullptr == m_rightHero.pTank)
	{
		Director *pDirector = Director::GetInstance();
		//进入游戏菜单
		GameOverScence *pGameOver = new GameOverScence();
		pDirector->ChangeScence(pGameOver);
	}
}

void Hero::GetHeroLeftData(HeroData &leftHero, HeroData &rightHero)
{
	leftHero = m_leftHero;
	rightHero = m_rightHero;
}

void Hero::ShowScore()
{
	// 
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 1, "                ", 0);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 1, "              ", 0);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 2, "              ", 0);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 4, "              ", 0);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 5, "              ", 0);
	//
	char szTipBuff[256];
	int nLeftScore = m_leftHero.nScore;
	int nLeftHP = m_leftHero.pTank == nullptr ? 0 : m_leftHero.pTank->GetTankData().nHP;
	//
	int nRightScore = m_rightHero.nScore;
	int nRightHP = m_rightHero.pTank == nullptr ? 0 : m_rightHero.pTank->GetTankData().nHP;

	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 - 2, "积分和到5分过关", F_RED);

	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "玩家一积分:%d", nLeftScore);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 1, szTipBuff, F_H_PURPLE);
	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "玩家一HP:%d", nLeftHP);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 2, szTipBuff, F_H_PURPLE);

	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "玩家二积分:%d", nRightScore);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 4, szTipBuff, F_H_PURPLE);
	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "玩家二HP:%d", nRightHP);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, WINDOWS_HEIGHT / 2 + 5, szTipBuff, F_H_PURPLE);
}

void Hero::AddScore(EHeroNum eHero, Tank::TankData &tankData)
{
	if (eHero == E_LEFT_HERO)
	{
		AddScore(m_leftHero, tankData);
	}
	if (eHero == E_RIGHT_HERO)
	{
		AddScore(m_rightHero, tankData);
	}
}

void Hero::AddScore(HeroData &heroData, Tank::TankData &tankData)
{
	heroData.nScore++;
	ShowScore();
	MissionComplete();
}

void Hero::MissionComplete()
{
	int nLeftScore = m_leftHero.nScore;
	int nRightScore = m_rightHero.nScore;
	//
	if (nLeftScore + nRightScore >= 5)
	{
		MissionCompleteScence *pMissionComplete = new MissionCompleteScence();
		Director *pDirector = Director::GetInstance();
		pDirector->ChangeScence(pMissionComplete);
	}

}
