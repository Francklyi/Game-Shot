#include "GLSL_getHLight.h"

GLSL_getHLight::GLSL_getHLight(char* VertexShader_,char* FragmentShader_):GLSLProj(VertexShader_,FragmentShader_)
{

}

void GLSL_getHLight::initSelf(){
	imgHHandle = glGetUniformLocation(program, "imgH");
	imgWHandle = glGetUniformLocation(program, "imgW");
	aveHandle = glGetUniformLocation(program, "AveLum");
	tex2Handle = glGetUniformLocation(program, "Tex2");
}
