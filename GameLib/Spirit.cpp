#include "Spirit.h"
#include "Keyboard.h"



Spirit::Spirit()
{
}


Spirit::~Spirit()
{
}

void Spirit::Display()
{

}

void Spirit::RcvKeyBoardNotify(char chKey)
{

}

void Spirit::RegKeyBoardNotify()
{
	Keyboard* pKeyboard = Keyboard::GetInstance();
	if (pKeyboard)
	{
		pKeyboard->RegKeyboardMonitor(this);
	}
}

void Spirit::UnRegKeyBoardNotify()
{
	Keyboard* pKeyboard = Keyboard::GetInstance();
	if (pKeyboard)
	{
		pKeyboard->UnRegKeyboardMonitor(this);
	}
}

void Spirit::InitSpirit(Scence *pScence)
{
	m_pScence = pScence;
}


void Spirit::BeforDestorySpirit()
{

}

void Spirit::BeforEnterScence()
{

}

