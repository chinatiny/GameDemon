#pragma once
#include "Spirit.h"
#include <vector>
#include <map>
#include <string>



class Scence :  public IKeyboardNotify
{
public:
	Scence(){ m_isRunscenceDetach = true; } //默认定时器和场景是分离的
	~Scence(){}
	virtual void InitScence();    //仅仅做数据初始化，注意旧的场景还未切换！！！！
	virtual void DestoryScence();
	virtual void BeforEnterScence();  //在进入当前场景之前（旧的场景还未切换注意！！！）有段黑屏，这个回调是用来这里这个的！！！
	virtual void  RunScence();  //已经进入当前的场景了,定时回调RunScence
	virtual void  ShowScence(); //在UI上展示出来
	void  DetachRunScence(); //已经进入当前的场景了,不要定时回调RunScence
	void AttachRunScence(); //已经进入当前的场景了, 定时回调RunScence
	void AddSpirit(Spirit* pSpirit);
	void DeleteSpirit(Spirit* pSpirit);
	//scence 提供名字服务
	void MapSpirit(std::string strSpiritName, Spirit *pSpirit);
	void UnMapSpirit(std::string strSpiritName);
	Spirit* FindMap(std::string strSpiritName);

	virtual void BeforSpiritDisplay(); //在演员表演之前
	virtual void AfterSpiritDisplay(); //在演员表演之后

	//键盘的注册、反注册和接受
	virtual void RcvKeyBoardNotify(char chKey);
	void RegKeyBoardNotify();
	void UnRegKeyBoardNotify();

	//设置当前场景状态
	void SetIsEnd(bool bFlag);



private:
	std::vector<Spirit*> m_arrySpirit;
	bool m_isRunscenceDetach;  //是否和定时回调分离
	bool m_bIsEnd; //当前场景是否已经结束
	std::map<std::string, Spirit*> m_spiritNameMap; //场景的名字服务
};

