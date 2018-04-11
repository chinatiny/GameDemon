#pragma once
#include "LibCommon.h"
#include <vector>
#include <set>
#include "IAStartCheck.h"
#include "Common.h"

class AStar
{
private:
	const static int c_nMaxAstarStep = 10000; //最多步数
	const static int c_nHeightArea = 3;
	const static int c_nWidthArea = 3;
	typedef struct AStartPos
	{
		bool operator==(const AStartPos& astarPos)
		{
			return astarPos.m_gamePos.nX == m_gamePos.nX && astarPos.m_gamePos.nY == m_gamePos.nY;
		}
		void operator=(const AStartPos& astarPos)
		{
			m_gamePos.nX = astarPos.m_gamePos.nX;
			m_gamePos.nY = astarPos.m_gamePos.nY;
		}
		GamePos m_gamePos;
	};
	//
	typedef struct AstartNode
	{
		AstartNode() :g(0), h(0), f(0){}
		int g; //移动损耗（每一动一步就加1）
		int h; //到终点的最短距离
		int f; //g + h
		void SetH_F(const AStartPos& startPos, const AStartPos& targetPos)
		{
			h = abs(startPos.m_gamePos.nX - targetPos.m_gamePos.nX) + abs(startPos.m_gamePos.nY - targetPos.m_gamePos.nY);
			f = g + h;
		}
		bool operator < (const AstartNode &a) const // 重载“<”操作符，自定义排序规则  
		{
			return (curPos.m_gamePos.nX + curPos.m_gamePos.nY * GAME_AREA_WIDTH) < (a.curPos.m_gamePos.nX + a.curPos.m_gamePos.nY * GAME_AREA_WIDTH);
		}
		//
		AStartPos curPos;
		int nParentIndex; //父节点在close集合的索引
	};

private:
	AStar(); //做成单例模式，避免频繁的创建导致的消耗
	~AStar();
public:
	static AStar* GetInstance(unsigned int uTargetID, GamePos &startPos, GamePos &targetPos, IAstartCheck *pCheckObj);
	static void DestoryInstance();
	bool FindPath();
	void GetPath(std::vector<GamePos> &vecPath);

private:
	void Init(unsigned int uTargetID, GamePos &startPos, GamePos &targetPos, IAstartCheck *pCheckObj);
	bool IsBoday(int nX, int nY);

private:
	AStartPos m_startPos;
	AStartPos m_targetPos;
	std::vector<AstartNode> m_vecOpen;  //存储待扩散的点（用来遍历）
	std::vector<AstartNode> m_vecClose; //存储扩散过的点(用来遍历)
	std::vector<GamePos> m_Path; //最短路径坐标
	IAstartCheck *m_pCheckObj;
	std::set<AstartNode> m_setOpen; //(用来查找)
	std::set<AstartNode> m_setClose; //(用来查找)
	int m_uTargetID;
	//
	static AStar* s_pAStart;



};

