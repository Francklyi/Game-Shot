#pragma once
#include "PhyScene.h"
#include "PhyPlane.h"
#include <vector>

using std::vector;

class PhySceneQuad : public PhyScene
{
public:

	float *pfPosOrig;

	float *pfPosNow;

	vector<PhyPlane *> vPlanes;

	int fsides[4];
public:
	PhySceneQuad();

	void setPlane(float *pfs);

	void resetPlane(float *pfs);

	virtual void calculate(float *nowPos, float *nextPos, float h1);

	//bool hasTouch(float posX,float posY,float poxZ);

	virtual bool hasTouch(float *nowPos, float *nextPos);

	/* almost using in nowPos */
	virtual bool isInside(float *pos, float h_);

	virtual void addNewPos(float x, float y, float z);

};
