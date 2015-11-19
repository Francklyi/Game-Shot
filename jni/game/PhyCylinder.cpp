#include "PhyCylinder.h"
#include "math/m_math.h"

PhyCylinder::PhyCylinder()
:r(3)
,pos1(0)
,ex(0)
,ey(0)
,temp(0)
,templ(0)
{

}

void PhyCylinder::setR(float r_)
{
	r=r_;
}

void PhyCylinder::interact(Graph *pG2)
{
	nowPos=pG->nowPos;
	nextPos=pG->nextPos;
	pos1=pG2->pTransform->pTOmatrix->mMatrixQueue.back();
	temp=	(nextPos[0]-pos1[12])*(nextPos[0]-pos1[12])+
				(nextPos[2]-pos1[14])*(nextPos[2]-pos1[14]);
	if(temp>r*r*4 )
		return;

	ex=nowPos[0]-pos1[12];
	ey=nowPos[2]-pos1[14];

	templ=ex*ex+ey*ey;

	if( temp > templ )
		return;

	templ=InvSqrt(templ);
	temp=ex*templ;
	ex=ey*templ;
	ey=-temp;

	templ=nextPos[0]-nowPos[0];
	temp=nextPos[2]-nowPos[2];

	templ=ex*templ+ey*temp;

	nextPos[0]= nowPos[0]+templ * ex;
	nextPos[2]= nowPos[2]+templ * ey;
}

void PhyCylinder::setGraph(Graph *pG_)
{
	pG=pG_;

}
