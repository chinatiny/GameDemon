#pragma  once
#include "Common.h"
#include "Map.h"
#include "MainScence.h"
#include <vector>
#include "IAStartCheck.h"


class Tank :public IAstartCheck
{
public:
	typedef struct
	{
		unsigned int  uID; //唯一ID
		int nCenterX; //中心x坐标
		int nCenterY; //中心y坐标
		EWallType eWalltype; //身体类型
		EDir eDir; //坦克的方向
		char szPic[PIC_LEN]; //图片类型
		int nColor; //颜色
		EFightCamp eCamp; //阵营
		int nHP; //血值
		int nSpeed;//速度
		WalkPath walkPath;
		int nAstarNiceValue;

	}TankData;
	const static int c_nTankHeight = 3; //坦克的高度 
	const static int c_nTankWidth = 3;  //坦克的宽度
	const static int c_nTankNiceValue = 1000; //默认的nice值
	const static int c_nArryTankShape[E_DIR_MAX][c_nTankHeight][c_nTankWidth];
public:
	Tank(MainScence *pScence, int nCenterX, int nCenterY, EWallType eWallType, EDir eDir, const char *pszPic, int nColor, EFightCamp eCamp, int nHp, int nSpeed);
	~Tank();
	virtual bool AStartCheck(unsigned int uTargetID, GamePos &preGamePos, GamePos &nowGamePos); //A*寻路检测当前点
	void ReduceAstarNiceValue(); //因为A*失败减少nice值
	void ResetAstarFailTimes();//重置A*寻路失败次数
	void SetAstarPath(std::vector<GamePos> &vecPath);
	void MoveDir(EDir eDir); //向指定方向移动
	void ShowTank();//在场景中展示自己
	void EraseTank();//在场景中删除自己
	bool BeShot(GamePos &gamePos, EFightCamp eBulletCamp, EHeroNum eFromHero);  //被打
	TankData GetTankData(); //获取坦克数据
	static void BodayPos2RealPos(TankData &tankData, std::vector<GamePos> &vecPos); //获取坦克所有身体的坐标对应的实际坐标
	static void BodayPos2RealPos(GamePos &centerPos, EDir eDir, std::vector<GamePos> &vecPos); //获取坦克所有身体的坐标对应的实际坐标
	static inline void BodayPos2RealPos(GamePos &centerPos, GamePos bodayPos, GamePos &realPos); //身体坐标转实际坐标
	void Fire(EHeroNum eHeroNum);   //开火
	void Action();  //行为
	void Walk(); //行走



private:
	bool IsTankCanStep(TankData &tankData);//判断当前坦克是否会卡在墙里面
	bool IsTankCanStep(TankData &tankData, unsigned int uIgnoreID); //忽略某个ID
	void UpdateTankDataByDir(TankData &tankData);
	bool IsBeShot(GamePos &gamePos, EFightCamp eBulletCamp); //判断是否被打中
	void  RunPath(); //按照路径行走
	void RunPathJustChangeDir(EDir edir);  //仅仅需要转向
	void RunPathImee(); //直接走
	void  RandomMove(); //随机移动
	void  RandomFire(); //随机开火
	EDir GetDirByPosOffset(int nTargetX, int nTargetY, int nSrcX, int nSrcY);

private:
	TankData m_tankData;
	MainScence *m_pMainScence;
	//
	unsigned __int64 m_u64Tickets;
	unsigned __int64 m_u64NextTicket;
};


