#pragma once
#include "GLSL/GLSL.h"
#include <string>
using std::string;

class MTexture
{
public:
	string strFileName;
	GLuint mTextureId;
	unsigned active_texture_id;

public:
	MTexture();
	void setFileName(char *fileName);
	void load();
};
