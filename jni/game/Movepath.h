#pragma once
#include "Entity.h"
#include "Movepoint.h"
#include <vector>
using std::vector;


class Movepath
{
public:
	float *pfMoveSteps;

	vector<Movepoint*> vMPs;

	bool isMoving;

	static float *pfMoveZero;

public:
	Movepath();
	~Movepath();

	void addMovepoint(Movepoint *pMP);

	void deleteMovepoint(Movepoint *pMP);

	void deleteMovepoint(int num);

	virtual void initMovepath();

	virtual float *getMovestep();

	virtual void setSpecData(float *pData);

};
