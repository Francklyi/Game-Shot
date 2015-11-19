#include "movement.h"
#include <stdio.h>
#include "fileDeal/loadFile.h"
#include "log.h"
#include "fourNumber.h"

Movement::Movement( int id, int code_, int attack_, char *fileName_, int type ):
id(id),
code(code_),
attack(attack_),
fileName(fileName_)
,pA(0)
{
	//load();
	//pA->setType( type );
}

Movement::~Movement()
{

}

float Movement::getHP()
{
	return HP;
}

bool Movement::load()
{
	LOGI("loadAnimation");
	char folder[]="./sdcard/animationFile/";
	//char * filePath=combineChars(aniPath,fileName);
	//LOGI("aniFile:%s",filePath);
	char * chars=getLoadFile().getDataOfAniFile(fileName);//getCharsofFile(filePath);
	pA=new Animation();//(Animation*)malloc(sizeof(*pA));
	pA->id=code;
	pA->frameNum=*(int*)chars;
	//LOGI("frameNum %d\n", *(ep->pA->frameNum));
	chars+=4;
	pA->frameTime=(float*)chars;
	//LOGI("frameTime %f\n", *(ep->pA->frameTime));
	chars+=4*(pA->frameNum);
	pA->jointNum=*(int*)chars;
	//LOGI("jointNum %d\n", *(ep->pA->jointNum));
	chars+=4;
	pA->joints=(Joint*)malloc(sizeof(Joint)*(pA->jointNum));

	pA->joints[0].sn=0;
	pA->joints[0].posf[0]=(*((float*)chars));
	chars+=4;
	pA->joints[0].posf[1]=(*((float*)chars));
	chars+=4;
	pA->joints[0].posf[2]=(*((float*)chars));
	chars+=4;
	pA->joints[0].Matrix=(float*)chars;
	chars+=16*4*(pA->frameNum);
	pA->joints[0].Matrix_1=(float*)chars;
	chars+=16*4*(pA->frameNum);

	for(int i=1;i<(pA->jointNum);i++){
		pA->joints[i].sn=i;
		pA->joints[i].posf[0]=(*((float*)chars));
		chars+=4;
		pA->joints[i].posf[1]=(*((float*)chars));
		chars+=4;
		pA->joints[i].posf[2]=(*((float*)chars));
		chars+=4;
		pA->joints[i].Matrix=(float*)chars;
		chars+=16*4*(pA->frameNum);
	}
	//ep->pA->jointMatrixs=(float*)chars;
	pA->frame=0;
	pA->direction=1;
	pA->change_stay=0;
	LOGI("loadAnimation done");
}

Animation * Movement::getAnimation()
{
	return pA;
}
/*
void Movement::setAniBuffers()
{
	pA->pFrames=(Frame *)malloc(pA->frameNum*sizeof(Frame));


	LOGI( "frameNum: %d", *(pA->frameNum) );

	float floats[3];
	for(int i=0;i<*(pA->frameNum); i++)
	{
		pA->pFrames[i].setInfo(i,Frame::TYPE_NORMAL );


		glGenBuffers(1, &(pA->pFrames[i].ubo) );
		//float *floats = (float*) malloc(3 * sizeof(float));
		FourNumber fourNumber = FourNumber(0, 0, 0);
		float w = 0;
		float wAll=0;
		for (int k = 0; k < (pGD->pointerNum); k++) {
			pGD->vp_[k * 3] = 0;
			pGD->vp_[k * 3 + 1] = 0;
			pGD->vp_[k * 3 + 2] = 0;
			w = 0;
			//wAll=0;
			for (int j = 0; j < (*(pA->jointNum)); j++) {
				w = (*(pGD->weightsp + (((*(pA->jointNum))) * k + j)));
				if ( w != 0.0f) {

					//wAll+=w;
					fourNumber = FourNumber(
							(pGD->vp + k * 8)[0]- (pA->joints[j].posf[0]),
							(pGD->vp + k * 8)[1]- (pA->joints[j].posf[1]),
							(pGD->vp + k * 8)[2]- (pA->joints[j].posf[2]));

					fourNumber.productMatrix(floats,
							(&(pA->joints[j].Matrix[16* i])));

					pGD->vp_[k * 3] += floats[0] * w;
					pGD->vp_[k * 3 + 1] += floats[1] * w;
					pGD->vp_[k * 3 + 2] += floats[2] * w;
				}

			}

			//pGD->vp_[k * 3] = pGD->vp_[k * 3] / wAll;//- (pGD->animations->joints[0].posf[0])
			//pGD->vp_[k * 3 + 1] = pGD->vp_[k * 3 + 1] / wAll;//- (pGD->animations->joints[0].posf[1])
			//pGD->vp_[k * 3 + 2] = pGD->vp_[k * 3 + 2] / wAll;//- (pGD->animations->joints[0].posf[2])

		}
		//free(floats);
		glBindBuffer(GL_ARRAY_BUFFER, pA->pFrames[i].ubo);
		glBufferData(GL_ARRAY_BUFFER, (pGD->pointerNum)*3*sizeof(float), pGD->vp_,
			GL_STATIC_DRAW);
	}

}

void Movement::getFramePointer(float *pf3, int frame,int num)
{
	float w = 0;
	float floats[3]={0,0,0};
	FourNumber fourNumber = FourNumber(0, 0, 0);
	//wAll=0;
	for (int j = 0; j < (*(pA->jointNum)); j++) {
		w = (*(pGD->weightsp + (((*(pA->jointNum))) * num + j)));
		if ( w != 0.0f) {

			//wAll+=w;
			fourNumber = FourNumber(
					(pGD->vp + num * 8)[0]- (pA->joints[j].posf[0]),
					(pGD->vp + num * 8)[1]- (pA->joints[j].posf[1]),
					(pGD->vp + num * 8)[2]- (pA->joints[j].posf[2]));

			fourNumber.productMatrix(floats,
					(&(pA->joints[j].Matrix[16* frame])));

			pf3[0] += floats[0] * w;
			pf3[1] += floats[1] * w;
			pf3[2] += floats[2] * w;
		}

	}
}
*/

Animation *Movement::loadAnimation(char * fileName)
{
	LOGI("loadAnimation 0");
	char folder[]="./sdcard/animationFile/";
	char * filePath=getLoadFile().combineChars(folder,fileName);
	LOGI("aniFile:%s",filePath);
	char * chars=getLoadFile().getCharsofFile(filePath);
	Animation *pA=(Animation*)malloc(sizeof(*pA));
	pA->frameNum=*(int*)chars;
	//LOGI("frameNum %d\n", *(ep->pA->frameNum));
	chars+=4;
	pA->frameTime=(float*)chars;
	//LOGI("frameTime %f\n", *(ep->pA->frameTime));
	chars+=4*(pA->frameNum);
	pA->jointNum=*(int*)chars;
	//LOGI("jointNum %d\n", *(ep->pA->jointNum));
	chars+=4;
	pA->joints=(Joint*)malloc(sizeof(Joint)*(pA->jointNum));

	for(int i=0;i<(pA->jointNum);i++){
		pA->joints[i].sn=i;
		pA->joints[i].posf[0]=(*((float*)chars));
		chars+=4;
		pA->joints[i].posf[1]=(*((float*)chars));
		chars+=4;
		pA->joints[i].posf[2]=(*((float*)chars));
		chars+=4;
		pA->joints[i].Matrix=(float*)chars;
		chars+=16*4*(pA->frameNum);
	}
	//ep->pA->jointMatrixs=(float*)chars;
	pA->frame=1;
	pA->direction=1;
	pA->change_stay=1;
	LOGI("loadAnimation done");
	return pA;
}
