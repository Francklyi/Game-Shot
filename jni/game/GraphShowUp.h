#pragma once
#include "graphic/Graph.h"

class GraphShowUp : public Graph
{
public:
	float theta;

	float posX;
	float posY;
	float posZ;

	Graph *pGraph;

	GraphShowUp();

	void setPos(float x, float y, float z);

	void update();

	void setGraph(Graph * pGraph_);
};
