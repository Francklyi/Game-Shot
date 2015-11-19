#include "AppScene.h"

bool AppScene::isRunning=true;

int AppScene::gameLevelRecord=0;

int AppScene::gameLevelNow=0;

int AppScene::gameMode=AppScene::GAMEMOVE_LEVEL;

AppScene::AppScene()
{
	firstTouchLast=Vector3();
	firstTouchNow=Vector3();
	secorndTouchNow=Vector3();
	secorndTouchLast=Vector3();

}
