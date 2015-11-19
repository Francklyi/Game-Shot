#include "MOpenil.h"
#include "log.h"
#include "fileDeal/loadFile.h"
//#include <unistd.h>

MOpenil::MOpenil():w(0),h(0),ID(0),pixels(0)
,format(0)
{
  init();
}

void MOpenil::init()
{ 
	ilInit();
	//iluInit();
	ilEnable(IL_CONV_PAL);
	//ilutEnable(ILUT_OPENGL_CONV);//建立抖动,对那些显卡较差的电脑使用
	LOGE("ilInit: %d",1);
}

bool MOpenil::loadImage(char *fileName)
{
	//init();
  ID=0;
        LOGE("fileName: %s",fileName);
	//if(!ID) ilDeleteImage(1);
	//
        //ILenum en=ilGetError();
        //LOGE("ilLoadImage error1: %d",en);
	//ilGenImages(1,&ID);
	//LOGE("ilLoadImage before: %d",0);
	//ilBindImage(ID);
	//LOGE("ilLoadImage before: %d",1);ilLoadImage(fileName)
  int length=0;
  char *pch = getLoadFile().getDataOfPicFile(fileName, &length);
  //sleep(10);
  LOGE("ilLoadImage after: %d",1);
	if (!ilLoadL(IL_TYPE_UNKNOWN,pch,length))
	{
	    ILenum en=ilGetError();
	    //const char *pce=iluErrorString(en);
	        LOGE("ilLoadImage error2: %d",en);
		return false;
	}
	//sleep(10);
	LOGE("ilLoadImage after: %d",2);
	//
	w=ilGetInteger(IL_IMAGE_WIDTH);
	h=ilGetInteger(IL_IMAGE_HEIGHT);
	format=ilGetInteger(IL_IMAGE_FORMAT);
	pixels=ilGetData();
	LOGE("ilLoadImage after: %d",3);
	//delete[] pch;

	return true;
}

GLuint MOpenil::loadTexture(char *fileName)
{


}

void MOpenil::clearImage()
{
	ilClearImage();
}

MOpenil::~MOpenil()
{

}

MOpenil& getMOL(){
	static MOpenil pMOL;
	return pMOL;
}

//MOpenil *pMOL=new MOpenil();
