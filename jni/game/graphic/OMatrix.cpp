#include "OMatrix.h"
#include <stdlib.h>

OMatrix::OMatrix(){
	mMatrixQueue=vector<float *>(1);
	mMatrixQueue[0]=(float*)malloc(sizeof(float)*16);
	identity(mMatrixQueue[0]);
}

OMatrix::~OMatrix(){
}


void OMatrix::scale(float x, float y, float z)
{
	getMatrix().scaleM(mMatrixQueue[0],0, x, y, z);
}

void OMatrix::translate(float x, float y, float z)
{
	getMatrix().translateM(mMatrixQueue[0],0, x, y, z);
}

void OMatrix::rotate(float a, float x, float y, float z)
{
	getMatrix().rotateM(mMatrixQueue[0],0, a,x, y, z);
}

void OMatrix::multiplyMM(float *m_)
{

}

void OMatrix::multiplyMV(float *v_)
{
	getMatrix().multiplyMV(v_,0, mMatrixQueue.back(), 0,v_, 0);
}

void OMatrix::popMatrix()
{
	if(mMatrixQueue.size()>=2)
	{
		delete mMatrixQueue[mMatrixQueue.size()-1];
		mMatrixQueue.pop_back();
	}
}

void OMatrix::pushMatrix()
{
	float *matrix_now=(float*)malloc(sizeof(float)*16);
	getMatrix().arrayCopy(mMatrixQueue[mMatrixQueue.size()-1], 0, matrix_now, 0, 16);
	mMatrixQueue.push_back(matrix_now);
}

void OMatrix::identity(float *pM)
{
	getMatrix().setIdentityM(pM,0);
}

void OMatrix::copy(OMatrix* pOMatrix)
{
	float *matrix_now=(float*)malloc(sizeof(float)*16);
	getMatrix().arrayCopy(pOMatrix->mMatrixQueue[mMatrixQueue.size()-1], 0, matrix_now, 0, 16);
	mMatrixQueue.push_back(matrix_now);
}

void OMatrix::copy(float *pf16){
	float *matrix_now=(float*)malloc(sizeof(float)*16);
	getMatrix().arrayCopy(pf16, 0, matrix_now, 0, 16);
	mMatrixQueue.push_back(matrix_now);
}
