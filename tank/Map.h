#pragma  once
#include "LibCommon.h"
#include "Common.h"
#include <vector>


typedef enum
{
	E_EMPTY, //空地
	E_BORDER,   //边框
	E_TANK, //坦克
	E_WATER, //水
	E_GRASS, //草地
	E_SOIL, //土墙
	E_IRON,// 铁
	E_WEAPON, //子弹
	E_WALL_TYPE_MAX  //最大的无效值
}EWallType;

extern char* g_WallPic[];


//图片长度
#define  PIC_LEN 5

//场景单元信息
typedef struct
{
	unsigned int uID;
	char szPic[PIC_LEN];
	int nColor;
	EWallType eWalltype;
	EFightCamp eCamp;
}ScenceActor;

//固定场景ID
typedef enum
{
	E_ID_EMPTY,
	E_ID_SOIL,
	E_ID_WATER,
	E_ID_GRASS,
	E_ID_IRON,
	E_ID_BORDER,
	E_ID_OTHER,  //其他类型的ID从这里开始
	E_ID_INVALID = 0xFFFFFFFF
}E_FIX_ACTOR_ID;


//随机坦克结构
typedef enum
{
	E_TANK_TYPE1,
	E_TANK_TYPE2,
	E_TANK_TYPE3,
	E_TANK_TYPE4,
	E_TANK_TYPE5,
	E_TANK_MAX
}ERandomTanktype;
typedef struct  
{
	int nColor;
	int nHP;
	char szPic[PIC_LEN];
	int nSpeed;
}RandomTank;
extern RandomTank g_randomTank[];

//路径
#define  RUN_PATH_WAIT_TIMES 10  //如果遇到挡道的需要等待多久
typedef struct
{
	int nStep;  //第几步了
	int nWaitTimes; //在当前点等待几次（假设存在别的坦克或者子弹挡道了）
	std::vector<GamePos> vecWalkPath;
}WalkPath;