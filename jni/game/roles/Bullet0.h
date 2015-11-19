#pragma once
#include "graphic/Graph.h"

class Bullet0:public Graph{
public:
	float pfPos[3];
	float pfVer[3];


	Bullet0();

	void init(float *fpos, float *fver);

	virtual void update();
};
