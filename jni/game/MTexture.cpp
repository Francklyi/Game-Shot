#include "MTexture.h"
#include "parser/ParserLy.h"
#include "fileDeal/loadFile.h"
#include <GLES2/gl2.h>
#include <GLES/gl.h>
#include <GLES2/gl2ext.h>
#include "ETCHeader.h"
#include "log.h"

MTexture::MTexture()
{
	mTextureId=0;
	active_texture_id=0;
}

void MTexture::setFileName(char *fileName)
{
	strFileName=string(fileName);
}

void MTexture::load()
{

	//char texFile[]="num1.png\0";
	mTextureId =pParserLy0->setTextureB((char*)strFileName.c_str());
	active_texture_id = getGL().getATextureID();
	//glBindTexture(GL_TEXTURE_2D, mTextureId);




/*
    GLuint texName;
    glGenTextures(1, &texName);
    //glActiveTexture(  GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texName);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_REPEAT
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
     //glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

    //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    //glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
     //     pixels);
    //glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES, 2048, 2048, 0, GL_UNSIGNED_BYTE,
    //		pch);
   // int length=0;
   // char *pch=pLoadFile->getDataOfPicFile((char*)strFileName.c_str(), &length);
    //ETCHeader loadedETCHeader = ETCHeader((unsigned char*)pch);
   // glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES,
    //         loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
    //         loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
    //         pch + 16);

    int length=0;
    char *pch=0;
    ETCHeader loadedETCHeader;

    length=0;
    pch=pLoadFile->getDataOfPicFile("sky6_mip_0.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 0, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_1.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 1, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_2.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 2, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_3.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 3, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_4.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 4, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_5.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 5, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_6.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 6, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_7.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 7, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_8.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 8, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_9.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 9, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);


    pch=pLoadFile->getDataOfPicFile("sky6_mip_10.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 10, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    pch=pLoadFile->getDataOfPicFile("sky6_mip_11.pkm", &length);
    loadedETCHeader = ETCHeader((unsigned char*)pch);
    glCompressedTexImage2D(GL_TEXTURE_2D, 11, GL_ETC1_RGB8_OES,
             loadedETCHeader.getWidth(), loadedETCHeader.getHeight(), 0,
             loadedETCHeader.getPaddedWidth() * loadedETCHeader.getPaddedHeight() >> 1,
             pch + 16);

    //LOGI("pkm w, h: %d, %d",loadedETCHeader.getWidth(), loadedETCHeader.getHeight());
    //free(textureData);

    glGenerateMipmap(GL_TEXTURE_2D);


	mTextureId =texName;
	active_texture_id = getGL().getATextureID();
	//glBindTexture(GL_TEXTURE_2D, mTextureId);
*/
}
