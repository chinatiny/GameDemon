#include "GameApp.h"
#include "CommTool.h"
#include "TimeWheel.h"
#include "Director.h"
#include "Keyboard.h"

GameApp::GameApp()
{
	m_bPause = false;
	m_pDirector = nullptr;
}
std::string GameApp::GetGameExeDir()
{
	if (m_strGameDir.empty())
	{
		GetExeRootPath(m_strGameDir);
	}
	return m_strGameDir;

}

void GameApp::StopGame()
{
	TimeWheel::ShutDown();
}

void GameApp::GameLoop(Director *pDirector)
{
	m_pDirector = pDirector;
	//注册导演定时回调，用来发布导演命令
	TimeWheel::RegRunFunc(Director::DirectorCallBack, Director::c_nDefaultCallBackIntervalTickets);
	//注册键盘回调，键盘是导演的传声筒
	TimeWheel::RegRunFunc(Keyboard::KeyboardCallBack, Keyboard::c_nDefaultCallBackIntervalTickets);
	//开启整个世界
	TimeWheel::Loop();
}

void GameApp::GamePause()
{
	m_bPause = true;
	m_pDirector->ScencePasue();
}

void GameApp::GameResume()
{
	if (m_bPause)
	{
		TimeWheel::Chase();
		m_pDirector->ScenceResume();
	}
	m_bPause = false;
}


