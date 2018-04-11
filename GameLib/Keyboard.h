#pragma once
#include <vector>

#include "IKeyboardNotify.h"

class Keyboard
{
private:
	Keyboard()
	{
		memset(pNotifyArry, 0, sizeof(pNotifyArry));
	}
public:
	const static int  c_nDefaultCallBackIntervalTickets = 20;  //默认的驱动回调Ms
public:
	static Keyboard* GetInstance();
	static void Destory();
	static void KeyboardCallBack();

	void RegKeyboardMonitor(IKeyboardNotify* pNotify);
	void UnRegKeyboardMonitor(IKeyboardNotify* pNotify);
	void DispatchEnterKey(char chKey);

private:
	static Keyboard* s_pKeyboard;
	const static int c_nMaxNotifyNum = 1024;  
	IKeyboardNotify* pNotifyArry[c_nMaxNotifyNum];  //最多支持1024个精灵，可以换动态数组实现
};

