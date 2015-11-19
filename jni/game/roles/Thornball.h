#pragma once
#include "graphic/Graph.h"
#include "roles/LeadRole.h"

class Thornball : public Graph
{
public:

	int theta;
	float R;
	float px,py,pz;
	float posMatrix[16];

	float *pos0;
	float *pos1;

	Thornball();
	~Thornball();

	void reset();

	void update(LeadRole *pg);

	void updateLess();

	void setPos(float x, float y, float z);
	bool isTouch(LeadRole *pg);

	virtual void updateAfterTransform();

	virtual void updateAfterDraw();

};
