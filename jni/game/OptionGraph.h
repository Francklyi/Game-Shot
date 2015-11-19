#pragma once
#include "graphic/Graph.h"

class OptionGraph : public Graph
{
public:
	float *changeMatrix;
	int pointCenter[2],pointSide1[2];
	float R;
public:
	OptionGraph();

	void setChangeMatrix(float *pCMatrix);

	void setData();

	bool isTouch(float x, float y);



};
