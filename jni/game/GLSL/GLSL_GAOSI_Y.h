#pragma once

#include "GLSLProj.h"

class GLSL_GAOSI_Y : public GLSLProj
{
public:
	GLuint imgHHandle;


	GLSL_GAOSI_Y(char* VertexShader_,char* FragmentShader_);
	virtual void initSelf();
};
