#ifndef _GETMTL_H_
#define _GETMTL_H_

#include "mygame1jni.h"
#include <string.h>
#include <stdio.h>

int getNewmtlNum(char **cp);

//int getInt(char **cp);

void getOneMtl(char **cp,struct Newmtl *Np);

//char * getCharsofFile(char *FileName);

int getBufferLength(char **cp);

void getFloatBuffers(char **cp,struct Newmtl *np);

GLuint setTexture(char **cp);

char * combineChars(char * A,char *B);

int setTextureB(char *map_Kd);

void getMtls(char **cp,struct EntityData *ep);

void loadModel(struct EntityData *ep , char * fileName);

void loadModel2(struct EntityData *ep , char * fileName);

void loadAnimation(EntityData *ep,char * fileName);

#endif //_GETMTL_H_
