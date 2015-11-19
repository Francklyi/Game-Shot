#include "PhySceneCylinder.h"
#include <math.h>
#include "log.h"
#include "math/m_math.h"

PhySceneCylinder::PhySceneCylinder()
:posOrgX(0.0f),posOrgY(0.0f),posOrgZ(0.0f)
,R(0.0f)
,Rd2(0.0f)
,PhyScene()
{
	shapeType=SHAPE_CIRCLE;
}

PhySceneCylinder::PhySceneCylinder(float R_, float h_)
:posOrgX(0.0f),posOrgY(0.0f),posOrgZ(0.0f)
,PhyScene()
{
	R=R_-1.0;
	Rd2=R*R;
	h=h_;
	shapeType=SHAPE_CIRCLE;
}

PhySceneCylinder::~PhySceneCylinder()
{

}

void PhySceneCylinder::setR(float R_)
{

}

void PhySceneCylinder::calculate(float *nowPos, float *nextPos, float h1)
{
	if( (fabsf(nextPos[1]-h))>h1 || (fabsf(nowPos[1]-h))>h1 )
	{
		//LOGI("H, ABS: %f, %f", h, fabsf(nextPos[1]-h));
		return;
	}

	if(!hasTouch(nowPos, nextPos) ) return;

	float tx=nextPos[0]-posNowX;
	float tz=nextPos[2]-posNowZ;

	float templ=R*InvSqrt(tx*tx+tz*tz);

	nextPos[0]=posNowX+tx*templ;
	nextPos[2]=posNowZ+tz*templ;
	//if()
}

	//bool hasTouch(float posOrgX,float posOrgY,float poxZ);

bool PhySceneCylinder::hasTouch(float *nowPos, float *nextPos)
{//( (nowPos[0]-posNowX)*(nowPos[0]-posNowX)+(nowPos[2]-posNowZ)*(nowPos[2]-posNowZ) )<Rd2
	//		&&
	if(  ( (nextPos[0]-posNowX)*(nextPos[0]-posNowX)+(nextPos[2]-posNowZ)*(nextPos[2]-posNowZ) )>Rd2 )
		return true;
	return false;
}

	/* almost using in nowPos */
bool PhySceneCylinder::isInside(float *pos, float h_)
{
	//if( (fabsf(pos[1]-h))>h_ )
	if( (h-pos[1])>h_ )
	{
		//LOGI("H, ABS: %f, %f", h, fabsf(pos[1]-h));
		return false;
	}
	else if( ( (pos[0]-posNowX)*(pos[0]-posNowX)+(pos[2]-posNowZ)*(pos[2]-posNowZ) )<Rd2 )
			return true;

	//LOGI("not :%f,%f,%f,%f ", pos[0],pos[2],posOrgX,posOrgZ);
		return false;

}

void PhySceneCylinder::addNewPos(float x, float y, float z)
{
	posNowX=posOrgX+x;
	posNowY=posOrgY+y;
	posNowZ=posOrgZ+z;
	h=posNowY;
}

void PhySceneCylinder::setCenterPos(float x, float y, float z)
{
	posOrgX=x;
	posOrgY=y;
	posOrgZ=z;
	posNowX=x;
	posNowY=y;
	posNowZ=z;
}

