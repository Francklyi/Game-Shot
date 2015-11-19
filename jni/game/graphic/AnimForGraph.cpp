#include "AnimForGraph.h"
#include "log.h"
#include "SysTime.h"

AnimForGraph::AnimForGraph()
:frame(0)
,lastFrame(0)
,pFrames(0)
,pMovement(0)
,pA(0)
,code(0)
,startTime(0)
,startFrame(0)
,lastKeyFrame(0)
,nextKeyFrame(0)
,frameInteval(0)
{

}

void AnimForGraph::setMovement(Movement* pMovement_)
{
	pMovement=pMovement_;
	pA=pMovement->pA;
	pFrames=pA->pFrames;
	code=pMovement->code;

	if(pA->vKF.size()==0)
		return;

	lastKeyFrame=0;

	nextKeyFrame=1;
}

void AnimForGraph::setPFrames(Frame* pFs)
{
	pFrames=pFs;
}

void AnimForGraph::startFrameNum(int frameNum_)
{
	lastFrame=frame;
	frame=frameNum_;
	startFrame=frameNum_;
	//gettimeofday(&t_start, NULL);
	//startTime=t_start.tv_sec*1000000+ t_start.tv_usec;
	startTime=pSysTime->nowTime;
	frameInteval=0;
	if(pA->vKF.size()==0)
			return ;

	nextKeyFrame=pA->getKeyFrame(frame);
}

int AnimForGraph::getFrameNum()
{
	if(change_stay == 0)
		return 0;

	lastFrame=frame;
	//gettimeofday(&t_now, NULL);

	frameInteval=(pSysTime->nowTime-startTime)/25000;//>>15;///20834;
	//LOGE("frameInteval: %d",frameInteval);
	frame=startFrame+frameInteval;
	frameInteval=frame-lastFrame;

	if(pA->vKF.size()!=0)
	if(frame>=pA->vKF[nextKeyFrame]->numofFrame)
	{
		frame=pA->vKF[nextKeyFrame]->numofFrame;
		lastKeyFrame=nextKeyFrame;
		if(nextKeyFrame<(pA->vKF.size()-1) )
			nextKeyFrame++;
	}

	if(frame>(pA->frameNum-1))
	{
		//LOGI("end!!!!!!!!!!");
		end();
		return 0;
	}
	//LOGE("frame: %d",frame);
	return frame;
}

void AnimForGraph::end()
{
	frame=0;
	lastFrame=frame;
	startFrame=frame;
	change_stay = 0;
}
