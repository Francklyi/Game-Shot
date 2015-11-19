#include "GLSL_GAOSI_X.h"

GLSL_GAOSI_X::GLSL_GAOSI_X(char* VertexShader_,char* FragmentShader_):GLSLProj(VertexShader_,FragmentShader_)
{

}

void GLSL_GAOSI_X::initSelf(){
	imgWHandle = glGetUniformLocation(program, "imgW");
}
