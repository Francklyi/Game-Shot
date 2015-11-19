#pragma once

#include "GLSLProj.h"

class GLSL_GAOSI_X : public GLSLProj
{
public:
	GLuint imgWHandle;


	GLSL_GAOSI_X(char* VertexShader_,char* FragmentShader_);
	virtual void initSelf();
};
