#include "GLSLProj.h"

GLSLProj::GLSLProj(char* VertexShader_,char* FragmentShader_)
{
	VertexShader=VertexShader_;
	FragmentShader=FragmentShader_;
	light = new Light(0);
	material = new Material(0);
}
