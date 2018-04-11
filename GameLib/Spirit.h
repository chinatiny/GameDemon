#pragma once
#include "IKeyboardNotify.h"
#include "LibCommon.h"
class Scence;

class Spirit :public IKeyboardNotify
{
public:
	Spirit();
	~Spirit();
	virtual void InitSpirit(Scence *pScence);    //仅仅做数据初始化，在刚创建的时候用
	virtual void BeforEnterScence();  //在进入主场景之前,不准changescence
	virtual void BeforDestorySpirit();  //在销毁内存之前做的事情，不准changescence
	virtual void Display();

	//键盘相关
	virtual void RcvKeyBoardNotify(char chKey);
	void RegKeyBoardNotify();
	void UnRegKeyBoardNotify();
protected:
	Scence *m_pScence;
};

