#pragma once
#include "graphic/Graph.h"
#include "roles/LeadRole.h"

class Thorn : public Graph
{
public:

	float xMin,xMax;
	float yMin,yMax;
	float zMin,zMax;

	Thorn();
	~Thorn();

	void reset();

	void update(LeadRole *pLR);

	void updateLess();

	void updateUnderFloor();

	virtual void updateAfterTransform();

	virtual void updateAfterDraw();

	void setRange();

	bool isTouch(float x, float y, float z);

	virtual void move(float x, float y, float z);
};
