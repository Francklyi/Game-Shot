#pragma once
#include "Movepath.h"

class MovepathCircle : public Movepath
{
public:
	int NumPoints;

	float *pfPoints;

	float *pfSteps;

	int stepNow;

	int frameNow;

	int framesInStep;

	MovepathCircle();

	~MovepathCircle();

	virtual void initMovepath();

	virtual float *getMovestep();

	virtual void setSpecData(float *pData);

};
