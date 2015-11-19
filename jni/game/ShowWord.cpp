#include "ShowWord.h"
#include "parser/ParserLy.h"
#include "TextureFactory.h"

float *ShowWord::vertics=0;
float *ShowWord::texCoord1=0;
unsigned short *ShowWord::index=0;
int ShowWord::objectSum=0;
GLuint ShowWord::mTextureId=0;

ShowWord::ShowWord()
:Entity()
,showNum(0)
{
	pColor[0]=0.0f;
	pColor[1]=0.0f;
	pColor[2]=0.0f;
	texCoordTemp=new float[8*3];
	if(objectSum!=0) return;

	objectSum++;
	vertics=new float[12*3];
	texCoord1=new float[44];
	index=new unsigned short[6*3];
	//texCoord1=texTemp;
	int n=3;
	float lenX=6.0f;
	float lenY=10.0f;
	float x0=-n*0.5f*lenX;
	int m=0;
	for(int i=0; i<n; i++)
	{
		m=12*i;
		vertics[m]=x0+lenX*i;
		vertics[m+1]=lenY*0.5f;
		vertics[m+2]=0;
		vertics[m+3]=vertics[m];
		vertics[m+4]=-vertics[m+1];
		vertics[m+5]=0;

		vertics[m+6]=x0+lenX*(i+1);
		vertics[m+7]=lenY*0.5f;
		vertics[m+8]=0;
		vertics[m+9]=vertics[m+6];
		vertics[m+10]=-vertics[m+7];
		vertics[m+11]=0;

	}
	n=10;

	float EWIDTH=0.0147f;//0.058593f;
	for(int i=0; i<n+1; i++)
	{
		m=4*i;
		texCoord1[m+0]=0.75f+i*EWIDTH;
		texCoord1[m+1]=0.507f;//0.234375f;
		texCoord1[m+2]=texCoord1[m+0];
		texCoord1[m+3]=0.5315f;//1.0f;
	}

	n=10;
	int k=0;
	for(int i=0; i<n; i++)
	{
		m=6*i;
		k=4*i;
		index[m+0]=k;
		index[m+1]=k+1;
		index[m+2]=k+2;
		index[m+3]=k+2;
		index[m+4]=k+1;
		index[m+5]=k+3;
	}
/*
	index[0]=0; index[1]=2; index[2]=1;
	index[3]=1; index[4]=2; index[5]=3;

	texCoord1[0]=0; texCoord1[1]=EWIDTH;
	texCoord1[2]=3*EWIDTH; texCoord1[3]=EWIDTH;
	texCoord1[4]=0; texCoord1[5]=2*EWIDTH;
	texCoord1[6]=3*EWIDTH; texCoord1[7]=2*EWIDTH;
*/
	//texCoord1[0]=0.0f; texCoord1[1]=1.0f;
	//texCoord1[2]=1.0f; texCoord1[3]=1.0f;
	//texCoord1[4]=0.0f; texCoord1[5]=0.0f;
	//texCoord1[6]=1.0f; texCoord1[7]=0.0f;


	char texFile[]="sky8.pkm\0";//"num1.png\0";
	mTextureId =pTexFactory->getTexID(texFile);
	//pParserLy0->setTextureB(texFile);
	//active_texture_id = getGL().getATextureID();
	//glBindTexture(GL_TEXTURE_2D, mTextureId);
}
/*
void ShowWord::showNum(int *pNum, int n)
{
	float len=5.0f;
	float x0=-n*0.5*len;
	int m=0;

	for(int i=0; i<n+1; i++)
	{
		m=6*i;
		vertics[m]=x0+len*i;
		vertics[m+1]=len*0.5f;
		vertics[m+2]=0;
		vertics[m+3]=vertics[m];
		vertics[m+4]=-vertics[m+1];
		vertics[m+5]=0;
	}
}
*/

void ShowWord::setColor(float r,float g,float b)
{
	pColor[0]=r;
	pColor[1]=g;
	pColor[2]=b;
}

void ShowWord::setShowNum(int n)
{
	showNum=n;
}

void ShowWord::draw(int n)
{
	drawMeterial();

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0, 0, texCoord1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, &(texCoord1[4*n]) );//+sizeof(float)*4*3
    glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

    Entity::draw();
    getGL().translate(6.0f,8.0f,0.0f);

    //pTransform->pTOmatrix->mMatrixQueue.back()[13]+=8.0f;

	getGL().glslUpdate();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	//GL_ONE_MINUS_SRC_COLOR, GL_ONE 中空

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,index);

	glDisable(GL_BLEND);
}

void ShowWord::draw(int n10,int n)
{
	drawMeterial();
	texCoordTemp;

	int num1=n10*4;

	texCoordTemp[0]=texCoord1[num1+0];
	texCoordTemp[1]=texCoord1[num1+1];
	texCoordTemp[2]=texCoord1[num1+2];
	texCoordTemp[3]=texCoord1[num1+3];

	texCoordTemp[4]=texCoord1[num1+4];
	texCoordTemp[5]=texCoord1[num1+5];
	texCoordTemp[6]=texCoord1[num1+6];
	texCoordTemp[7]=texCoord1[num1+7];

	num1=n*4;
	texCoordTemp[8]=texCoord1[num1+0];
	texCoordTemp[9]=texCoord1[num1+1];
	texCoordTemp[10]=texCoord1[num1+2];
	texCoordTemp[11]=texCoord1[num1+3];

	texCoordTemp[12]=texCoord1[num1+4];
	texCoordTemp[13]=texCoord1[num1+5];
	texCoordTemp[14]=texCoord1[num1+6];
	texCoordTemp[15]=texCoord1[num1+7];


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0, 0, texCoord1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoordTemp );//+sizeof(float)*4*3
    glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

    Entity::draw();
    getGL().translate(3.0f,8.0f,0.0f);

    //pTransform->pTOmatrix->mMatrixQueue.back()[13]+=8.0f;

	getGL().glslUpdate();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	//GL_ONE_MINUS_SRC_COLOR, GL_ONE 中空

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6*2,GL_UNSIGNED_SHORT,index);

	glDisable(GL_BLEND);
}

void ShowWord::draw(int n100,int n10,int n)
{
	drawMeterial();

	int num1=n100*4;

	texCoordTemp[0]=texCoord1[num1+0];
	texCoordTemp[1]=texCoord1[num1+1];
	texCoordTemp[2]=texCoord1[num1+2];
	texCoordTemp[3]=texCoord1[num1+3];

	texCoordTemp[4]=texCoord1[num1+4];
	texCoordTemp[5]=texCoord1[num1+5];
	texCoordTemp[6]=texCoord1[num1+6];
	texCoordTemp[7]=texCoord1[num1+7];

	num1=n10*4;
	texCoordTemp[8]=texCoord1[num1+0];
	texCoordTemp[9]=texCoord1[num1+1];
	texCoordTemp[10]=texCoord1[num1+2];
	texCoordTemp[11]=texCoord1[num1+3];

	texCoordTemp[12]=texCoord1[num1+4];
	texCoordTemp[13]=texCoord1[num1+5];
	texCoordTemp[14]=texCoord1[num1+6];
	texCoordTemp[15]=texCoord1[num1+7];

	num1=n*4;
	texCoordTemp[16]=texCoord1[num1+0];
	texCoordTemp[17]=texCoord1[num1+1];
	texCoordTemp[18]=texCoord1[num1+2];
	texCoordTemp[19]=texCoord1[num1+3];

	texCoordTemp[20]=texCoord1[num1+4];
	texCoordTemp[21]=texCoord1[num1+5];
	texCoordTemp[22]=texCoord1[num1+6];
	texCoordTemp[23]=texCoord1[num1+7];

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0, 0, texCoord1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoordTemp );//+sizeof(float)*4*3
    glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

    Entity::draw();
    getGL().translate(0.0f,8.0f,0.0f);

    //pTransform->pTOmatrix->mMatrixQueue.back()[13]+=8.0f;

	getGL().glslUpdate();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	//GL_ONE_MINUS_SRC_COLOR, GL_ONE 中空

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6*3,GL_UNSIGNED_SHORT,index);

	glDisable(GL_BLEND);
}

void ShowWord::drawMeterial()
{
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();

	//glLineWidth(20.125f);

	getGL().material->setAmbient(pColor);

	getGL().material->setDiffuse(pColor);
	getGL().material->setSpecular(pColor);

	getGL().material->draw();

    getGL().setHasTex(1.0f);

    glActiveTexture( GL_TEXTURE0);//active_texture_id +

    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glUniform1i(getGL().pGPNow->tex1Handle, 0);//active_texture_id
}

void ShowWord::draw()
{
	return;

	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();

	//glLineWidth(20.125f);

	getGL().material->setAmbient(pColor);

	getGL().material->setDiffuse(pColor);
	getGL().material->setSpecular(pColor);

	getGL().material->draw();

    getGL().setHasTex(1.0f);

    glActiveTexture( GL_TEXTURE0);//active_texture_id +

    glBindTexture(GL_TEXTURE_2D, mTextureId);

    glUniform1i(getGL().pGPNow->tex1Handle, 0);//active_texture_id


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0, 0, texCoord1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, &(texCoord1[4*showNum]) );//+sizeof(float)*4*3
    glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

    Entity::draw();
    getGL().translate(0.0f,8.0f,0.0f);

    //pTransform->pTOmatrix->mMatrixQueue.back()[13]+=8.0f;

	getGL().glslUpdate();

	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);

	//GL_ONE_MINUS_SRC_COLOR, GL_ONE 中空

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,index);

	glDisable(GL_BLEND);
	//line.draw(&p1, &pMid);
}
