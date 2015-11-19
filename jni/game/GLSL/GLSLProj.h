#pragma once
#include <GLES2/gl2.h>
#include "light.h"
#include "material.h"

class GLSLProj
{
public:
	GLuint program;

	GLuint MVMatrixHandle;
	GLuint PMatrixHandle;
	GLuint MVPMatrixHandle;

	GLuint MVPLightMatrixHandle;

	GLuint hasTextureHandle;
	GLuint NormalMatrixHandle;

	GLuint gvPositionHandle;
	GLuint gvNormalHandle;
	GLuint gvTexCoordHandle;
	GLuint gvColorHandle;

	GLuint funcTypeHandle;

	GLuint timeHandle;

	GLuint tex1Handle;

	char* VertexShader;
	char* FragmentShader;

	Light *light;

	Material *material;

public:
	GLSLProj(char* VertexShader_,char* FragmentShader_);
	virtual void initSelf(){}
};
