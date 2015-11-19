#include "GLSL_GAOSI_Y.h"

GLSL_GAOSI_Y::GLSL_GAOSI_Y(char* VertexShader_,char* FragmentShader_):GLSLProj(VertexShader_,FragmentShader_)
{

}

void GLSL_GAOSI_Y::initSelf(){
	imgHHandle = glGetUniformLocation(program, "imgH");
}
