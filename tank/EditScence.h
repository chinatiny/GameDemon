#pragma once
#include "Scence.h"
#include "windows.h"
#include <vector>
#include "Map.h"
#include "Tank.h"


class EditScence :
	public Scence
{
public:
	typedef enum
	{
		E_EDIT_SELECT_WALL,  //土墙
		E_EDIT_SELECT_WATER, //水
		E_EDIT_GRASS,  //草
		E_EDIT_IRON, //铁块
		E_EDIT_SAVE,
		E_EDIT_EXIT,
		E_EDIT_ERASE,
		E_EDIT_MAX
	}EditSelect;

	typedef struct
	{
		int nX;
		int nY;
		EWallType eWalltype;
	}EditNode;
public:
	EditScence();
	~EditScence();

	virtual void InitScence();
	virtual void DestoryScence();
	virtual void BeforEnterScence();
	virtual void  RunScence();

private:
	void AddEditNode(int nX, int nY, EWallType eWallType);
	void DeleteEditNode(int nX, int nY);
	void SaveToFile();
	//判断一个点是否被覆盖了
	bool IsHasDraw(int nX, int nY);

private:
	HANDLE m_hStdIn;
	EditSelect m_eEditSelect;
	std::vector<EditNode> m_vecEditNodes;
	DWORD m_dwOldConsoleMode;
	Tank *m_pLeft; 
	Tank *m_pRight;
	std::vector<GamePos> m_vecUsePos;//已经被覆盖的点
};

