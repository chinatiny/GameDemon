#include "SelectMapScence.h"
#include "CommTool.h"
#include "ColorDefine.h"
#include "KeyboardValue.h"
#include "MenuScence.h"
#include "MainScence.h"
#include "TankGame.h"
#include "ConsoleUIBuff.h"


std::vector<EditScence::EditNode> SelectMapScence::s_vecEditNode;
const char* SelectMapScence::c_pszArrow = "◆";
std::string SelectMapScence::s_strMapName = "DefaultMap.map";
const char* SelectMapScence::c_pszEscTip = "按ESC键进入菜单界面";
const char* SelectMapScence::c_pszEnterTip = "按Enter进入游戏";

SelectMapScence::SelectMapScence(){}
SelectMapScence::~SelectMapScence(){}

void SelectMapScence::InitScence()
{
	std::string strMapPath = TankGame::GetGameInstance()->GetGameExeDir() + "/map/";
	::GetAllFiles((char*)strMapPath.c_str(), ".map", m_vecFiles);
	m_nOffset = 0;
	//向键盘注册通知
	RegKeyBoardNotify();

}

void SelectMapScence::BeforEnterScence()
{
	int nOffset = m_nOffset;
	//把文件名给画出来
	for (size_t i = 0; i < m_vecFiles.size(); i++)
	{
		std::string strFileName = m_vecFiles[i];
		ConsoleUIBuff::GetInstance()->DrawImme(c_nFileNameBaseX + 2, c_nFileNameBaseY + nOffset, (char*)strFileName.c_str(), F_H_YELLOW);
		nOffset++;
	}
	//把光标画出来
	if (m_vecFiles.size() > 0)
	{
		DrawSelect(0);
	}

	ConsoleUIBuff::GetInstance()->DrawImme(c_nEnterTipX, c_nEnterTipY, (char*)c_pszEnterTip, F_H_RED);
	ConsoleUIBuff::GetInstance()->DrawImme(c_nEscTipX, c_nEscTipY, (char*)c_pszEscTip, F_H_RED);

}

void SelectMapScence::DestoryScence()
{
	//让键盘不再通知
	UnRegKeyBoardNotify();
	//清理缓冲区
	ConsoleUIBuff::GetInstance()->CleanBuff();
	//调用父类通用清理函数
	__super::DestoryScence();

}


void SelectMapScence::RcvKeyBoardNotify(char chKey)
{
	switch (chKey)
	{
	case E_KEY_UP:case E_KEY_W:case E_KEY_w:
	{

					  EraseOldSelct();
					  int nNewOffset = (m_nOffset - 1 + m_vecFiles.size()) % m_vecFiles.size();
					  DrawSelect(nNewOffset);
					  m_nOffset = nNewOffset;
					  break;
	}

	case E_KEY_DOWN:case E_KEY_s:case E_KEY_S:
	{
						EraseOldSelct();
						int nNewOffset = (m_nOffset + 1 + m_vecFiles.size()) % m_vecFiles.size();
						DrawSelect(nNewOffset);
						m_nOffset = nNewOffset;
						break;
	}
	case E_KEY_ENTER:
	{
						if (m_vecFiles.size() > 0)
						{
							s_strMapName = m_vecFiles[m_nOffset];
						}
						//获取导演类
						Director *pDirector = Director::GetInstance();
						MainScence *pMainScence = new MainScence();
						pDirector->ChangeScence(pMainScence);
						break;
	}
	case E_KEY_ESC:
	{
					  //获取导演类
					  Director *pDirector = Director::GetInstance();
					  //开启游戏主菜单
					  MenuScence *pMainMenu = new MenuScence();
					  pDirector->ChangeScence(pMainMenu);
					  break;
	}
	default:
		break;
	}
}

void SelectMapScence::EraseOldSelct()
{
	ConsoleUIBuff::GetInstance()->DrawImme(c_nFileNameBaseX, c_nFileNameBaseY + m_nOffset, "  ", 0);
}

void SelectMapScence::DrawSelect(int nOffset)
{
	ConsoleUIBuff::GetInstance()->DrawImme(c_nFileNameBaseX, c_nFileNameBaseY + nOffset, (char*)c_pszArrow, F_H_YELLOW);
}

std::string SelectMapScence::GetMapName()
{
	return s_strMapName;
}

