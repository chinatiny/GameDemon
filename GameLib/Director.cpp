#include "Director.h"


Director* Director::s_pDirector = nullptr;

Director::Director()
{
	m_bScencePase = false;
}

Director* Director::GetInstance()
{
	if (!s_pDirector)
	{
		s_pDirector = new Director();
	}
	return s_pDirector;
}

void Director::Destory()
{
	if (!s_pDirector)
	{
		delete s_pDirector;
		s_pDirector = nullptr;
	}
}


void Director::FistRunScence(Scence* pScence)
{
	//设置上个scence
	m_pNextScence = pScence;
	//设置为当前scence
	m_pCurrentScence = pScence;
	//初始化场景
	m_pCurrentScence->InitScence();
	//处理加载资源到到界面显示中间的黑屏问题
	m_pCurrentScence->BeforEnterScence();
	//和定时器结合
	m_pCurrentScence->AttachRunScence();
}

void Director::DirectorCallBack()
{
	if (s_pDirector->m_pCurrentScence != s_pDirector->m_pNextScence)  //发现当前执行的场景和下次不一样，这是最后一次执行当前场景也是新场景第一次执行（清理数据）
	{
		//把旧的场景和定时器分离
		s_pDirector->m_pCurrentScence->DetachRunScence();
		//设置当前场景已经结束
		s_pDirector->m_pCurrentScence->SetIsEnd(true);
		//旧的场景进行destory
		s_pDirector->m_pCurrentScence->DestoryScence();
		delete s_pDirector->m_pCurrentScence;
		s_pDirector->m_pCurrentScence = nullptr;
		//把当前的场景设置为下次的
		s_pDirector->m_pCurrentScence = s_pDirector->m_pNextScence;

		//新的场景和定时器先分离
		s_pDirector->m_pCurrentScence->DetachRunScence();
		//新的场景初始化
		s_pDirector->m_pCurrentScence->InitScence();
		//设置当前场景已经结束为false
		s_pDirector->m_pCurrentScence->SetIsEnd(false);
		//处理加载资源到到界面显示中间的黑屏问题
		s_pDirector->m_pCurrentScence->BeforEnterScence();
		//和定时器结合
		s_pDirector->m_pCurrentScence->AttachRunScence();
	}
	else
	{
		if (!(s_pDirector->m_bScencePase))
		{
			s_pDirector->m_pCurrentScence->RunScence();
		}
	}
}


Scence* Director::GetRuningScence()
{
	return m_pCurrentScence;
}

void Director::ChangeScence(Scence* pScence)
{
	//设置好下次加载的场景
	m_pNextScence = pScence;
}

void Director::ScencePasue()
{
	m_bScencePase = true;
}

void Director::ScenceResume()
{
	m_bScencePase = false;
}
