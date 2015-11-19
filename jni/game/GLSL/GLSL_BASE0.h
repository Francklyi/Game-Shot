#pragma once
#include <GLES2/gl2.h>
#include "matrix.h"
#include <vector>

using std::vector;

class GLSL_BASE
{
public:
  GLSL_BASE();

  ~GLSL_BASE();

  GLuint init(const char* VertexShader,const char* FragmentShader);// for create and link

  GLuint loadShader(GLenum shaderType, const char* pSource);

  GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

  void setHandles();

  void printGLString(const char *name, GLenum s);

  GLuint getProgramHandle(void);

  void chooseMatrix(int m);

  void setLookAt( float eyeX, float eyeY, float eyeZ,
      float centerX, float centerY, float centerZ,
      float upX, float upY,float upZ);

  void ortho( float left, float right, float bottom, float top,
      float near, float far);

  void frustum( float left, float right, float bottom, float top,
      float near, float far);

  void setIdentity(void);

  void perspective(float fovy, float aspect, float zNear, float zFar);

  void scale(float x, float y, float z);

  void translate(float x, float y, float z);

  void rotate(float a, float x, float y, float z);

  void multiplyMVMatrix();

  void multiplyMPMatrix();

  void multMatrix(float *matrix);

  void sendMatrix();

  void attribPointer(int handle, GLuint ubo,
      GLsizei stride, const GLvoid* pointer);

  void useProgram();

  void popMatrix();

  void pushMatrix();

  void glslUpdate();

  unsigned getATextureID();

  void setLight();

  void setHasTex(float has);

  unsigned TextureID[32];

  const static int VMATRIX = 1;
  const static int MMATRIX = 2;
  const static int MVMATRIX = 3;
  const static int PMATRIX = 4;
  const static int MVPMATRIX = 5;
  //protected:
  //private:
  static int whichMatrix; // 1->ViewMatrix , 2->ModelMatrix , 3->ModelViewMatrix , 4->projection Matrix
  static float* VMatrix;
  static float* MMatrix;
  static float* MVMatrix;
  static float* NormalMatrix;
  static float* PMatrix;
  static float* MVPMatrix;

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

  vector<float *> mMatrixQueue; //

};

