#include "PhySceneQuad.h"
#include "log.h"
#include <math.h>

PhySceneQuad::PhySceneQuad()
:PhyScene()
{
	pfPosOrig=new float[12];
	pfPosNow=new float[12];

	vPlanes=vector<PhyPlane*>(4);
	fsides[0]=0;
	fsides[1]=0;
	fsides[2]=0;
	fsides[3]=0;
	shapeType=SHAPE_QUAD;
}

bool PhySceneQuad::isInside(float *pos, float h_)
{
	//if( (fabsf(pos[1]-h))>h_ )
	if( (h-pos[1])>h_ )
	{
		//LOGI("H, ABS: %f, %f", h, fabsf(pos[1]-h));
		return false;
	}
	else
	if(fsides[0]==1?
			  (vPlanes[0]->getSide(pos)>0? true: false)
			: (vPlanes[0]->getSide(pos)<0? true: false) )
	{
		if(fsides[1]==1?
				  (vPlanes[1]->getSide(pos)>0? true: false)
				: (vPlanes[1]->getSide(pos)<0? true: false) )
		{
			if(fsides[2]==1?
					  (vPlanes[2]->getSide(pos)>0? true: false)
					: (vPlanes[2]->getSide(pos)<0? true: false) )
			{
				if(fsides[3]==1?
						  (vPlanes[3]->getSide(pos)>0? true: false)
						: (vPlanes[3]->getSide(pos)<0? true: false) )
				{
					return true;
				}
				else
					return false;
			}
			else
				return false;
		}
		else
			return false;
	}
	else
		return false;
}

bool PhySceneQuad::hasTouch(float *nowPos, float *nextPos)
{
	planeID=-1;
	collisionTime=0;
	for(int i=0; i<vPlanes.size(); i++)
	{
		if(vPlanes[i]->hasCollision(nowPos, nextPos))
		{
			collisionTime++;
			if(collisionTime>1)
			{
				type=TYPE_STAY;
				return true;
			}
			type=TYPE_CHANGE;
			planeID=i;
		}
	}

	if(collisionTime==0)
	{
		type=TYPE_NONE;
		return false;
	}


	//if( (fabsf(nextPos[1]-h))<2) return true;
	//return false;
}

void PhySceneQuad::setPlane(float *pfs)
{
	for(int i=0;i<12;i++)
	{
		pfPosOrig[i]=pfs[i];
		//pfPosNow[i]=pfs[i];
	}

	PhyPlane *pPlane1=new PhyPlane();
	pPlane1->setPointers(pfs[0],pfs[2],pfs[3],pfs[5]);
	vPlanes[0]=pPlane1;

	pPlane1=new PhyPlane();
	pPlane1->setPointers(pfs[3],pfs[5],pfs[6],pfs[8]);
	vPlanes[1]=pPlane1;

	pPlane1=new PhyPlane();
	pPlane1->setPointers(pfs[6],pfs[8],pfs[9],pfs[11]);
	vPlanes[2]=pPlane1;

	pPlane1=new PhyPlane();
	pPlane1->setPointers(pfs[9],pfs[11],pfs[0],pfs[2]);
	vPlanes[3]=pPlane1;

	h=(pfs[1]+pfs[4]+pfs[7]+pfs[10])*0.25f-0.2f;

	float midPos[3];
	midPos[0]=(pfs[0]+pfs[3]+pfs[6]+pfs[9])*0.25f;
	midPos[1]=h;
	midPos[2]=(pfs[2]+pfs[5]+pfs[8]+pfs[11])*0.25f;

	fsides[0]=vPlanes[0]->getSide(midPos)>0? 1: -1;
	fsides[1]=vPlanes[1]->getSide(midPos)>0? 1: -1;
	fsides[2]=vPlanes[2]->getSide(midPos)>0? 1: -1;
	fsides[3]=vPlanes[3]->getSide(midPos)>0? 1: -1;

}

void PhySceneQuad::addNewPos(float x, float y, float z)
{
	pfPosNow[0]=pfPosOrig[0]+x;
	pfPosNow[1]=pfPosOrig[1]+y;
	pfPosNow[2]=pfPosOrig[2]+z;
	pfPosNow[3]=pfPosOrig[3]+x;
	pfPosNow[4]=pfPosOrig[4]+y;
	pfPosNow[5]=pfPosOrig[5]+z;
	pfPosNow[6]=pfPosOrig[6]+x;
	pfPosNow[7]=pfPosOrig[7]+y;
	pfPosNow[8]=pfPosOrig[8]+z;
	pfPosNow[9]=pfPosOrig[9]+x;
	pfPosNow[10]=pfPosOrig[10]+y;
	pfPosNow[11]=pfPosOrig[11]+z;

	resetPlane(pfPosNow);
}

void PhySceneQuad::resetPlane(float *pfs)
{

	vPlanes[0]->setPointers(pfs[0],pfs[2],pfs[3],pfs[5]);

	vPlanes[1]->setPointers(pfs[3],pfs[5],pfs[6],pfs[8]);

	vPlanes[2]->setPointers(pfs[6],pfs[8],pfs[9],pfs[11]);

	vPlanes[3]->setPointers(pfs[9],pfs[11],pfs[0],pfs[2]);

	h=(pfs[1]+pfs[4]+pfs[7]+pfs[10])*0.25f;
/*
	float midPos[3];
	midPos[0]=(pfs[0]+pfs[3]+pfs[6]+pfs[9])*0.25f;
	midPos[1]=h;
	midPos[2]=(pfs[2]+pfs[5]+pfs[8]+pfs[11])*0.25f;

	fsides[0]=vPlanes[0]->getSide(midPos)>0? 1: -1;
	fsides[1]=vPlanes[1]->getSide(midPos)>0? 1: -1;
	fsides[2]=vPlanes[2]->getSide(midPos)>0? 1: -1;
	fsides[3]=vPlanes[3]->getSide(midPos)>0? 1: -1;
*/
}

void PhySceneQuad::calculate(float *nowPos, float *nextPos, float h1)
{

	if( (fabsf(nextPos[1]-h))>h1 || (fabsf(nowPos[1]-h))>h1 )
	{
		//LOGI("H, ABS: %f, %f", h, fabsf(nextPos[1]-h));
		return;
	}

	//if(nextPos[1]<h) nextPos[1]=h;
	//LOGI("0");
	if(!hasTouch(nowPos, nextPos) ) return;
	//LOGI("1");

	//nextPos[1]=h;

	if(type==TYPE_STAY)
	{
		nextPos[0]=nowPos[0];
		nextPos[2]=nowPos[2];
		//LOGI("TYPE_STAY");
		return;
	}else if(type==TYPE_CHANGE)
	{
		//LOGI("TYPE_CHANGE");
		vPlanes[planeID]->calculate(nowPos, nextPos);
		for(int i=0; i<vPlanes.size(); i++)
		{
			if(i==planeID) continue;
			if(vPlanes[i]->hasCollision(nowPos, nextPos))
			{
				nextPos[0]=nowPos[0];
				nextPos[2]=nowPos[2];
				return;
			}
		}
	}

/*
	myNextPos[0]=nextPos[0];
	myNextPos[1]=nextPos[1];
	myNextPos[2]=nextPos[2];

	myNextPosTemp[0]=nextPos[0];
	myNextPosTemp[1]=nextPos[1];
	myNextPosTemp[2]=nextPos[2];

	collisionTime=0;

	for(int i=0; i<vPlanes.size(); i++)
	{
		if(vPlanes[i]->calculate(nowPos, nextPos))
		{
			collisionTime++;
			if(collisionTime==1)
			{
				//nextPos[0]=myNextPos[0];
				//nextPos[2]=myNextPos[2];

				//myNextPos[0]=myNextPosTemp[0];
				//myNextPos[1]=myNextPosTemp[1];
				//myNextPos[2]=myNextPosTemp[2];
			}else
			{
				//LOGI("碰撞了两个墙壁");
				//碰撞了两个墙壁,就静止
				nextPos[0]=nowPos[0];
				nextPos[2]=nowPos[2];
				return;
			}
			//nextPos[0]=nowPos[0];
			//nextPos[2]=nowPos[2];
		}
	}
*/
	//nextPos[0]=myNextPos[0];
	//nextPos[2]=myNextPos[2];

}
