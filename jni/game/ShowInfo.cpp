#include "showInfo.h"
#include "GLSL/matrix.h"
#include "parser/ParserLy.h"
#include "Screen2d.h"
#include "TextureFactory.h"

ShowInfo::ShowInfo()
:showNum(0),
 showNumLast(-1)
{

	vertics=new float[12];
	texCoord1=new float[8];
	index=new unsigned short[6];
	index[0]=0; index[1]=2; index[2]=1;
	index[3]=0; index[4]=3; index[5]=2;

	texCoord1[0]=0.5f;//0.0f;
	texCoord1[1]=0.913f;//0.0f;
	texCoord1[2]=0.53f;//1.0f;
	texCoord1[3]=0.913f;//0.0f;
	texCoord1[4]=0.53f;//1.0f;
	texCoord1[5]=0.942f;//1.0f;
	texCoord1[6]=0.5f;//0.0f;
	texCoord1[7]=0.942f;//1.0f;

	char texFile[]="sky8.pkm\0";//"coin1.png\0";
	mTextureId =pTexFactory->getTexID(texFile);//pParserLy0->setTextureB(texFile);
	//active_texture_id = getGL().getATextureID();
	glBindTexture(GL_TEXTURE_2D, mTextureId);


	pColor[0]=0.0f;
	pColor[1]=0.0f;
	pColor[2]=0.0f;

	//for num
	char texFile1[]="sky8.pkm\0";//"num1.png\0";
	mTextureId_num =pTexFactory->getTexID(texFile1);//pParserLy0->setTextureB(texFile1);
	//active_texture_id_num = getGL().getATextureID();
	glBindTexture(GL_TEXTURE_2D, mTextureId_num);

	vertics_num=new float[12*3];
	texCoord1_num=new float[8*10];
	index_num=new unsigned short[6*3];
	texCoord1_num2=new float[8*10];
	//index_num[0]=0; index_num[1]=2; index_num[2]=1;
	//index_num[3]=0; index_num[4]=3; index_num[5]=2;

	//texCoord1_num[0]=0.058593f*2;texCoord1_num[1]=0.234375f;
	//texCoord1_num[2]=0.058593f*3;texCoord1_num[3]=0.234375f;
	//texCoord1_num[4]=0.058593f*3;texCoord1_num[5]=1.0f;
	//texCoord1_num[6]=0.058593f*2;texCoord1_num[7]=1.0f;


	int n=3;
	int m=0;
	int k=0;
	for(int i=0; i<n; i++)
	{
		m=6*i;
		k=4*i;
		index_num[m+0]=k;
		index_num[m+1]=k+1;
		index_num[m+2]=k+2;
		index_num[m+3]=k+2;
		index_num[m+4]=k+1;
		index_num[m+5]=k+3;
	}
	float EWIDTH=0.0147f;//0.058593f;
	n=10;

	texCoord1_num[0]=0.75f;//0;
	texCoord1_num[1]=0.507f;//0.234375f;
	texCoord1_num[2]=texCoord1_num[0];
	texCoord1_num[3]=0.5315f;//1.0f;

	for(int i=0; i<n-1; i++)
	{
		m=8*i;
		texCoord1_num[m+4]=texCoord1_num[0]+(i+1)*EWIDTH;
		texCoord1_num[m+5]=texCoord1_num[1];
		texCoord1_num[m+6]=texCoord1_num[m+4];
		texCoord1_num[m+7]=texCoord1_num[3];

		texCoord1_num[m+8]=texCoord1_num[m+4];
		texCoord1_num[m+9]=texCoord1_num[1];
		texCoord1_num[m+10]=texCoord1_num[m+4];
		texCoord1_num[m+11]=texCoord1_num[3];

	}
	m=9*8;
	texCoord1_num[m+4]=texCoord1_num[0]+10*EWIDTH;
	texCoord1_num[m+5]=texCoord1_num[1];
	texCoord1_num[m+6]=texCoord1_num[m+4];
	texCoord1_num[m+7]=texCoord1_num[3];

}

void ShowInfo::init()
{
	float interval[3];
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	float pf4[4];
	//getGL().glslUpdate();

	pScreen2d->getPos(getGL().viewport[2]*0.5f+50, 10, vertics);
	pScreen2d->getPos(getGL().viewport[2]*0.5f+100, 10, &(vertics[3]));

	//pMatrix->screenPto3dP(getGL().viewport[2]*0.5f+50, 10, vertics);
	//pMatrix->screenPto3dP(getGL().viewport[2]*0.5f+100, 10, &(vertics[3]));

	interval[0]=vertics[3]-vertics[0];
	interval[1]=vertics[4]-vertics[1];
	interval[2]=vertics[5]-vertics[2];

	pScreen2d->getPos(getGL().viewport[2]*0.5f+100, 60, &(vertics[6]));
	//pMatrix->screenPto3dP(getGL().viewport[2]*0.5f+100, 60, &(vertics[6]));
	//pMatrix->screenPto3dP(10, 30, &(vertics[9]));

	vertics[9] =vertics[6]-interval[0];
	vertics[10]=vertics[7]-interval[1];
	vertics[11]=vertics[8]-interval[2];


	//for num
	vertics_num[0] =vertics[3];
	vertics_num[1] =vertics[4];
	vertics_num[2] =vertics[5];

	pScreen2d->getPos(getGL().viewport[2]*0.5f+100+30, 10, &(vertics_num[6]));
	//pMatrix->screenPto3dP(getGL().viewport[2]*0.5f+100+30, 10, &(vertics_num[3]));

	interval[0]=vertics_num[6]-vertics_num[0];
	interval[1]=vertics_num[7]-vertics_num[1];
	interval[2]=vertics_num[8]-vertics_num[2];

	vertics_num[3]  =vertics[6];
	vertics_num[4] =vertics[7];
	vertics_num[5] =vertics[8];

	vertics_num[9] =vertics[3]+interval[0];
	vertics_num[10] =vertics_num[4];
	vertics_num[11] =vertics_num[5];

	int n=3;
	int m=0;
	for(int i=1; i<n; i++)
	{
		m=12*i;
		vertics_num[m]=vertics_num[0]+interval[0]*i;
		vertics_num[m+1]=vertics_num[1];
		vertics_num[m+2]=vertics_num[2];
		vertics_num[m+3]=vertics_num[0]+interval[0]*i;
		vertics_num[m+4]=vertics_num[4];
		vertics_num[m+5]=vertics_num[5];

		vertics_num[m+6]=vertics_num[m]+interval[0];
		vertics_num[m+7]=vertics_num[m+1];
		vertics_num[m+8]=vertics_num[m+2];
		vertics_num[m+9]=vertics_num[m+3]+interval[0];
		vertics_num[m+10]=vertics_num[m+4];
		vertics_num[m+11]=vertics_num[m+5];

	}

	setShowNum(8);
}

void ShowInfo::setShowNum(int num)
{
	showNum=num;
}

void ShowInfo::setNum()
{
	if(showNum==showNumLast)
		return;
	int num_hun, num_ten, num_0;
	num_hun=showNum/100;
	num_ten=showNum%100/10;
	num_0=showNum%10;

	int num1=num_hun*8;

	texCoord1_num2[0]=texCoord1_num[num1+0];
	texCoord1_num2[1]=texCoord1_num[num1+1];
	texCoord1_num2[2]=texCoord1_num[num1+2];
	texCoord1_num2[3]=texCoord1_num[num1+3];

	texCoord1_num2[4]=texCoord1_num[num1+4];
	texCoord1_num2[5]=texCoord1_num[num1+5];
	texCoord1_num2[6]=texCoord1_num[num1+6];
	texCoord1_num2[7]=texCoord1_num[num1+7];

	num1=num_ten*8;
	texCoord1_num2[8]=texCoord1_num[num1+0];
	texCoord1_num2[9]=texCoord1_num[num1+1];
	texCoord1_num2[10]=texCoord1_num[num1+2];
	texCoord1_num2[11]=texCoord1_num[num1+3];

	texCoord1_num2[12]=texCoord1_num[num1+4];
	texCoord1_num2[13]=texCoord1_num[num1+5];
	texCoord1_num2[14]=texCoord1_num[num1+6];
	texCoord1_num2[15]=texCoord1_num[num1+7];

	num1=num_0*8;
	texCoord1_num2[16]=texCoord1_num[num1+0];
	texCoord1_num2[17]=texCoord1_num[num1+1];
	texCoord1_num2[18]=texCoord1_num[num1+2];
	texCoord1_num2[19]=texCoord1_num[num1+3];

	texCoord1_num2[20]=texCoord1_num[num1+4];
	texCoord1_num2[21]=texCoord1_num[num1+5];
	texCoord1_num2[22]=texCoord1_num[num1+6];
	texCoord1_num2[23]=texCoord1_num[num1+7];

}

void ShowInfo::update()
{
	//glDisable (GL_CULL_FACE);
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();

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

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoord1 );//+sizeof(float)*4*3
    glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

    getGL().glslUpdate();

    glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_COLOR);//GL_ONE_MINUS_SRC_ALPHA);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,index);

	setNum();

    glActiveTexture( GL_TEXTURE0);//active_texture_id_num +

    glBindTexture(GL_TEXTURE_2D, mTextureId_num);

    glUniform1i(getGL().pGPNow->tex1Handle, 0);//active_texture_id_num

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics_num);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 2, GL_FLOAT, GL_FALSE, 0, texCoord1_num2 );//+sizeof(float)*4*3
    glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

    glBlendFunc(GL_ONE, GL_ONE);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,18,GL_UNSIGNED_SHORT,index_num);


	glDisable(GL_BLEND);
	//glDisable (GL_CULL_FACE);
}
