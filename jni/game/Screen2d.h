#pragma once
#include "GLSL/matrix.h"

class Screen2d
{
public:
	float pLeftUp[3];
	//float pRightUp[3];
	//float pLeftDown[3];
	//float pRightDown[3];

	float vectorE[3];

	Screen2d();

	void init();

	void getPos(int x, int y, float *pos);


};

extern Screen2d *pScreen2d;
