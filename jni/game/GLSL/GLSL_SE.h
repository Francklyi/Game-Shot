#pragma once
#pragma once

#include "GLSLProj.h"

class GLSL_SE: public GLSLProj
{
public:
  GLSL_SE();
  GLSL_SE(char* VertexShader_,char* FragmentShader_):GLSLProj(VertexShader_,FragmentShader_)
  {

  }
  ~GLSL_SE();


};
