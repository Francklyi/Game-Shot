#pragma once
#include "graphic/Graph.h"

class PhyCylinder
{
public:
	float r;

	float *nowPos;

	float *nextPos;

	float *pos1;

	float ex, ey, temp, templ;


	Graph *pG;

	PhyCylinder();

	void setR(float r_);

	void setGraph(Graph *pG_);

	void interact(Graph *pG2);
};
