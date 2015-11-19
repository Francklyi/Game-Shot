#pragma once
#include<unistd.h>

class SysTime
{
public:
	timeval t_now;
	long nowTime;
	long lastTime;
	long frameTime;
	float frameInterval;
public:
	SysTime();
	void renewTime();
};

extern SysTime *pSysTime;
