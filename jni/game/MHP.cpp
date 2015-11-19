#include "MHP.h"
#include "RenderManager.h"

float MHP::ThetaBody=0;

MHP::MHP()
:hp(1),
 hpAll(1)
{
	//pos[0]=0;
	//pos[1]=0;
	//pos[2]=0;
}

MHP::~MHP()
{

}

void MHP::setData()
{
/*
	float *pv=new float[12]
	{
			-10.0f,0.0f,10.0f,
			10.0f,0.0f,10.0f,
			-10.0f,0.0f,-10.0f,
			10.0f,0.0f,-10.0f
	};

	int *pvi=new int[6]
	{
			0,1,2,
			2,1,3
	};
*/
	float pv[]=
	{
			-10.0f,10.0f,0.0f,
			10.0f,10.0f,0.0f,
			-10.0f,0.0f,0.0f,
			10.0f,0.0f,0.0f
			//,
			//0.0f,0.5f,-2.0f,
			//0.0f,0.5f,2.0f,
			//0.0f,0.0f,-2.0f,
			//0.0f,0.0f,2.0f
	};

	unsigned short pvi[]=
	{
			//0,1,2,
			//1,3,2
			//,
			0,2,1,
			1,2,3
			//4,6,5,
			//5,6,7
	};
	glGenBuffers(1, &ubov);
	glBindBuffer(GL_ARRAY_BUFFER, ubov);
	glBufferData(GL_ARRAY_BUFFER, 12 * sizeof(float), pv,
			GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &ubovi);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubovi);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned short), pvi,
			GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    Ka=new float[3];
    Ka[0]=1.0f;
    Ka[1]=0.0f;
    Ka[2]=0.0f;

    Kd=new float[3];
    Kd[0]=1.0f;
    Kd[1]=0.0f;
    Kd[2]=0.0f;

    Ks=new float[3];
    Ks[0]=1.0f;
    Ks[1]=0.0f;
    Ks[2]=0.0f;

	Shininess=20;


}

void MHP::setHP(float hp_)
{
	hp=hp_;
}

void MHP::setHPAll(float hpa_)
{
	hpAll=hpa_;
}

void MHP::setTheta()
{
	ThetaBody=atan2(-(getGL().VMatrix[8]), getGL().VMatrix[10])*180/M_PI;
}

void MHP::setPos(float x, float y, float z)
{
	getMatrix().setIdentityM(pTransform->pTOmatrix->mMatrixQueue.back(),0);
	pTransform->pTOmatrix->mMatrixQueue.back()[12]=x;
	pTransform->pTOmatrix->mMatrixQueue.back()[13]=y;
	pTransform->pTOmatrix->mMatrixQueue.back()[14]=z;
}

void MHP::draw()
{

	getMatrix().rotateM(
			pTransform->pTOmatrix->mMatrixQueue.back(),0
			, ThetaBody, 0, 1, 0);
	//getGL().rotate(ThetaBody,0,1,0);
	//getGL().glslUpdate();

	//hpAll=10;
	pRenderManager->addGraph(this);
		return;
}

void MHP::render()
{
	updateTransform();

	glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(glslFunctype) );

	getGL().material->setAmbient(Ka);
	getGL().material->setDiffuse(Kd);
	getGL().material->setSpecular(Ks);
	getGL().material->setShininess(Shininess);

	getGL().material->draw();
	pf32[0]=-2.0f; pf32[1]=0.5f; pf32[2]=0.0f;
	pf32[3]=-2.0f+4.0f * hp/hpAll; pf32[4]=0.5f; pf32[5]=0.0f;
	pf32[6]=-2.0f; pf32[7]=0.0f; pf32[8]=0.0f;
	pf32[9]=-2.0f+4.0f * hp/hpAll; pf32[10]=0.0f; pf32[11]=0.0f;

	/*
	float pv[]=
	{
			-2.0f,0.5f,0.0f,
			-2.0f+4.0f * hp/hpAll ,0.5f,0.0f,
			-2.0f,0.0f,0.0f,
			-2.0f+4.0f * hp/hpAll ,0.0f,0.0f
	};
	*/

	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, pf32 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubovi);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, 0);
}
