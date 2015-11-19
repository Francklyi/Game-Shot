#pragma once
#include"Entity.h"

class MShape:public Entity
{
public:
	Entity * pEntity;//所属的Graph
	int shapeType; //shape type : 0: BOX, 1: SPHERE
	float mass;
	float restn;//弹性

	MShape();

	~MShape();

	void setGraph(Entity *);//设置所属的Graph，以传递变换矩阵

	void draw();

	void setShapeType(int ST_){ shapeType= ST_; }

	void setMass(float m_){ mass = m_; }

	void setRestn(float restn_){ restn = restn_; }

	void copy(MShape* pMShape);
public:
	void operator = (const MShape& mShape_);
};
