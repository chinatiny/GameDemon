#include "Keyboard.h"
#include <conio.h>

Keyboard* Keyboard::s_pKeyboard = nullptr;

Keyboard* Keyboard::GetInstance()
{
	if (!s_pKeyboard)
	{
		s_pKeyboard = new Keyboard();
	}
	return s_pKeyboard;
}

void Keyboard::Destory()
{
	if (s_pKeyboard)
	{
		delete s_pKeyboard;
		s_pKeyboard = nullptr;
	}
}

void Keyboard::KeyboardCallBack()
{
	if (_kbhit())
	{
		char chWhatEnterKey;
		chWhatEnterKey = _getch();
		/*
		from MSDN:
		When reading a function key or an arrow key, each function must be called twice;
		the first call returns 0 or 0xE0, and the second call returns the actual key
		code.
		*/
		if ((char)0xE0 == chWhatEnterKey) return;  //原因上面写的有
		GetInstance()->DispatchEnterKey(chWhatEnterKey);
	}
}

void Keyboard::DispatchEnterKey(char chKey)
{
		for (auto i = 0; i < c_nMaxNotifyNum; i++)
		{
			if (nullptr != pNotifyArry[i])
			{
				pNotifyArry[i]->RcvKeyBoardNotify(chKey);
			}
		}
}

void Keyboard::RegKeyboardMonitor(IKeyboardNotify* pNotify)
{
	for (auto i = 0; i < c_nMaxNotifyNum; i++)
	{
		if (nullptr == pNotifyArry[i])
		{
			pNotifyArry[i] = pNotify;
			break;
		}
	}
}

void Keyboard::UnRegKeyboardMonitor(IKeyboardNotify* pNotify)
{

	for (auto i = 0; i < c_nMaxNotifyNum; i++)
	{
		if (pNotify == pNotifyArry[i])
		{
			pNotifyArry[i] = nullptr;
		}
	}
}
