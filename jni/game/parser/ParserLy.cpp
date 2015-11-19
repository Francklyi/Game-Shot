#include "ParserLy.h"
#include "fileDeal/loadFile.h"
#include "log.h"
#include "MOpenil.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "TextureFactory.h"
//#include <semaphore.h>

ParserLy::ParserLy():Parser(){

}

ParserLy::~ParserLy(){

}

void ParserLy::parse()
{
	if (!gp) return;
	char folder[]="./sdcard/lyfile/";
	//char * filgpath=combineChars(lyPath,pfileName);
	LOGI("Graph:%s",pfileName);
	char *data=getLoadFile().getDataOfLyFile(pfileName);
	//gp->fileData=data;//getCharsofFile(filgpath);

	//gp->fileData=getCharsofFile(pfileName);
	//gp->fileData=chars;
	if(*(data)++!=76) return;

	//getMtls(&(gp->fileData),gp);
	getMtls(&(data));

	//delete[] data;
	//gp->fileData=0;
	data=0;

}

void ParserLy::getMtls(char **cp) {
	gp->numofMtl = getNewmtlNum(cp);
	gp->pmtls = (Mtl **) malloc((gp->numofMtl) * sizeof(Mtl*) * 4);

	LOGI("numofMtl:%d",gp->numofMtl);

	for (int i=0; i<gp->numofMtl; i++)
	{
		gp->pmtls[i]=new Mtl();
	}
	int i = 0, j = gp->numofMtl;
	while (i < j) {
		getOneMtl(cp, gp->pmtls[i]);
		i++;
	}
	i = 0;

	gp->pointerNum = *(int *) (*cp);
	LOGI("pointerNum:%d",gp->pointerNum);
	(*cp) += sizeof(int);


	gp->vp=new float[(gp->pointerNum) * 9];

	memcpy(gp->vp,*cp, sizeof(float)*(gp->pointerNum) * 9 );

	//gp->vp = (float*) (*cp);
	(*cp) += (sizeof(float) * 3);
	gp->vnp = &(gp->vp[3]);//(float*) (*cp);
	(*cp) += (sizeof(float) * 3);
	gp->vtp = &(gp->vp[6]);//(float*) (*cp);
	(*cp) += (sizeof(float) * 3);


	(*cp) += (gp->pointerNum -1 ) * 9 * sizeof(float);
	gp->jointNump = *(int *) (*cp);
	(*cp) += sizeof(int);
	LOGI("jointNump: %d",gp->jointNump);
	int n1=(gp->pointerNum) * (gp->jointNump);
	LOGI("n: %d",n1);
	gp->weightsp=new float[n1];
	float *pf= (float *) (*cp);
	memcpy(gp->weightsp,*cp, sizeof(float)*n1 );
	/*
	for(int j=0;j<n1;j++)
	{
		LOGI("j: %d", j);
		LOGI("%d: %f",j,pf[j]);
		pf0[j]=pf[j];//*((*cp)+i*sizeof(float));
		//if(pf[i]!=0)
		//
		//LOGI("pf0: %f", pf0[i]);
		//LOGI("pf0: %d, %f");
	}

	gp->weightsp=pf0;
	*/
	(*cp) +=  n1 * sizeof(float);
	//LOGI("gm2.5");
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glEnable(GL_TEXTURE_2D);

	//glGenBuffers(1, gp->ubo);
	//glBindBuffer(GL_ARRAY_BUFFER, gp->ubo[0]);
	//glBufferData(GL_ARRAY_BUFFER,
	//	gp->pointerNum * 9 * sizeof(float) , gp->vp,
	//	GL_STATIC_DRAW);

	// glBindBuffer(GL_ARRAY_BUFFER, ubo[1]);
	// glBufferSubData (GL_ARRAY_BUFFER, 3 * sizeof(float), pointerNum*8*sizeof(float), const GLvoid* data);
	// glBufferData(GL_ARRAY_BUFFER, pointerNum*8*sizeof(float), vnp, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	i = 0;
	//ubois = (GLuint *)malloc(sizeof(GLuint)*numofMtl);
	LOGI("getMtls 1");
	while (i < gp->numofMtl) {
		//glGenBuffers(1, gp->pmtls[i]->uboi);
		gp->pmtls[i]->viL = *(int *) (*cp);

		(*cp) += 4;
		gp->pmtls[i]->vip = new unsigned short[gp->pmtls[i]->viL];
		memcpy( gp->pmtls[i]->vip, (*cp)
				, (gp->pmtls[i]->viL)*sizeof(unsigned short) );
		//gp->pmtls[i]->vip = (unsigned short *) (*cp);

		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->pmtls[i]->uboi[0]);
		//glBufferData(GL_ELEMENT_ARRAY_BUFFER,
		//	gp->pmtls[i]->viL * sizeof(unsigned short), gp->pmtls[i]->vip,
		//	GL_STATIC_DRAW);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		(*cp) += 2 * ((gp->pmtls[i]->viL));

		if ((gp->pmtls[i]->Lmap_Kd) != 0) {
			LOGI("getMtls 2");
			gp->pmtls[i]->mTextureId =pTexFactory->getTexID(gp->pmtls[i]->map_Kd);
			//setTextureB(gp->pmtls[i]->map_Kd);
			LOGI("getMtls 3");
			gp->pmtls[i]->active_texture_id = getGL().getATextureID();
			glBindTexture(GL_TEXTURE_2D, gp->pmtls[i]->mTextureId);

		}
		i++;
	}

	LOGI("getMtls done");
}

void ParserLy::getOneMtl(char **cp, Mtl *pMtl) {
	float *pTemp;

	pMtl->Lmap_Ka = (int) *((*cp)++);

	char *str=new char[pMtl->Lmap_Ka];
	strcpy( str, (*cp) );
	pMtl->map_Ka = str;
	(*cp) += pMtl->Lmap_Ka;

	pMtl->Lmap_Kd = (int) *((*cp)++);

	char *str2=new char[pMtl->Lmap_Kd];
	strcpy(str2,(*cp));
	pMtl->map_Kd = str2;
	(*cp) += pMtl->Lmap_Kd;
	LOGI("Map: %s", pMtl->map_Kd);

	pMtl->Ns = *(float*) (*cp);
	(*cp) += sizeof(float);
	LOGI("Ns: %f", pMtl->Ns);
	pMtl->Ni = *(float*) (*cp);
	(*cp) += sizeof(float);
	pMtl->d = *(float*) (*cp);
	(*cp) += sizeof(float);
	pMtl->Tr = *(float*) (*cp);
	(*cp) += sizeof(float);
	pMtl->illum = *(int*) (*cp);
	(*cp) += sizeof(int);
	pTemp=(float*) (*cp);
	pMtl->Tf[0] = pTemp[0];
	pMtl->Tf[1] = pTemp[1];
	pMtl->Tf[2] = pTemp[2];
	pMtl->Tf[3] = pTemp[3];
	(*cp) += 16;
	pTemp=(float*) (*cp);
	pMtl->Ka[0] = pTemp[0];
	pMtl->Ka[1] = pTemp[1];
	pMtl->Ka[2] = pTemp[2];
	pMtl->Ka[3] = pTemp[3];
	//pMtl->Ka = (float*) (*cp);
	(*cp) += 16;
	pTemp=(float*) (*cp);
	pMtl->Kd[0] = pTemp[0];
	pMtl->Kd[1] = pTemp[1];
	pMtl->Kd[2] = pTemp[2];
	pMtl->Kd[3] = pTemp[3];
	//pMtl->Kd = (float*) (*cp);
	(*cp) += 16;
	pTemp=(float*) (*cp);
	pMtl->Ks[0] = pTemp[0];
	pMtl->Ks[1] = pTemp[1];
	pMtl->Ks[2] = pTemp[2];
	pMtl->Ks[3] = pTemp[3];
	//pMtl->Ks = (float*) (*cp);
	(*cp) += 16;
	pTemp=(float*) (*cp);
	pMtl->Ke[0] = pTemp[0];
	pMtl->Ke[1] = pTemp[1];
	pMtl->Ke[2] = pTemp[2];
	pMtl->Ke[3] = pTemp[3];
	//pMtl->Ke = (float*) (*cp);
	(*cp) += 16;
	pMtl->mTextureId = 0;
}

GLuint ParserLy::setTexture(char **cp) {
	GLuint texName;
	int w = getLoadFile().getInt(cp);
	int h = getLoadFile().getInt(cp);
	int imgLength = getLoadFile().getInt(cp);
	glGenTextures(1, &texName);
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	// glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
		*cp);
	//glGenerateMipmap (GL_TEXTURE_2D);
	return texName;
}


GLuint ParserLy::setTexture(BYTE *pixels,int w, int h,unsigned int format)
{
        GLuint texName;
        glGenTextures(1, &texName);
        glBindTexture(GL_TEXTURE_2D, texName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_REPEAT
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
         //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );


    	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_LINEAR_MIPMAP_LINEAR);
    	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_LINEAR_MIPMAP_LINEAR);
    	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
    	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);

        //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
              pixels);
        //glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, w, h, 0, GL_UNSIGNED_BYTE,
         //       pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
        return texName;
}

GLuint ParserLy::setTextureB(char *map_Kd) {
	GLuint texName11;
	//char folder[]="./sdcard/gamepics/";//"./sdcard/textureFiles/";
	//char *filgpath=combineChars(texPath,map_Kd);
	//LOGI("tex0");
	if(!getMOL().loadImage(map_Kd)) return 0;
	//LOGI("tex1");
	//char * chars;
	//chars=getCharsofFile(filgpath);
	//texName11=setTexture(&chars);
	texName11=setTexture(getMOL().pixels,getMOL().w,getMOL().h,getMOL().format);
	getMOL().clearImage();
	//LOGI("tex2");
	return texName11;
}

ParserLy *pParserLy0=new ParserLy();
