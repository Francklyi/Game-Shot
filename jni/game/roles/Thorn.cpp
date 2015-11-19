#include "Thorn.h"
#include "fileDeal/loadFile.h"
#include <ctime>
#include <cstdlib>
#include "MJNI.h"
#include "log.h"

Thorn::Thorn()
:Graph()
{

}

Thorn::~Thorn()
{

}

void Thorn::reset()
{
	Graph::reset();
}

void Thorn::update(LeadRole *pLR)
{
	if(pMovepath)
	{
		pfMoveStepNow=pMovepath->getMovestep();
		move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
	}
	setRange();
	draw();
	if(pLR->moveCode==pLR->ID_FALLDOWN1&&pLR->pAFG->frame<24&&pLR->pAFG->change_stay == 1)
		return;
	if(isTouch(pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12],
			pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13],
			pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14]))
	{
		//LOGI("in Thorn");

		pLR->act(LeadRole::ID_FALLDOWN1);

		//pAFG->startFrameNum(0);
		//pLR->command=pLR->ID_JUMP1;
		pLR->pAFG->change_stay = 1;
	}
}


void Thorn::updateLess()
{
	if(pMovepath)
	{
		pfMoveStepNow=pMovepath->getMovestep();
		move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
	}
	draw();
}

void Thorn::updateUnderFloor()
{
	if(pMovepath)
	{
		pfMoveStepNow=pMovepath->getMovestep();
		move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
	}
}

void Thorn::setRange()
{
	xMin=pTransform->pTOmatrix->mMatrixQueue.back()[12]
	     -5.0f*pTransform->pTOmatrix->mMatrixQueue.back()[0];
	xMax=pTransform->pTOmatrix->mMatrixQueue.back()[12]+5.0f*pTransform->pTOmatrix->mMatrixQueue.back()[0];

	yMin=pTransform->pTOmatrix->mMatrixQueue.back()[13]-1.0f;
	yMax=pTransform->pTOmatrix->mMatrixQueue.back()[13]+6.0f;

	zMin=pTransform->pTOmatrix->mMatrixQueue.back()[14]
	     -5.0f*pTransform->pTOmatrix->mMatrixQueue.back()[10];
	zMax=pTransform->pTOmatrix->mMatrixQueue.back()[14]+5.0f*pTransform->pTOmatrix->mMatrixQueue.back()[10];
}

bool Thorn::isTouch(float x, float y, float z)
{
	//LOGI("xyz: %f,%f,%f",x,y,z);
	//LOGI("xx:%f,%f",xMin,xMax);
	//LOGI("yy:%f,%f",yMin,yMax);
	//LOGI("zz:%f,%f",zMin,zMax);
	if(x>xMin&&x<xMax && (y+9.5)>yMin&&y<yMax && z>zMin&&z<zMax)
		return true;
	else
		return false;
}

void Thorn::move(float x, float y, float z)
{
	Graph::move(x,y,z);

}

void Thorn::updateAfterTransform()
{

}

void Thorn::updateAfterDraw()
{

}
