#pragma once
#include "Frame.h"
#include "roles/movement.h"
#include "SysTime.h"

class AnimForGraph
{
public:
	int code;// same as code in movement

	int frame;

	int lastFrame;

	char change_stay;

	Frame *pFrames;

	Movement* pMovement;

	Animation *pA;
	//the start time of the movement
	long startTime;

	int startFrame;

	timeval t_start,t_now;

	int lastKeyFrame;

	int nextKeyFrame;

	int frameInteval;

	AnimForGraph();

	void setMovement(Movement* pMovement_);

	void setPFrames(Frame* pFs);

	void startFrameNum(int frameNum_);

	int getFrameNum();

	void end();

	void setKeyFrame(int *keyFrame_);
};
