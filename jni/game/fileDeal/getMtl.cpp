#include "getMtl.h"
#include "Entity.h"
char * getCharsofFile(char *FileName){
	FILE * fp;
	if((fp=fopen(FileName,"rb"))==NULL) {
		return 0;
	}
	fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    const long longBytes=ftell(fp);// longBytes就是文件的长度
	char * cp=(char *)malloc(longBytes);
	fseek(fp,0,SEEK_SET);
	fread(cp,1,longBytes,fp);
	fclose(fp);
    return cp;
}

int getNewmtlNum(char **cp){
	return (int)*((*cp)++);
}

int getInt(char **cp){
	int i=0;
	i+=*((*cp)++);
	i+=(*((*cp)++))*128;
	i+=(*((*cp)++))*16384;
	i+=(*((*cp)++))*2097152;
	return i;
}

void getOneMtl(char **cp,struct Newmtl *Np){
	Np->Lmap_Ka=(*cp)++;
	(*cp)+=*Np->Lmap_Ka;
	Np->Lmap_Kd=(*cp)++;
	(*cp)+=*Np->Lmap_Kd;
	Np->Ns=(float*)(*cp);
	(*cp)+=4;
	Np->Ni=(float*)(*cp);
	(*cp)+=4;
	Np->d=(float*)(*cp);
	(*cp)+=4;
	Np->Tr=(float*)(*cp);
	(*cp)+=4;
	Np->illum=(int*)(*cp);
	(*cp)+=4;
	Np->Tf=(float*)(*cp);
	(*cp)+=16;
	Np->Ka=(float*)(*cp);
	(*cp)+=16;
	Np->Kd=(float*)(*cp);
	(*cp)+=16;
	Np->Ks=(float*)(*cp);
	(*cp)+=16;
	Np->Ke=(float*)(*cp);
	(*cp)+=16;
	Np->mTextureId=0;
}


int getBufferLength(char **cp){
	return *(int*)(*cp)++;
}

void getFloatBuffers(char **cp,struct Newmtl *np){
	np->VTNp->lV=getInt(cp);
	np->VTNp->lN=getInt(cp);
	np->VTNp->lT=getInt(cp);
	np->VTNp->vp=(float*)*cp;
	*cp+=np->VTNp->lV;
	np->VTNp->np=(float*)*cp;
	*cp+=np->VTNp->lN;
	np->VTNp->tp=(float*)*cp;
	*cp+=np->VTNp->lT;
}

GLuint setTexture(char **cp){
	GLuint texName;
	int w=getInt(cp);
	int h=getInt(cp);
	int imgLength=getInt(cp);
	//glActiveTexture(GL_TEXTURE0);
	//LOGI("W %d H %d ", w, h);
	//
    glGenTextures(1, &texName);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w,
                 h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 *cp);
	//LOGI("Postion %d",188);
	//(*cp)+=imgLength;
	return texName;
}

char * combineChars(char * A,char *B){
	int n=0,m=0;
	while((*(A+n))!='\n') n++;
	while((*(B+m))!='\n') m++;
	char *newAB=(char*)malloc(m+n);
	n=0;

	while(A[n]!='\n'){
		newAB[n]=A[n];
		n++;
	}
	//LOGI("B %d %d \n",m ,B[m]);
	m=0;
	while(B[m]!='\n'){
		newAB[n]=B[m];
		n++;
		m++;
	}
	return newAB;
}

int setTextureB(char *map_Kd){
	GLuint texName11;
	//LOGE("Could not link program:\n%d\n", 155);
	char folder[]="./sdcard/textureFiles/\n";
	char *filePath=combineChars(folder,map_Kd);
    char * chars=getCharsofFile(filePath);
    texName11=setTexture(&chars);
    return texName11;
}

void getMtls(char **cp,struct EntityData *ep){
	ep->numofNewMtl=getNewmtlNum(cp);
	ep->np=(struct Newmtl *)malloc((ep->numofNewMtl)*sizeof(struct Newmtl)*4);
	int i=0 , j=ep->numofNewMtl;
	while(i<j)
	{
		getOneMtl(cp,&(ep->np[i]));
		i++;
	}
	i=0;
	//设置像素存储格式及开启纹理功能
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glEnable(GL_TEXTURE_2D);
	while(i<j)
	{
		ep->np[i].VTNp=(struct VTNbuffer*)malloc(sizeof(struct VTNbuffer)*4);
		getFloatBuffers(cp,&(ep->np[i]));
		if(((int)*(ep->np[i].Lmap_Kd))!=0){
			GLuint texName_=setTextureB((ep->np[i].Lmap_Kd)+1);//setTexture(cp);
			ep->np[i].mTextureId=texName_;
            //glBindTexture(GL_TEXTURE_2D,ep->np[i].mTextureId);
			}
		i++;
	}
}

void getMtls2(char **cp,struct EntityData *ep){
	ep->numofNewMtl=getNewmtlNum(cp);

	ep->np=(Newmtl *)malloc((ep->numofNewMtl)*sizeof(Newmtl)*4);
	int i=0 , j=ep->numofNewMtl;
	while(i<j)
	{
		getOneMtl(cp,&(ep->np[i]));
		i++;
	}
	i=0;
	//设置像素存储格式及开启纹理功能
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	//glEnable(GL_TEXTURE_2D);
	ep->pointerNum=*(int *)(*cp);
	(*cp)+=4;
	ep->vp=(float*)(*cp);
	LOGI("ep->vp %f\n", ep->vp[0]);
	(*cp)+=(4*3);
	ep->vnp=(float*)(*cp);
	LOGI("ep->vnp %f\n", ep->vnp[0]);
	(*cp)+=(4*3);
	ep->vtp=(float*)(*cp);
	LOGI("ep->vtp %f\n", ep->vtp[0]);
	(*cp)+=((ep->pointerNum))*8*4-4*6;
	ep->jointNump=*(int *)(*cp);
	(*cp)+=4;
	ep->weightsp=(float *)(*cp);
	(*cp)+=((ep->pointerNum))*((ep->jointNump))*4;

	while(i<j)
	{
		ep->np[i].viL=*(int *)(*cp);

		(*cp)+=4;
		ep->np[i].vip=(unsigned short *)(*cp);
		LOGI("ep->np[i].vip %d\n", ep->np[i].vip[1]);
		(*cp)+=2*((ep->np[i].viL));
		if(((int)*(ep->np[i].Lmap_Kd))!=0){
			GLuint texName_=setTextureB((ep->np[i].Lmap_Kd)+1);
			ep->np[i].mTextureId=texName_;
//			LOGI("ep->np[i].mTextureId %d\n", ep->np[i].mTextureId);
            //glBindTexture(GL_TEXTURE_2D,ep->np[i].mTextureId);
			}
		i++;
	}
}

void loadModel(EntityData *ep,char * fileName){
    //jiliye3_1.ly//"my_scene1_7.ly"
	char folder[]="./sdcard/lyfile/\n";
	char * filePath=combineChars(folder,fileName);
	ep->fileData=getCharsofFile(filePath);
	//ep->fileData=chars;
	if(*(ep->fileData)++!=76) return;

	getMtls(&(ep->fileData),ep);

}

void loadModel2(EntityData *ep,char * fileName){
    //jiliye3_1.ly//"my_scene1_7.ly"
	char folder[]="./sdcard/lyfile/\n";
	char * filePath=combineChars(folder,fileName);
	ep->fileData=getCharsofFile(filePath);
	//ep->fileData=chars;
	if(*(ep->fileData)++!=76) return;

	getMtls2(&(ep->fileData),ep);
}

void loadAnimation(EntityData *ep,char * fileName){
//	LOGI("loadAnimation\n");
	char folder[]="./sdcard/animationFile/\n";
	char * filePath=combineChars(folder,fileName);
	char * chars=getCharsofFile(filePath);
	ep->animations=(Animation*)malloc(sizeof(*(ep->animations)));
	ep->animations->frameNum=(int*)chars;
//	LOGI("frameNum %d\n", *(ep->animations->frameNum));
	chars+=4;
	ep->animations->frameTime=(float*)chars;
//	LOGI("frameTime %f\n", *(ep->animations->frameTime));
	chars+=4*(*(ep->animations->frameNum));
	ep->animations->jointNum=(int*)chars;
//	LOGI("jointNum %d\n", *(ep->animations->jointNum));
	chars+=4;
	ep->animations->joints=(Joint*)malloc(sizeof(Joint)*(*(ep->animations->jointNum)));

	for(int i=0;i<*(ep->animations->jointNum);i++){
		ep->animations->joints[i].sn=i;
		ep->animations->joints[i].posf[0]=(*((float*)chars));
		chars+=4;
		ep->animations->joints[i].posf[1]=(*((float*)chars));
		chars+=4;
		ep->animations->joints[i].posf[2]=(*((float*)chars));
		chars+=4;
	//	LOGI("posf[0] %f\n", ep->animations->joints[i].posf[0]);
	//	LOGI("posf[1] %f\n", ep->animations->joints[i].posf[1]);
	//	LOGI("posf[2] %f\n", ep->animations->joints[i].posf[2]);
		ep->animations->joints[i].Matrix=(float*)chars;
		chars+=16*4*(*(ep->animations->frameNum));
	}
	//ep->animations->jointMatrixs=(float*)chars;
	ep->animations->frame=1;
	ep->animations->direction=1;
	ep->animations->change_stay=1;
}

void loadIniFile(moveMents *mMentsp,char * iniFilep){
	char folder[]="./sdcard/\n";
	char * filePath=combineChars(folder,iniFilep);
	char * chars=getCharsofFile(filePath);
}
