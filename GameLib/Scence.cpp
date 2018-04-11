#include "Scence.h"
#include "Keyboard.h"

void Scence::AddSpirit(Spirit* pSpirit)
{
	pSpirit->InitSpirit(this);
	pSpirit->BeforEnterScence();
	m_arrySpirit.push_back(pSpirit);
}

void Scence::DeleteSpirit(Spirit* pSpirit)
{
	std::vector<Spirit*> tmp;
	for (auto ite = m_arrySpirit.begin(); ite != m_arrySpirit.end(); ite++)
	{
		if (*ite != pSpirit)
		{
			tmp.push_back(*ite);
		}
		else
		{
			(*ite)->BeforDestorySpirit();
			delete (*ite);
		}
	}
	m_arrySpirit.clear();
	m_arrySpirit = tmp;
}

void Scence::MapSpirit(std::string strSpiritName, Spirit *pSpirit)
{
	m_spiritNameMap[strSpiritName] = pSpirit;
}
void Scence::UnMapSpirit(std::string strSpiritName)
{
	m_spiritNameMap.erase(strSpiritName);
}

Spirit* Scence::FindMap(std::string strSpiritName)
{
	auto ite = m_spiritNameMap.find(strSpiritName);

	if (ite != m_spiritNameMap.end())
	{
		return ite->second;
	}
	else
	{
		return nullptr;
	}
}

//有时间轮训来执行
void Scence::RunScence()
{
	if (!m_isRunscenceDetach)
	{
		BeforSpiritDisplay();
		for (auto ite = m_arrySpirit.begin();  ite < m_arrySpirit.end(); ite++)
		{
			(*ite)->Display();
			if (m_bIsEnd)
			{
				return;
			}
		}
		//在UI上展示演员的动作
		ShowScence();
		AfterSpiritDisplay();
	}
}

void ShowScence()
{

}

void Scence::InitScence()
{
}


void Scence::DestoryScence()
{
	for (auto ite = m_arrySpirit.begin(); ite < m_arrySpirit.end(); ite++)
	{
		(*ite)->BeforDestorySpirit();
		delete (*ite);
	}
	//m_arrySpirit.clear();
}

void Scence::DetachRunScence()
{
	m_isRunscenceDetach = true;
}

void Scence::AttachRunScence()
{
	m_isRunscenceDetach = false;
}

void Scence::BeforEnterScence()
{
}

void Scence::RcvKeyBoardNotify(char chKey)
{


}

void Scence::RegKeyBoardNotify()
{
	Keyboard* pKeyboard = Keyboard::GetInstance();
	if (pKeyboard)
	{
		pKeyboard->RegKeyboardMonitor(this);
	}
}

void Scence::UnRegKeyBoardNotify()
{
	Keyboard* pKeyboard = Keyboard::GetInstance();
	if (pKeyboard)
	{
		pKeyboard->UnRegKeyboardMonitor(this);
	}
}

void Scence::BeforSpiritDisplay()
{

}

void Scence::AfterSpiritDisplay()
{

}

void Scence::ShowScence()
{

}

void Scence::SetIsEnd(bool bFlag)
{
	m_bIsEnd = bFlag;
}

