#pragma once
#include "Camera.h"
#include "Vector3.h"

class AppScene
{
public:
	Camera *pCamera;
	Vector3 firstTouchLast;
	Vector3 firstTouchNow;

	Vector3 secorndTouchNow;
	Vector3 secorndTouchLast;

	static bool isRunning;

	static int gameLevelRecord;

	static int gameLevelNow;

	static int gameMode;

	enum{
		GAMEMOVE_LEVEL,
		GAMEMODE_DEMO
	};

public:
	AppScene();

	virtual void onCreate()=0;

	virtual void onSize(int w, int h)=0;

	virtual void onUpdate()=0;

	virtual void onTouchDown(float x, float z)=0;

	virtual void onTouchUp(float x, float z)=0;

	virtual void onTouchMove(float x, float z)=0;

	virtual void back()=0;

};
