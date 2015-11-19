#pragma once
#include "Entity.h"

class Movepoint : public Entity
{
public:
	int stepSum;

	int step;

	float moveSteps[3];

	float moveStepsOut[3];

	Movepoint(void);

	~Movepoint(void);

	Entity * pEntity;//所属的Graph

	int shapeType; //shape type : 0: BOX, 1: SPHERE

	int moveType;

	int frameSum;

	int delay;// num of frame

	int delayNow;// delay frame now

	enum MoveType{
		MOVE_PERSISTENT,MOVE_TRIGGER
	};

	int movePathType;

	int direct;

	int offsetFrame;

	int offsetFNow;

	enum MovePathType{
		PATH_LINE, PATH_CIRCLE, PATH_NO1, PATH_NO2
	};

	enum MoveDirect{
		DIRECT_CB, DIRECT_CCB
	};

	int numCirclePoints;

	void setGraph(Entity *);//设置所属的Graph，以传递变换矩阵

	void setShapeType(int ST_);

	void setStepSum(int sum);

	void initSteps(float beginX, float beginY, float beginZ);

	float *getSteps();
};
