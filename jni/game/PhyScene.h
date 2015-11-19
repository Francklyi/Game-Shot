#pragma once
#include "PhyPlane.h"
#include <vector>

using std::vector;

class PhyScene
{
public:
	float h;

	int collisionTime;

	int planeID;

	float myNextPos[3];
	float myNextPosTemp[3];

	int type;

	enum{ TYPE_NONE, TYPE_STAY, TYPE_CHANGE ,
		TYPE_ON_1, TYPE_ON_2, TYPE_ON_3, TYPE_ON_4};

	int shapeType;

	enum{
		SHAPE_QUAD, SHAPE_CIRCLE
	};

public:
	PhyScene();

	void setHeight(float h_);

	virtual void calculate(float *nowPos, float *nextPos, float h1);

	//bool hasTouch(float posX,float posY,float poxZ);

	virtual bool hasTouch(float *nowPos, float *nextPos);

	/* almost using in nowPos */
	virtual bool isInside(float *pos, float h_);

	void moveNoPhy(float x, float y, float z);

	virtual void addNewPos(float x, float y, float z);

};
