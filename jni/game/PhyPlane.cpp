#include "PhyPlane.h"
#include <math.h>
#include "math/m_math.h"

PhyPlane::PhyPlane()
:h(0)
{

}

void PhyPlane::setHeight(float h_)
{
	h=h_;
}

void PhyPlane::setPointers( float x1_, float y1_, float x2_, float y2_)
{
	x1=x1_;
	y1=y1_;
	x2=x2_;
	y2=y2_;

	ex=x2-x1;
	ex= ex==0.0f?0.0001f:ex;
	ey=y2-y1;

	k=ey/ex;

	float templ=InvSqrt(ex*ex+ey*ey);
	ex=ex*templ;
	ey=ey*templ;
}

float PhyPlane::getSide(float *pos)
{
	return (pos[0]-x1)*k - pos[2]+y1 ;
}

bool PhyPlane::hasCollision(float *nowPos, float *nextPos)
{
	m1=(nowPos[0]-x1)*k - nowPos[2]+y1 ;
	m2=(nextPos[0]-x1)*k - nextPos[2]+y1 ;

	float n1= (x1-nowPos[0])*(nextPos[2]-nowPos[2])- (y1-nowPos[2] )*(nextPos[0]-nowPos[0]) ;
	float n2= (x2-nowPos[0])*(nextPos[2]-nowPos[2]) - (y2-nowPos[2] )*(nextPos[0]-nowPos[0]);
	//LOGI("m12 : %f, %f", m1 , m2 );
	if(  m1 * m2 <0 && n1 *n2<0  )
	return true;
	else
		return false;
}

bool PhyPlane::calculate(float *nowPos, float *nextPos)
{
	//LOGE("m12 : %f, %f", m1 , m2 );
	m1=nextPos[0]-nowPos[0];
	m2=nextPos[2]-nowPos[2];

	m1=ex*m1+ey*m2;

	nextPos[0]= nowPos[0]+m1 * ex;
	nextPos[2]= nowPos[2]+m1 * ey;

	return true;
}
