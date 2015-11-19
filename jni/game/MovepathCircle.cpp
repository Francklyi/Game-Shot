#include "MovepathCircle.h"
#include "log.h"

MovepathCircle::MovepathCircle():
stepNow(0),
frameNow(0),
Movepath()
{
	pfPoints=0;
	NumPoints=40;

}

MovepathCircle::~MovepathCircle()
{

}

void MovepathCircle::initMovepath()
{
	pfSteps=new float[3*NumPoints];
	if(!pfPoints)
		return;
	framesInStep=(vMPs[0]->frameSum/NumPoints);
	if(vMPs[0]->direct==0)
	{
		pfSteps[0]=pfPoints[0]/framesInStep;
		pfSteps[1]=pfPoints[1]/framesInStep;
		pfSteps[2]=pfPoints[2]/framesInStep;
		//LOGI("NumPoints:%d",NumPoints);
		//LOGI("pfPoints:%f",pfSteps[1]);
		for (int i=1;i<NumPoints;i++)
		{
			pfSteps[i*3]=(pfPoints[i*3]-pfPoints[(i-1)*3])/framesInStep;
			pfSteps[i*3+1]=(pfPoints[i*3+1]-pfPoints[(i-1)*3+1])/framesInStep;
			pfSteps[i*3+2]=(pfPoints[i*3+2]-pfPoints[(i-1)*3+2])/framesInStep;
			//LOGI("pfSteps:%f,%f",pfPoints[i*3+1],pfPoints[(i-1)*3+1]);
		}
	}else
	{
		//LOGI("NumPoints:%d",NumPoints);
		//LOGI("pfPoints:%f",pfSteps[1]);
		int k=0;
		for (int i=NumPoints-1;i>0;i--)
		{
			pfSteps[k*3]=(pfPoints[(i-1)*3]-pfPoints[i*3])/framesInStep;
			pfSteps[k*3+1]=(pfPoints[(i-1)*3+1]-pfPoints[i*3+1])/framesInStep;
			pfSteps[k*3+2]=(pfPoints[(i-1)*3+2]-pfPoints[i*3+2])/framesInStep;
			//LOGI("pfSteps:%f,%f",pfPoints[i*3+1],pfPoints[(i-1)*3+1]);
			k++;
		}
		pfSteps[(NumPoints-1)*3]=-pfPoints[0]/framesInStep;
		pfSteps[(NumPoints-1)*3+1]=-pfPoints[1]/framesInStep;
		pfSteps[(NumPoints-1)*3+2]=-pfPoints[2]/framesInStep;
	}
}

void MovepathCircle::setSpecData(float *pData)
{
	pfPoints=pData;
}

float *MovepathCircle::getMovestep()
{
	if(vMPs.size()==0)
		return pfMoveZero;
	//LOGI("frameNow:%d,%d",frameNow,stepNow);
	frameNow++;
	if(frameNow>=framesInStep)
	{
		frameNow=0;
		stepNow++;
		if(stepNow>=NumPoints)
			stepNow=0;
	}
	return &(pfSteps[stepNow*3]);
}
