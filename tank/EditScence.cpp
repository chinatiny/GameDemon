#include "EditScence.h"
#include "ColorDefine.h"
#include "MainScence.h"
#include "Common.h"
#include "Director.h"
#include "MenuScence.h"
#include "ConsoleUIBuff.h"
#include "TankGame.h"
#include "Hero.h"
#include "KeyboardValue.h"
#include <fstream>


EditScence::EditScence(){}
EditScence::~EditScence(){}

void EditScence::InitScence()
{
	m_pLeft = new Tank((MainScence*)nullptr, Hero::c_nDefaultLeftX, Hero::c_nDefaultLeftY, E_TANK, E_DIR_UP, g_WallPic[E_TANK], F_H_YELLOW, E_HERO_CAMP, 0, 0);
	m_pRight = new Tank((MainScence*)nullptr, Hero::c_nDefaultRightX, Hero::c_nDefaultRightY, E_TANK, E_DIR_UP, g_WallPic[E_TANK], F_H_PURPLE, E_HERO_CAMP, 0, 0);
	//
	std::vector<GamePos> vecPos;
	Tank::TankData tkData = m_pLeft->GetTankData();
	m_pLeft->BodayPos2RealPos(tkData, vecPos);
	//画左边的坦克
	for (size_t i = 0; i < vecPos.size(); i++)
	{
		m_vecUsePos.push_back(vecPos[i]);
		ConsoleUIBuff::GetInstance()->DrawImme(vecPos[i].nX, vecPos[i].nY, g_WallPic[E_TANK], F_H_YELLOW);
	}
	//画右边的坦克
	vecPos.clear();
	tkData = m_pRight->GetTankData();
	m_pRight->BodayPos2RealPos(tkData, vecPos);
	for (size_t i = 0; i < vecPos.size(); i++)
	{
		m_vecUsePos.push_back(vecPos[i]);
		ConsoleUIBuff::GetInstance()->DrawImme(vecPos[i].nX, vecPos[i].nY, g_WallPic[E_TANK], F_H_PURPLE);
	}
	//
	m_hStdIn = GetStdHandle(STD_INPUT_HANDLE);
	GetConsoleMode(m_hStdIn, &m_dwOldConsoleMode);
	SetConsoleMode(m_hStdIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	//
	m_eEditSelect = E_EDIT_MAX;

}

void EditScence::DestoryScence()
{
	//设置最初的光标
	SetConsoleMode(m_hStdIn, m_dwOldConsoleMode);
	ConsoleUIBuff::GetInstance()->CleanBuff();
	__super::DestoryScence();
}

void EditScence::BeforEnterScence()
{
	for (int i = 0; i < GAME_AREA_HEIGHT; i++)
	{
		for (int j = 0; j < GAME_AREA_WIDTH; j++)
		{
			if (0 == i || 0 == j || GAME_AREA_HEIGHT - 1 == i || GAME_AREA_WIDTH - 1 == j)
			{
				ConsoleUIBuff::GetInstance()->DrawImme(j, i, "◆", B_H_WHITE);
			}
		}
	}
	char szTipBuff[256] = {0};
	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "★     %s 土墙    ★", g_WallPic[E_SOIL]);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 6, "★★★★★★★★★★", F_H_YELLOW);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 7, szTipBuff, F_H_YELLOW);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 8, "★★★★★★★★★★", F_H_YELLOW);

	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "★    %s水        ★", g_WallPic[E_WATER]);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 10, "★★★★★★★★★★", F_H_BLUE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 11, szTipBuff, F_H_BLUE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 12, "★★★★★★★★★★", F_H_BLUE);

	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "★    %s草        ★", g_WallPic[E_GRASS]);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 14, "★★★★★★★★★★", F_H_GREEN);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 15, szTipBuff, F_H_GREEN);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 16, "★★★★★★★★★★", F_H_GREEN);

	sprintf_s(szTipBuff, sizeof(szTipBuff)-1, "★    %s铁        ★", g_WallPic[E_IRON]);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 18, "★★★★★★★★★★", F_WHITE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 19, szTipBuff, F_WHITE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 20, "★★★★★★★★★★", F_WHITE);



	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 24, "★★★★★★★★★★", F_H_PURPLE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 25, "★      保存      ★", F_H_PURPLE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 26, "★★★★★★★★★★", F_H_PURPLE);

	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 28, "★★★★★★★★★★", F_H_RED);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 29, "★      退出      ★", F_H_RED);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 30, "★★★★★★★★★★", F_H_RED);

	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 32, "★★★★★★★★★★", F_BLUE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 33, "★   擦除元素     ★", F_BLUE);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 34, "★★★★★★★★★★", F_BLUE);

	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "           ", 0);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:无", F_H_PURPLE);

}

void EditScence::RunScence()
{
	// INPUT_RECORD  读取控制台输入信息都在这个结构体
	INPUT_RECORD inputRecord = { 0 };
	//一个 unsigned int 型 这里被typedef 定义过
	DWORD dwSize = 0;
	COORD stcPos = { 0 };

	ReadConsoleInput(m_hStdIn, &inputRecord, 1, &dwSize);

	//鼠标移动事件
	if (inputRecord.EventType == MOUSE_EVENT)
	{
		if (inputRecord.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
		{
			//直接忽略掉双击
			if (inputRecord.Event.MouseEvent.dwEventFlags == (DOUBLE_CLICK)) return;
			//下面是单击处理
			stcPos = inputRecord.Event.MouseEvent.dwMousePosition;
			//
			if (stcPos.X >= 126 && stcPos.X <= 145 &&
				stcPos.Y >= 6 && stcPos.Y < 8)
			{
				//土墙
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "              ", 0);
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:土墙", F_H_PURPLE);
				m_eEditSelect = E_EDIT_SELECT_WALL;
			}
			else if (stcPos.X >= 126 && stcPos.X <= 145 &&
				stcPos.Y >= 10 && stcPos.Y < 12)
			{
				//水
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "          ", 0);
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:水", F_H_PURPLE);
				m_eEditSelect = E_EDIT_SELECT_WATER;

			}
			else if (stcPos.X >= 126 && stcPos.X <= 145 &&
				stcPos.Y >= 14 && stcPos.Y < 16)
			{
				//草
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "           ", 0);
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:草", F_H_PURPLE);
				m_eEditSelect = E_EDIT_GRASS;
			}
			else if (stcPos.X >= 126 && stcPos.X <= 145 &&
				stcPos.Y >= 18 && stcPos.Y < 20)
			{
				//铁
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "           ", 0);
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:铁", F_H_PURPLE);
				m_eEditSelect = E_EDIT_IRON;
			}
			else if (stcPos.X >= 126 && stcPos.X <= 145 &&
				stcPos.Y >= 24 && stcPos.Y < 26)
			{
				//保存
				if (m_eEditSelect == E_EDIT_MAX)
				{
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "                    ", 0);
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:不能保存!", F_H_PURPLE);
				}
				else
				{
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "              ", 0);
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:保存", F_H_PURPLE);
					m_eEditSelect = E_EDIT_SAVE;
					SaveToFile();
				}
			}
			else if (stcPos.X >= 126 && stcPos.X <= 145 &&
				stcPos.Y >= 28 && stcPos.Y < 30)
			{
				//退出
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "              ", 0);
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:退出", F_H_PURPLE);
				m_eEditSelect = E_EDIT_EXIT;
				//进入主菜单界面
				Director *pDirector = Director::GetInstance();
				//进入菜单界面
				MenuScence *pMainGame = new MenuScence();
				pDirector->ChangeScence(pMainGame);
			}
			else if (stcPos.X >= 126 && stcPos.X <= 145 &&
				stcPos.Y >= 32 && stcPos.Y < 34)
			{
				//擦除
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "              ", 0);
				ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "选择:擦除", F_H_PURPLE);
				m_eEditSelect = E_EDIT_ERASE;
			}
			else if (stcPos.X >= 2 && stcPos.X <= 118 &&
				stcPos.Y >= 1 && stcPos.Y <= 49)
			{
				if (E_EDIT_ERASE == m_eEditSelect)//擦除
				{
					ConsoleUIBuff::GetInstance()->DrawImme(stcPos.X / 2, stcPos.Y, "  ", 0);
					DeleteEditNode(stcPos.X / 2, stcPos.Y);
				}
				else if (E_EDIT_MAX != m_eEditSelect && !IsHasDraw(stcPos.X / 2, stcPos.Y))
				{
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "             ", 0);
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:绘图", F_H_PURPLE);
					if (E_EDIT_SELECT_WALL == m_eEditSelect)
					{
						ConsoleUIBuff::GetInstance()->DrawImme(stcPos.X / 2, stcPos.Y, g_WallPic[E_SOIL], F_H_YELLOW);
						AddEditNode(stcPos.X / 2, stcPos.Y, E_SOIL);
					}
					else if (E_EDIT_SELECT_WATER == m_eEditSelect)
					{
						ConsoleUIBuff::GetInstance()->DrawImme(stcPos.X / 2, stcPos.Y, g_WallPic[E_WATER], F_H_BLUE);
						AddEditNode(stcPos.X / 2, stcPos.Y, E_WATER);
					}
					else if (E_EDIT_GRASS == m_eEditSelect)
					{
						ConsoleUIBuff::GetInstance()->DrawImme(stcPos.X / 2, stcPos.Y, g_WallPic[E_GRASS], F_H_GREEN);
						AddEditNode(stcPos.X / 2, stcPos.Y, E_GRASS);
					}
					else if (E_EDIT_IRON == m_eEditSelect)
					{
						ConsoleUIBuff::GetInstance()->DrawImme(stcPos.X / 2, stcPos.Y, g_WallPic[E_IRON], F_WHITE);
						AddEditNode(stcPos.X / 2, stcPos.Y, E_IRON);
					}
				}
				else if (E_EDIT_MAX == m_eEditSelect)
				{
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "                    ", 0);
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:请先选择", F_H_PURPLE);
				}
				else if (IsHasDraw(stcPos.X / 2, stcPos.Y))
				{
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "                       ", 0);
					ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:不可编辑", F_H_PURPLE);
				}
			}

		}
	}
}

void EditScence::AddEditNode(int nX, int nY, EWallType eWallType)
{
	EditNode editNode;
	editNode.nX = nX;
	editNode.nY = nY;
	editNode.eWalltype = eWallType;
	m_vecEditNodes.push_back(editNode);
	//
	GamePos gamePos = {editNode.nX, editNode.nY };
	m_vecUsePos.push_back(gamePos);
}

void EditScence::DeleteEditNode(int nX, int nY)
{
	for (auto ite = m_vecEditNodes.begin(); ite != m_vecEditNodes.end(); ite++)
	{
		if (nX == ite->nX && nY == ite->nY)
		{
			m_vecEditNodes.erase(ite);
			break;
		}
	}
	//
	for (auto ite = m_vecUsePos.begin(); ite != m_vecUsePos.end(); ite++)
	{
		if (nX == ite->nX && nY == ite->nY)
		{
			m_vecUsePos.erase(ite);
			break;
		}
	}

}

void EditScence::SaveToFile()
{

	SetConsoleMode(m_hStdIn, m_dwOldConsoleMode);
	ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH / 2 - 15, GAME_AREA_HEIGHT / 2, "请输入你的保存文件名:", F_H_RED);
	fflush(stdin);
	char szInputBuff[256] = { 0 };
	scanf_s("%s", szInputBuff, sizeof(szInputBuff));
	fflush(stdin);
	std::string strFullPath = TankGame::GetGameInstance()->GetGameExeDir() + "map/" + szInputBuff + ".map";
	std::ofstream writeFile(strFullPath.c_str(), std::ios::out | std::ios::binary);
	if (writeFile.is_open())
	{
		int nLen = m_vecEditNodes.size();
		if (nLen <= 0) //全部擦除了
		{
			//写四个字节的0
			int nSize = 0;
			writeFile.write((char*)&nSize, 4);
		}
		else
		{
			int nSize = nLen *sizeof(EditNode);
			//写入四个字节的长度
			writeFile.write((char*)&nSize, 4);
			//循环写入
			for (auto ite = m_vecEditNodes.begin(); ite != m_vecEditNodes.end(); ite++)
			{
				EditNode editNode = *ite;
				writeFile.write((char*)&editNode, sizeof(editNode));
			}
		}
		writeFile.close();
		ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "                     ", 0);
		ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "状态:已经保存", F_H_PURPLE);
	}
	else
	{
		ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "                     ", 0);
		ConsoleUIBuff::GetInstance()->DrawImme(GAME_AREA_WIDTH + 2, 2, "创建文件失败", F_H_PURPLE);
	}
	//恢复光标
	SetConsoleMode(m_hStdIn, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

bool EditScence::IsHasDraw(int nX, int nY)
{
	for (auto ite = m_vecUsePos.begin(); ite != m_vecUsePos.end(); ite++)
	{
		if (nX == ite->nX && nY == ite->nY)
		{
			return true;
		}
	}
	return false;
}
