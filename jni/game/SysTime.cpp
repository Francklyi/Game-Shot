#include "SysTime.h"

SysTime::SysTime()
:nowTime(0)
,lastTime(0)
,frameTime(0)
,frameInterval(0.0f)
{
	gettimeofday(&t_now, NULL);
	nowTime=t_now.tv_sec*1000000+ t_now.tv_usec;
	lastTime=nowTime;
}

void SysTime::renewTime()
{
	gettimeofday(&t_now, NULL);
	nowTime=t_now.tv_sec*1000000+ t_now.tv_usec;
	frameTime=nowTime-lastTime;
	frameInterval=((float)frameTime)/20834.0f;
	if(frameInterval<1) frameInterval=1;
	lastTime=nowTime;
}

SysTime *pSysTime=new SysTime();
