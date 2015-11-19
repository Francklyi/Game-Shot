#include "Bullet0.h"

Bullet0::Bullet0()
:Graph(){

}

void Bullet0::init(float *fpos, float *fver){
	pfPos[0]=fpos[0];
	pfPos[1]=fpos[1];
	pfPos[2]=fpos[2];

	pfVer[0]=fver[0];
	pfVer[1]=fver[1];
	pfVer[2]=fver[2];

	pTransform->pTOmatrix->mMatrixQueue.back()[12]=pfPos[0];
	pTransform->pTOmatrix->mMatrixQueue.back()[13]=pfPos[1];
	pTransform->pTOmatrix->mMatrixQueue.back()[14]=pfPos[2];

	pTransform->pTOmatrix->copy(fpos);
}

void Bullet0::update(){

}
