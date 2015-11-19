#include "BulletSmall.h"

BulletSmall::BulletSmall()
:Bullet0(){
	char fileName[]="bullet2.ly";
	setFileName(fileName);
	load();
}

void BulletSmall::update(){

	pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfVer[0]*0.2;
	//pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfVer[1]*0.1;
	pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfVer[2]*0.2;
	draw();
}
