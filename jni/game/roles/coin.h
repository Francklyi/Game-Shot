#pragma once
#include "graphic/Graph.h"
#include "roles/LeadRole.h"

class Coin : public Graph
{
public:

	int theta;
	float R;
	float px,py,pz;
	float posMatrix[16];
	bool visable;
	bool isClose;
	float *pos0;
	float *pos1;

	static int coinSum;

	Coin();
	~Coin();

	void reset();

	void update(LeadRole *pg);

	void updateLess();

	void setPos(float x, float y, float z);
	void setVisable(bool visable_);
	bool isVisable();
	bool isCapture(LeadRole *pg);
	bool getIsClose(LeadRole *pg);
	double random(int seed,double start, double end);

	virtual void updateAfterTransform();

	virtual void updateAfterDraw();

};
