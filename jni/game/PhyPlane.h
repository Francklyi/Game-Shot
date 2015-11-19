#pragma once

/**
 * 2D
 */

class PhyPlane
{
public:
	float h;
	float x1, y1, k;
	float x2, y2;
	float ex, ey;
	float m1, m2;

	PhyPlane();


	void setHeight(float h_);
	void setPointers( float x1_, float y1_, float x2_, float y2_);
	bool calculate(float *nowPos, float *nextPos);

	bool hasCollision(float *nowPos, float *nextPos);

	float getSide(float *pos);

};
