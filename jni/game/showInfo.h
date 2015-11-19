#pragma once
#include "GLSL/GLSL.h"

class ShowInfo
{
public:
	float *vertics;
	unsigned short *index;
	float *texCoord1;
	GLuint mTextureId;

	unsigned active_texture_id;
	float pColor[3];

	float *vertics_num;
	unsigned short *index_num;
	float *texCoord1_num;

	float *texCoord1_num2;

	GLuint mTextureId_num;

	unsigned active_texture_id_num;

	int showNum;

	int showNumLast;

public:
	ShowInfo();

	void init();

	void setShowNum(int num);

	void setNum();

	void update();

};
