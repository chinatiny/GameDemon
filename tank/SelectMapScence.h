#pragma once
#include "Scence.h"
#include "EditScence.h"
#include <vector>
#include "SelectMapScence.h"
#include "Common.h"



class SelectMapScence :
	public Scence
{
public:
	SelectMapScence();
	~SelectMapScence();

	virtual void InitScence();
	virtual void DestoryScence();
	virtual void BeforEnterScence();

	//接受键盘回调
	virtual void RcvKeyBoardNotify(char chKey);

	//
	static std::string GetMapName();

private:
	void  EraseOldSelct();
	void DrawSelect(int m_nOffset);

public:
	static std::vector<EditScence::EditNode> s_vecEditNode;
	std::vector<std::string> m_vecFiles;
	int m_nOffset;

	static const int c_nFileNameBaseY = WINDOWS_HEIGHT / 3;
	static const int c_nFileNameBaseX = WINDOWS_WIDTH / 3;
	static const char* c_pszArrow;
	static const char* c_pszEnterTip;
	static const char* c_pszEscTip;

	static const int c_nEnterTipY = WINDOWS_HEIGHT - 3;
	static const int c_nEnterTipX = WINDOWS_WIDTH - 10;
	static const int c_nEscTipY = WINDOWS_HEIGHT - 2;
	static const int c_nEscTipX = WINDOWS_WIDTH - 10;

	//设置全局变量，用来给固定土墙加载数据
	static std::string s_strMapName;

};

