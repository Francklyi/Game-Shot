#pragma once
#include"line.h"
#include "roles/LeadRole.h"
class LeadHP
{
public:

	Vector3 p1;
	Vector3 p2;
	Vector3 pMid;

	LeadRole *pLR;

	Line line;

	float pColor[3];

	float vertics[13];

	float interval[3];

	float HPPercentage;

	unsigned short index[6];

	LeadHP();

	void init();

	void setPLR(LeadRole* pLR);

	void draw();

};
