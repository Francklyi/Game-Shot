#include "GLSL_BLOOM.h"

GLSL_BLOOM::GLSL_BLOOM(char* VertexShader_,char* FragmentShader_):GLSLProj(VertexShader_,FragmentShader_)
{

}

void GLSL_BLOOM::initSelf(){
	tex2Handle = glGetUniformLocation(program, "Tex2");
}
