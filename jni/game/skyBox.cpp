#include "skyBox.h"
#include "fileDeal/loadFile.h"
//float SkyBox::yRot=0.0f;
#include "Camera.h"

SkyBox::SkyBox()
:Graph()
{
	yRot=0.0f;
	yRotSpeed = 0.01f;

	getMatrix().scaleM(pTransform->pTOmatrix->mMatrixQueue.back(), 0, 100,100,100);

	char fileName[]="skybox6.ly";
	setFileName(fileName);
	load();
}

void SkyBox::update()
{
	//glsl->setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY

	//getMatrix().set(matrixTemp,pTransform->pTOmatrix->mMatrixQueue.back());

	//glsl->translate(camera1->m_View.x
     //       		,camera1->m_View.y-250//
      //      		,camera1->m_View.z);

			//pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12]
	         //       ,pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13]-250
	          //      ,pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14] );


	//glsl->scale(20,20,20);


	draw();

}

void SkyBox::updateAfterDraw()
{
	//getMatrix().set(pTransform->pTOmatrix->mMatrixQueue.back(),matrixTemp);
}

float SkyBox::getYRot(){
		yRot+=yRotSpeed;
		//if(yRot==360) yRot=0;
	    return -yRot;
	     }

void SkyBox::setYRotSpeed(float speed){
	yRotSpeed = speed;
}


