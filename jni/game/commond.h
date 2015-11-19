#pragma once

#include <jni.h>
#include "game.h"
#include "AppScene.h"
#include "SetScene.h"
#include "RenderManager.h"

extern JNIEnv *mEnv;

extern Game *pGame;

extern AppScene *pAppScene;

extern SetScene *pSetScene;

extern int SysSceneType;

enum{
	SCENETYPE_LEVELS=1,
	SCENETYPE_GAME
};

void initEngine();
