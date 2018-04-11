#pragma  once
#include "LibCommon.h"
#include <vector>

#define  WINDOWS_WIDTH	75  //窗体的宽
#define  WINDOWS_HEIGHT	 51 //窗体的高

#define  GAME_AREA_HEIGHT  51 //游戏区域的高度
#define  GAME_AREA_WIDTH   61//游戏区域的宽度
#define  GAME_NAME "坦克大战"

typedef enum
{
	E_DIR_UP,
	E_DIR_DOWN,
	E_DIR_LEFT,
	E_DIR_RIGHT,
	E_DIR_MAX
}EDir;   //方向

//战斗双方
typedef enum
{
	E_HERO_CAMP,  //友方
	E_ENEMY_CAMP, //敌方
	E_CAMP_MAX     //中立方
}EFightCamp;


//游戏模式
typedef enum
{
	E_LEFT_HERO, 
	E_RIGHT_HERO,  //双人模式
	E_HERO_MOD_MAX
}EHeroNum;

