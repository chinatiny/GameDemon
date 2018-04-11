#pragma once
//键盘监控事件回调的纯虚函数


class IKeyboardNotify
{
public:
	virtual void RcvKeyBoardNotify(char chKey) = 0;
	virtual void RegKeyBoardNotify() = 0;
	virtual void UnRegKeyBoardNotify() = 0;
};

