#pragma once

#include "GLSLProj.h"

class GLSL_getHLight : public GLSLProj
{
public:
	GLuint imgHHandle;
	GLuint imgWHandle;
	GLuint aveHandle;
	GLuint tex2Handle;

	GLSL_getHLight(char* VertexShader_,char* FragmentShader_);
	virtual void initSelf();
};
