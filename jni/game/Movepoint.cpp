#include "Movepoint.h"

Movepoint::Movepoint()
:shapeType(0)
,stepSum(1)
,step(0)
,moveType(Movepoint::MOVE_PERSISTENT)
,frameSum(200)
,delay(0)
,delayNow(0)
,movePathType(PATH_LINE)
,direct(DIRECT_CB)
,numCirclePoints(20)
,offsetFrame(0)
,offsetFNow(0)
,Entity()
{

}

Movepoint::~Movepoint()
{

}

void Movepoint::setGraph(Entity *pE)
{
	//设置所属的Graph，以传递变换矩阵
	pEntity=pE;
}

void Movepoint::setShapeType(int ST_)
{
	shapeType= ST_;
}

void Movepoint::setStepSum(int sum)
{
	stepSum=sum;
}

void Movepoint::initSteps(float beginX, float beginY, float beginZ)
{
	moveSteps[0]=(pTransform->pTOmatrix->mMatrixQueue.back()[12]-beginX)/frameSum;
	moveSteps[1]=(pTransform->pTOmatrix->mMatrixQueue.back()[13]-beginY)/frameSum;
	moveSteps[2]=(pTransform->pTOmatrix->mMatrixQueue.back()[14]-beginZ)/frameSum;
}

float *Movepoint::getSteps()
{
	if(offsetFNow<offsetFrame)
	{
		moveStepsOut[0]=0;
		moveStepsOut[1]=0;
		moveStepsOut[2]=0;
		offsetFNow++;
	}else
	if(step==0)
	{
		delayNow++;
		moveStepsOut[0]=0;
		moveStepsOut[1]=0;
		moveStepsOut[2]=0;
		if(delayNow>=delay)
		{
			delayNow=0;
			step++;
			moveStepsOut[0]=moveSteps[0];
			moveStepsOut[1]=moveSteps[1];
			moveStepsOut[2]=moveSteps[2];
		}

	}
	else
	{
		if(step>=frameSum)
		{
			step=-1;
			moveSteps[0]=-moveSteps[0];
			moveSteps[1]=-moveSteps[1];
			moveSteps[2]=-moveSteps[2];

			moveStepsOut[0]=moveSteps[0];
			moveStepsOut[1]=moveSteps[1];
			moveStepsOut[2]=moveSteps[2];
		}
		step++;
	}

	return moveStepsOut;
}
