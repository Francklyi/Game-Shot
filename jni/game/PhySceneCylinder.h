#pragma once
#include "PhyScene.h"

class PhySceneCylinder : public PhyScene
{
public:
	float posOrgX;
	float posOrgY;
	float posOrgZ;

	float posNowX;
	float posNowY;
	float posNowZ;

	float R;
	float Rd2;

public:
	PhySceneCylinder();

	PhySceneCylinder(float R_, float h);

	~PhySceneCylinder();

	void setR(float R_);

	virtual void calculate(float *nowPos, float *nextPos, float h1);

	//bool hasTouch(float posX,float posY,float poxZ);

	virtual bool hasTouch(float *nowPos, float *nextPos);

	/* almost using in nowPos */
	virtual bool isInside(float *pos, float h_);

	virtual void addNewPos(float x, float y, float z);

	void setCenterPos(float x, float y, float z);

};
