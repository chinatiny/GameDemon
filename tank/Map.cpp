#include "Map.h"
#include "ColorDefine.h"

char* g_WallPic[] =
{
	"  ",   // =>E_EMPTY
	"¡ô",   // =>E_BORDER
	"¡õ",   // =>E_TANK
	"¡Ö",  // =>E_WATER
	"¡ù",  // =>E_GRASS
	"¨ˆ",   // =>E_SOIL
	"¡ö",   // =>E_IRON
	"¡ð"    // =>E_WEAPON
};

RandomTank g_randomTank[] = 
{
	{F_WHITE, 1, "¡ô", 5},   // => E_TANK_TYPE1  
	{F_H_BLUE, 2, "¡ñ", 5},  // => E_TANK_TYPE2 
	{F_GREEN, 3, "¡Ñ", 4},   // => E_TANK_TYPE3
	{ F_H_PURPLE, 4, "¨’", 4},  // => E_TANK_TYPE4
	{ F_H_YELLOW, 5, "¡ï", 3},  // => E_TANK_TYPE5
};