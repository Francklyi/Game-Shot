#pragma once
#include "graphic/Graph.h"
#include "graphic/GraphRace.h"

class Shadow : public Graph
{
public:
	float posX;
	float posY;
	float posZ;

	GraphRace* enemyRace;

	Graph *pLR;

	Shadow();

	virtual void updateAfterTransform();

	virtual void updateAfterDraw();

	void setPos(float x, float y, float z);

	void update();

	void setGraphRace( GraphRace * pGR);

	void setPLR(Graph * pLR_);

};
