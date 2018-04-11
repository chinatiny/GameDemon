#include "MainScence.h"
#include "ConsoleUIBuff.h"
#include "FixWall.h"
#include "Hero.h"
#include "Enemy.h"
#include <assert.h>
#include "Tank.h"
#include "Director.h"
#include "KeyboardValue.h"
#include "MenuScence.h"
#include "ColorDefine.h"
#include "TankGame.h"

unsigned int MainScence::s_nLastID = E_ID_OTHER;

MainScence::MainScence(){}
MainScence::~MainScence(){}


void MainScence::InitScence()
{
	//初始化白板
	for (int i = 0; i < GAME_AREA_HEIGHT; i++)
	{
		for (int j = 0; j < GAME_AREA_WIDTH; j++)
		{
			ChangePosAttr(GetActorID(E_EMPTY), E_CAMP_MAX,  j, i, E_EMPTY, g_WallPic[E_EMPTY], 0);
		}
	}

	//各种障碍物
	FixWall *pFixWall = new FixWall();
	AddSpirit(pFixWall);
	MapSpirit("mainGameFixWall", pFixWall);

	//武器也是演员
	Weapon *pWeapon = new Weapon();
	AddSpirit(pWeapon);
	//做一个名字映射,方便和别的精灵交互
	MapSpirit("mainGameWeapon", pWeapon);


	//初始化主角,添加主角
	Hero *pHero = new Hero();
	AddSpirit(pHero);
	MapSpirit("mainGameHero", pHero);

	//敌军坦克
	Enemy *pEnemy = new Enemy();
	AddSpirit(pEnemy);
	MapSpirit("mainGameEnemy", pEnemy);

	//向键盘注册通知
	RegKeyBoardNotify();
}

void MainScence::BeforEnterScence()
{
}

void MainScence::DestoryScence()
{
	//ID回绕，避免过大
	s_nLastID = E_ID_OTHER;
	//向键盘反注册通知
	UnRegKeyBoardNotify();
	ConsoleUIBuff::GetInstance()->CleanBuff();
	__super::DestoryScence();
}

void MainScence::ShowScence()
{
	for (int i = 0; i < GAME_AREA_HEIGHT; i++)
	{
		for (int j = 0; j < GAME_AREA_WIDTH; j++)
		{
			ConsoleUIBuff::GetInstance()->DrawDelay(j, i, m_scenceArryPos[i][j].szPic, m_scenceArryPos[i][j].nColor);
		}
	}
}

void MainScence::RcvKeyBoardNotify(char chKey)
{
	if (chKey == (char)E_KEY_ESC)
	{
		TankGame::GetGameInstance()->GamePause();
		//打印出提示字符串
		ConsoleUIBuff::GetInstance()->DrawImme(WINDOWS_WIDTH / 2 - 15, WINDOWS_HEIGHT / 2, "是否终止游戏进入菜单:(y/n):", F_H_RED);
		char szInputBuff[256] = { 0 };
		scanf_s("%s", szInputBuff, sizeof(szInputBuff));

		if (strcmp(szInputBuff, "y") == 0 || strcmp(szInputBuff, "yes") == 0
			|| strcmp(szInputBuff, "Y") == 0 || strcmp(szInputBuff, "YES") == 0)
		{
			//获取导演类
			Director *pDirector = Director::GetInstance();
			//进入菜单界面
			MenuScence *pMenuScence = new MenuScence();
			pDirector->ChangeScence(pMenuScence);
		}
		else
		{
			ConsoleUIBuff::GetInstance()->DrawImme(WINDOWS_WIDTH / 2 - 15, WINDOWS_HEIGHT / 2, "                                  ", F_H_RED);
		}
		//重新设置光标不可见
		ConsoleUIBuff::SetCursorUnVisable();
		TankGame::GetGameInstance()->GameResume();
	}
}


void MainScence::ChangePosAttr(unsigned int uID, EFightCamp eCamp, int nX, int nY, EWallType eWalltype, const char* szPic, int nColor)
{
	//把边框也算作障碍物的一种
	assert(nX >= 0 && nX <= GAME_AREA_WIDTH - 1);
	assert(nY >= 0 && nY <= GAME_AREA_HEIGHT - 1);
	m_scenceArryPos[nY][nX].nColor = nColor;
	m_scenceArryPos[nY][nX].eWalltype = eWalltype;
	m_scenceArryPos[nY][nX].uID = uID;
	m_scenceArryPos[nY][nX].eCamp = eCamp;
	strcpy_s(m_scenceArryPos[nY][nX].szPic, sizeof(m_scenceArryPos[nY][nX].szPic) - 1, szPic);


}

void MainScence::ErasePos(int nX, int nY)
{
	ChangePosAttr(0, E_CAMP_MAX,  nX, nY, E_EMPTY, "  ", 0);
}


ScenceActor MainScence::DirtyWallActor(int nX, int nY)
{
	return m_scenceArryPos[nY][nX];
}

unsigned int MainScence::GetActorID(EWallType eWallType)
{
	switch (eWallType)
	{
	case E_EMPTY:
	{
					return E_ID_EMPTY;
	}
	case E_BORDER:
	{
					 return E_ID_BORDER;
	}
	case E_WATER:
	{
					return E_ID_WATER;
	}
	case E_SOIL:
	{
				   return E_ID_SOIL;
	}
	case E_IRON:
	{
				   return E_ID_IRON;
	}
	case E_GRASS:
	{
					return E_ID_GRASS;
	}
	default:
	{
			   unsigned int uRet = s_nLastID;
			   ++s_nLastID;
			   if (E_ID_INVALID == s_nLastID)
			   {
				   s_nLastID = E_ID_OTHER;
			   }
			   return uRet;
	}
	}
}

void MainScence::DirtyGetEmptyPos(std::vector<GamePos> &vecEmptyPos)
{
	for (int i = 0; i < GAME_AREA_HEIGHT; i++)
	{
		for (int j = 0; j < GAME_AREA_WIDTH; j++)
		{
			if (E_EMPTY == m_scenceArryPos[i][j].eWalltype)
			{
				GamePos gamePos = {j, i};
				vecEmptyPos.push_back(gamePos);
			}
		}
	}
}

void MainScence::GetTankCanUseEmptyPos(std::vector<GamePos> &vecCanUsePos)
{
	std::vector<GamePos> vecEmptyPoses;
	DirtyGetEmptyPos(vecEmptyPoses);
	for (int i = 0; i < (int)vecEmptyPoses.size(); i++)
	{
		bool bIsCanStep = true;
		for (int p = 0; p < Tank::c_nTankHeight; p++)
		{
			for (int q = 0; q < Tank::c_nTankWidth; q++)
			{
				int nOffsetX = q - 1;
				int nOffsetY = p - 1;
				int nTmpPosX, nTmpPosY;
				EWallType eWallType;
				nTmpPosX = vecEmptyPoses[i].nX + nOffsetX;
				nTmpPosY = vecEmptyPoses[i].nY + nOffsetY;
				eWallType = DirtyWallActor(nTmpPosX, nTmpPosY).eWalltype;
				if (nTmpPosX <= 0 || nTmpPosX >= GAME_AREA_WIDTH - 1 ||
					nTmpPosY <= 0 || nTmpPosY >= GAME_AREA_HEIGHT - 1 ||
					eWallType != E_EMPTY
					)
				{
					bIsCanStep = false;
				}
			}
		}
		if (bIsCanStep) vecCanUsePos.push_back(vecEmptyPoses[i]);
	}
}

