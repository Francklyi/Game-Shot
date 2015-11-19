#include "Movepath.h"
#include "graphic/Graph.h"

float *Movepath::pfMoveZero=new float[3];

Movepath::Movepath()
:isMoving(false){
	vMPs=vector<Movepoint*>(2);
	vMPs.clear();

	pfMoveZero[0]=0.0f;
	pfMoveZero[1]=0.0f;
	pfMoveZero[2]=0.0f;
}

Movepath::~Movepath()
{

}

void Movepath::addMovepoint(Movepoint *pMP)
{
	vMPs.push_back(pMP);
}

void Movepath::deleteMovepoint(Movepoint *pMP)
{
	for (int i=0;i<vMPs.size();i++)
	{
		if(vMPs[i]==pMP)
		{

		}
	}
}

void Movepath::deleteMovepoint(int num)
{

}

void Movepath::initMovepath()
{
	if(vMPs.size()==0)
		return;

	//pfMoveSteps=new float[vMPs.size()*3];

	//int oneMaxSteps=100;

	//pfMoveSteps[0]=vMPs[0]->pTransform->pTOmatrix->mMatrixQueue.back()[12]/oneMaxSteps;
	//pfMoveSteps[1]=vMPs[0]->pTransform->pTOmatrix->mMatrixQueue.back()[13]/oneMaxSteps;
	//pfMoveSteps[2]=vMPs[0]->pTransform->pTOmatrix->mMatrixQueue.back()[14]/oneMaxSteps;
	//vMPs[0]->setStepSum(200);

	vMPs[0]->initSteps(0,0,0);

	for (int i=1;i<vMPs.size();i++)
	{
		//vMPs[i]->setStepSum(200);
		vMPs[i]->initSteps(vMPs[i-1]->pTransform->pTOmatrix->mMatrixQueue.back()[12],
				vMPs[i-1]->pTransform->pTOmatrix->mMatrixQueue.back()[13],
				vMPs[i-1]->pTransform->pTOmatrix->mMatrixQueue.back()[14]);
		//pfMoveSteps[i*3]=(vMPs[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12]-
		//		vMPs[i-1]->pTransform->pTOmatrix->mMatrixQueue.back()[12])/oneMaxSteps;
		//pfMoveSteps[i*3+1]=(vMPs[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13]-
		//		vMPs[i-1]->pTransform->pTOmatrix->mMatrixQueue.back()[13])/oneMaxSteps;
		//pfMoveSteps[i*3+2]=(vMPs[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]-
		//		vMPs[i-1]->pTransform->pTOmatrix->mMatrixQueue.back()[14])/oneMaxSteps;
	}
}

void Movepath::setSpecData(float *pData)
{

}

float *Movepath::getMovestep()
{
	if(!isMoving)
		return pfMoveZero;
	if(vMPs.size()==0)
		return pfMoveZero;
	return vMPs[0]->getSteps();
}
