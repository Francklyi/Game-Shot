#include "OptionGraph.h"
#include "GLSL/GLSL.h"
#include "log.h"

OptionGraph::OptionGraph()
:Graph()
{
	changeMatrix=0;
	pointCenter[0]=0;
	pointCenter[1]=0;
	pointSide1[0]=0;
	pointSide1[1]=0;
	R=100;
}

void OptionGraph::setChangeMatrix(float *pCMatrix)
{
	changeMatrix=pCMatrix;
}

void OptionGraph::setData()
{
	pfTemp4[0] = pTransform->pTOmatrix->mMatrixQueue.back()[12];
	pfTemp4[1] = pTransform->pTOmatrix->mMatrixQueue.back()[13];
	pfTemp4[2] = pTransform->pTOmatrix->mMatrixQueue.back()[14];
	pfTemp4[3] = 1.0f;

	getMatrix().multiplyMV(pfTemp4, 0, getGL().MVPMatrix, 0, pfTemp4, 0);

	if (pfTemp4[3] == 0.0f) return;

	R = 1.0f / pfTemp4[3];

	pointCenter[0] = getGL().viewport[0] + getGL().viewport[2]
			         * (pfTemp4[0] * R + 1.0f)* 0.5f;

	pointCenter[1] = getGL().viewport[1] + getGL().viewport[3]
			         * (pfTemp4[1] * R + 1.0f) * 0.5f;

	LOGI("x,y: %d, %d",	pointCenter[0],	pointCenter[1] );
	R=100;
	//pointCenter[2] = (pfTemp4[2] * rw + 1.0f) * 0.5f;
}

bool OptionGraph::isTouch(float x, float y)
{
	if( ( (x-pointCenter[0])*(x-pointCenter[0])+(y-pointCenter[1])*(y-pointCenter[1]) )<R*R )
	return true;

	return false;
}
