#include "coin.h"
#include "fileDeal/loadFile.h"
#include <ctime>
#include <cstdlib>
#include "MJNI.h"

int Coin::coinSum=0;
Coin::Coin():theta(0),visable(true),
px(10),py(5),pz(10)
,R(6)
,isClose(false)
,Graph()
{
	theta=(coinSum++)*30;

}

Coin::~Coin()
{

}

void Coin::reset()
{
	Graph::reset();
	visable=true;
	isClose=false;
}

void Coin::update(LeadRole *pg)
{
	//theta++;
	//if(theta==360) theta=0;
	//getGL().translate(px, 20, pz);
	//float *matrix1=getGL().VMatrix;
	//float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;
	if(!visable) return;

	if(pMovepath)
	{
		pfMoveStepNow=pMovepath->getMovestep();
		move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
	}
	if(isClose)
	{
		if(!(visable=!isCapture(pg)))
			{
				//pMjni->captureCoin();
				pg->coinNum++;
				return;
			}
		//pos0=pTransform->pTOmatrix->mMatrixQueue.back();
		//pos1=pg->pTransform->pTOmatrix->mMatrixQueue.back();
		pos0[12]+=(pos1[12]-pos0[12])*0.25;
		pos0[13]+=(pos1[13]+8.0f-pos0[13])*0.25;
		pos0[14]+=(pos1[14]-pos0[14])*0.25;

	}
	else
	{
		isClose=getIsClose(pg);
	}

	//LOGI("dis0:%s",fileStr.c_str());
	floats1[0]=pTransform->pTOmatrix->mMatrixQueue.back()[12];
	floats1[1]=pTransform->pTOmatrix->mMatrixQueue.back()[13];
	floats1[2]=pTransform->pTOmatrix->mMatrixQueue.back()[14];
	floats1[3]=1;
	//getMatrix().multiplyMM(pf32, 0, getGL().VMatrix, 0, getGL().MMatrix, 0);
	//getMatrix().multiplyMM(pf32, 16, getGL().PMatrix, 0, getGL().VMatrix, 0 );
	getMatrix().multiplyMV(pf32,0,getGL().PVMatrix, 0, floats1, 0);
	if(fabs(pf32[0]/pf32[3])>1 || fabs(pf32[1]/pf32[3])>1 || pf32[3]<0 )
		return ;


	//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();
	//getMatrix().rotateM(pTransform->pTOmatrix->mMatrixQueue.back(),0, 2,0,1,0);
	//getGL().rotate(theta,0,1,0);
	setRotateY(2);
	draw();
}

void Coin::updateLess()
{
	//theta++;
	//if(theta==360) theta=0;
	//getGL().translate(px, 20, pz);
	//float *matrix1=getGL().VMatrix;
	//float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;
	if(!visable) return;

	if(pMovepath)
	{
		pfMoveStepNow=pMovepath->getMovestep();
		move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
	}
	//if(!isPointerScreen())
	//{
		//LOGI("dis0:%s",fileStr.c_str());
	//	return;
	//}
	floats1[0]=pTransform->pTOmatrix->mMatrixQueue.back()[12];
	floats1[1]=pTransform->pTOmatrix->mMatrixQueue.back()[13];
	floats1[2]=pTransform->pTOmatrix->mMatrixQueue.back()[14];
	floats1[3]=1;
	//getMatrix().multiplyMM(pf32, 0, getGL().VMatrix, 0, getGL().MMatrix, 0);
	//getMatrix().multiplyMM(pf32, 16, getGL().PMatrix, 0, getGL().VMatrix, 0 );
	getMatrix().multiplyMV(pf32,0,getGL().PVMatrix, 0, floats1, 0);
	if(fabs(pf32[0]/pf32[3])>1 || fabs(pf32[1]/pf32[3])>1 || pf32[3]<0 )
		return ;
	//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();
	//getMatrix().rotateM(pTransform->pTOmatrix->mMatrixQueue.back(),0, 2,0,1,0);
	//getGL().rotate(theta,0,1,0);
	setRotateY(2);
	draw();
}

void Coin::updateAfterTransform()
{

}

void Coin::updateAfterDraw()
{

}

void Coin::setVisable(bool visable_)
{
	visable=visable_;
}

bool Coin::isVisable()
{
	return visable;
}

double Coin::random(int seed,double start, double end)
{
  srand(seed);
  return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

bool Coin::getIsClose(LeadRole *pg)
{
	pos0=pTransform->pTOmatrix->mMatrixQueue.back();
	pos1=pg->pTransform->pTOmatrix->mMatrixQueue.back();

	if(((pos0[12]-pos1[12])*(pos0[12]-pos1[12])+
			(pos0[13]-pos1[13]-8.0f)*(pos0[13]-pos1[13]-8.0f)+
			(pos0[14]-pos1[14])*(pos0[14]-pos1[14]))
			<250)
		return true;
	else
		return false;
}

bool Coin::isCapture(LeadRole *pg)
{

	pos0=pTransform->pTOmatrix->mMatrixQueue.back();
	pos1=pg->pTransform->pTOmatrix->mMatrixQueue.back();
	if(((pos0[12]-pos1[12])*(pos0[12]-pos1[12])+
			(pos0[13]-pos1[13]-8.0f)*(pos0[13]-pos1[13]-8.0f)+
			(pos0[14]-pos1[14])*(pos0[14]-pos1[14]))
			<14)
		return true;
	else
		return false;
		//setVisable(false);
}
