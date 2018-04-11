#include "TankGame.h"
#include "Map.h"

int main(int argc, char* argv[])
{
	TankGame* pTankGame = TankGame::GetGameInstance();    //获取游戏唯一实例

	pTankGame->DoBeforGameStart();  //游戏开始之前的全局初始化
	pTankGame->StartGame();  //游戏开始
	pTankGame->OnGameStop();  //在游戏结束时候做的善后工作

	TankGame::DestoryGameInstance(); //销毁掉游戏实例

	return 0;
}