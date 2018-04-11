#include "AStar.h"
#include "ConsoleUIBuff.h"
#include "ColorDefine.h"

AStar* AStar::s_pAStart = nullptr;
AStar::AStar(){}
AStar::~AStar(){}

bool AStar::FindPath()
{
	//如果是相同点不用寻路
	if (m_startPos.m_gamePos.nX== m_targetPos.m_gamePos.nX && m_startPos.m_gamePos.nY == m_targetPos.m_gamePos.nY)
	{
		return false;
	}
	//处理开始坐标
	AstartNode nodeInfo;
	nodeInfo.curPos = m_startPos;
	nodeInfo.g = 0;
	nodeInfo.SetH_F(m_startPos, m_targetPos);
	//把起点添加到open表
	m_vecOpen.push_back(nodeInfo);
	//
	m_setOpen.insert(nodeInfo);
	//ConsoleUIBuff::GetInstance()->DrawImme(nodeInfo.curPos.m_gamePos.nX, nodeInfo.curPos.m_gamePos.nY, "○", F_GREEN);
	int nCount = 0;
	//开始寻路
	while (true)
	{
		++nCount;
		//如果open表中没有待检测的点说明没找到
		if (m_vecOpen.empty())
		{
			return false;
		}
		if (nCount >= c_nMaxAstarStep)
		{
			return false;
		}
		//从open表中找到f值最小的
		int nIndex = 0;
		for (size_t i = 1; i < m_vecOpen.size(); i++)
		{
			if (m_vecOpen[nIndex].f > m_vecOpen[i].f)
			{
				nIndex = i;
			}
		}
		//
		AstartNode newNode[4];
		//上
		newNode[0].curPos.m_gamePos.nX = m_vecOpen[nIndex].curPos.m_gamePos.nX;
		newNode[0].curPos.m_gamePos.nY = m_vecOpen[nIndex].curPos.m_gamePos.nY - 1;
		//下
		newNode[1].curPos.m_gamePos.nX = m_vecOpen[nIndex].curPos.m_gamePos.nX;
		newNode[1].curPos.m_gamePos.nY = m_vecOpen[nIndex].curPos.m_gamePos.nY + 1;
		//左
		newNode[2].curPos.m_gamePos.nX = m_vecOpen[nIndex].curPos.m_gamePos.nX - 1;
		newNode[2].curPos.m_gamePos.nY = m_vecOpen[nIndex].curPos.m_gamePos.nY;
		//右
		newNode[3].curPos.m_gamePos.nX = m_vecOpen[nIndex].curPos.m_gamePos.nX + 1;
		newNode[3].curPos.m_gamePos.nY = m_vecOpen[nIndex].curPos.m_gamePos.nY;

		//把发源点放到close集合
		AstartNode beginNode = m_vecOpen[nIndex];
		m_vecClose.push_back(beginNode);
		//ConsoleUIBuff::GetInstance()->DrawImme(beginNode.curPos.m_gamePos.nX, beginNode.curPos.m_gamePos.nY, "■", F_RED);
		//
		m_setClose.insert(beginNode);
		//将其他属性赋值
		for (int i = 0; i < 4; i++)
		{
			newNode[i].nParentIndex = (int)m_vecClose.size() -1;   //设置父坐标在close集合的索引
			newNode[i].g = m_vecOpen[nIndex].g + 1;                              //移动一步加1
			newNode[i].SetH_F(newNode[i].curPos, m_targetPos); //设置h和f的值
		}

		//把发源点从open表中删除
		m_vecOpen.erase(m_vecOpen.begin() + nIndex);
		//
		m_setOpen.erase(beginNode);

		//检测扩散出的点是否合格
		for (int i = 0; i < 4; i++)
		{
			//如果是终点，添加到close表中后函数返回
			if (newNode[i].curPos == m_targetPos)
			{
				m_vecClose.push_back(newNode[i]);
				return true;
			}

			//对象的检测
			bool bSpiritCheck = m_pCheckObj->AStartCheck(m_uTargetID, m_vecClose[newNode[i].nParentIndex].curPos.m_gamePos, newNode[i].curPos.m_gamePos);
			if (!bSpiritCheck)
			{
				continue;
			}

			//如果在open或者在close表中不要
			if (m_setOpen.end() != m_setOpen.find(newNode[i]))
				goto End;

			if (m_setClose.end() != m_setClose.find(newNode[i]))
				goto End;

			//如果没有被跳转添加到open表中
			//ConsoleUIBuff::GetInstance()->DrawImme(newNode[i].curPos.m_gamePos.nX, newNode[i].curPos.m_gamePos.nY, "○", F_GREEN);
			m_vecOpen.push_back(newNode[i]);
			m_setOpen.insert(newNode[i]);
		End:;
		}
	}
	return false;
}

void AStar::GetPath(std::vector<GamePos> &vecPath)
{
	//取结束点为当前节点
	int nCurrentIndex = m_vecClose.size() - 1;
	//把当前点放入集合
	m_Path.push_back(m_vecClose[nCurrentIndex].curPos.m_gamePos);
	//当前点的父节点
	int nParentIndex = m_vecClose[nCurrentIndex].nParentIndex;
	int nCount = 0;
	while (true)
	{
		++nCount;
		//如果当前节点的父节点为开始点，塞入路径中，直接退出
		if (m_vecClose[nParentIndex].curPos == m_startPos || nCount >= c_nMaxAstarStep)
		{
			m_Path.push_back(m_startPos.m_gamePos);
			break;
		}
		else
		{
			//父节点作为新的当前节点
			nCurrentIndex = nParentIndex;
			int nX = m_vecClose[nCurrentIndex].curPos.m_gamePos.nX;
			int nY = m_vecClose[nCurrentIndex].curPos.m_gamePos.nY;
			m_Path.push_back(m_vecClose[nCurrentIndex].curPos.m_gamePos);
			//再计算出当前节点的父亲节点
			nParentIndex = m_vecClose[nCurrentIndex].nParentIndex;
		}
	}
	//
	for (int i = m_Path.size() - 1; i >= 0; i--)
	{
		vecPath.push_back(m_Path[i]);
	}
}

void AStar::Init(unsigned int uTargetID, GamePos &startPos, GamePos &targetPos, IAstartCheck *pCheckObj)
{
	//清理掉几个数据结构
	m_setClose.clear();
	m_setOpen.clear();
	m_vecClose.clear();
	m_vecOpen.clear();
	m_Path.clear();
	m_uTargetID = 0;
	//初始化
	AStartPos astartStart;
	astartStart.m_gamePos = startPos;
	AStartPos astartTarge;
	astartTarge.m_gamePos = targetPos;
	//
	m_startPos = astartStart;
	m_targetPos = astartTarge;
	m_pCheckObj = pCheckObj;
	//
	m_uTargetID = uTargetID;
}

AStar* AStar::GetInstance(unsigned int uTargetID, GamePos &startPos, GamePos &targetPos, IAstartCheck *pCheckObj)
{
	if (nullptr == s_pAStart)
	{
		s_pAStart = new AStar();
	}
	s_pAStart->Init(uTargetID, startPos, targetPos, pCheckObj);
	return s_pAStart;
}

void AStar::DestoryInstance()
{
	if (s_pAStart)
	{
		delete s_pAStart;
		s_pAStart = nullptr;
	}
}


