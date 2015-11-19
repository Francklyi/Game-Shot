#pragma once
#include "GLSL/matrix.h"
#include <vector>
using std::vector;
/************************************************************************/
/* object Matrix class                                                                     */
/************************************************************************/
class OMatrix
{
public:
	vector<float *> mMatrixQueue; //

	void scale(float x, float y, float z);

	void translate(float x, float y, float z);

	void rotate(float a, float x, float y, float z);

	void multiplyMM(float *m_);

	void multiplyMV(float *v_);

	void popMatrix();

	void pushMatrix();

	void identity(float *);

	void copy(OMatrix* pOMatrix);

	void copy(float *pf16);

	OMatrix();
	~OMatrix();
};
