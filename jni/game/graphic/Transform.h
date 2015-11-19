#pragma once
#include "OMatrix.h"
/************************************************************************/
/* a set of translate , rotate, scale                                                              */
/************************************************************************/
class Transform
{
public:
	OMatrix *pTOmatrix; //存储变换矩阵的对象
	OMatrix *pROmatrix; //rotate Matrix
	OMatrix *pSOmatrix; //scale Matrix

	Transform();
	~Transform();

	void copy(Transform*);

	void scale(float x, float y, float z);

	void translate(float x, float y, float z);

	void rotate(float a, float x, float y, float z);

};
