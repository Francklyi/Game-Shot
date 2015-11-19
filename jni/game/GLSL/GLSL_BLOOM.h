#pragma once

#include "GLSLProj.h"

class GLSL_BLOOM : public GLSLProj
{
public:
	GLuint tex2Handle;


	GLSL_BLOOM(char* VertexShader_,char* FragmentShader_);
	virtual void initSelf();
};
