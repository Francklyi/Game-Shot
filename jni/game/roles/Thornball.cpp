#include "Thornball.h"
#include "fileDeal/loadFile.h"
#include <ctime>
#include <cstdlib>
#include "MJNI.h"
#include "log.h"

Thornball::Thornball():theta(0),
px(10),py(5),pz(10)
,R(7.0f)
,Graph()
{

}

Thornball::~Thornball()
{

}

void Thornball::reset()
{
	Graph::reset();

}

void Thornball::update(LeadRole *pLR)
{
	//theta++;
	//if(theta==360) theta=0;
	//getGL().translate(px, 20, pz);
	//float *matrix1=getGL().VMatrix;
	//float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;
	//LOGI("thornball update");

	if(pMovepath)
	{
		pfMoveStepNow=pMovepath->getMovestep();
		move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		//LOGI("m: %f, %f, %f",pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
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

	//setRotateY(2);
	draw();

	if(pLR->moveCode==LeadRole::ID_FALLDOWN1)
		return;
	if(isTouch(pLR))
	{
		//LOGI("in Thorn0");

		pLR->act(LeadRole::ID_FALLDOWN1);
		//LOGI("in Thorn1");
		//pAFG->startFrameNum(0);
		//pLR->command=pLR->ID_JUMP1;
		//pLR->pAFG->change_stay = 1;
	}

}

void Thornball::updateLess()
{
	//theta++;
	//if(theta==360) theta=0;
	//getGL().translate(px, 20, pz);
	//float *matrix1=getGL().VMatrix;
	//float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;

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
	//setRotateY(2);
	draw();
}

void Thornball::updateAfterTransform()
{

}

void Thornball::updateAfterDraw()
{

}

bool Thornball::isTouch(LeadRole *pLR)
{

	pos0=pTransform->pTOmatrix->mMatrixQueue.back();
	pos1=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	float l1=((pos0[12]-pos1[12])*(pos0[12]-pos1[12])+
			//(pos0[13]-pos1[13]-8.0f)*(pos0[13]-pos1[13]-8.0f)+
			(pos0[14]-pos1[14])*(pos0[14]-pos1[14]));
	//LOGI("l1:%f",l1);
	if(l1<49)
		if((pos1[13]+10.0f)>(pos0[13]-7.0f)&&pos1[13]<(pos0[13]+7.0f))
		return true;
	else
		return false;
		//setVisable(false);
}
