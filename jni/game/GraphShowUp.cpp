#include "GraphShowUp.h"

GraphShowUp::GraphShowUp()
:theta(0)
{
	char pcshadow[]="showup2_1.ly\0";
	setFileName(pcshadow);
	load();
	posX=0;
	posY=0;
	posZ=0;
}

void GraphShowUp::setPos(float x, float y, float z)
{
	posX=x;
	posY=y;
	posZ=z;
}

void GraphShowUp::update()
{
	glDisable (GL_CULL_FACE);
	theta+=5.0f;
	//if(theta>360) theta= 0;

	float * pos=pGraph->pTransform->pTOmatrix->mMatrixQueue.back();

	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	//getGL().translate( pos[12],0.0f,pos[14] );

	//getGL().scale(1.2,1,1.2);
	getGL().rotate(theta ,0.0f, 1.0f, 0.0f);

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE_MINUS_SRC_COLOR, GL_ONE);
	draw();

	glDisable(GL_BLEND);
	glEnable (GL_CULL_FACE);

}

void GraphShowUp::setGraph(Graph * pGraph_)
{
	pGraph=pGraph_;
}
