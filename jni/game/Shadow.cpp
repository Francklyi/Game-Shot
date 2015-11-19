#include "Shadow.h"

Shadow::Shadow()
:Graph()
{
	char pcshadow[]="shadow1.ly\0";
	setFileName(pcshadow);
	load();
	posX=0;
	posY=0;
	posZ=0;
}

void Shadow::setGraphRace( GraphRace * pGR)
{
	enemyRace=pGR;
}

void Shadow::setPLR(Graph * pLR_)
{
	pLR=pLR_;
}

void Shadow::updateAfterTransform()
{

}

void Shadow::updateAfterDraw()
{

}

void Shadow::setPos(float x, float y, float z)
{
	posX=x;
	posY=y;
	posZ=z;
}

void Shadow::update()
{
	//float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();
	//float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	//getGL().translate( pos[12],.2,pos[14] );
	//getGL().scale(1.2,1,1.2);

	draw();

	return;


		glEnable(GL_BLEND);
		glBlendFunc(GL_DST_COLOR, GL_ONE_MINUS_DST_ALPHA);
/*
	for(int i=0; i<enemyRace->vpg.size(); i++)
	{
		if(enemyRace->vpg[i]->state==2)
			continue;
		float * pos=enemyRace->vpg[i]->pTransform->pTOmatrix->mMatrixQueue.back();


		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();
		//float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
		getGL().translate( pos[12],.2,pos[14] );

		getGL().scale(1.2,1,1.2);

		draw();

		//glDisable(GL_BLEND);
	}
*/
	float *pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	getGL().translate( pos[12],.2,pos[14] );
	getGL().scale(1.2,1,1.2);

	draw();

	glDisable(GL_BLEND);

	return;

	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	getGL().translate(posX,.2,posZ);
	getGL().scale(1.2,1,1.2);


	//glBlendEquation(GL_FUNC_SUBTRACT);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5f);

	draw();

	//glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
}
