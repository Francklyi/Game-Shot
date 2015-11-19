#include "GLSL_BASE0.h"

#include <stdlib.h>
#include "log.h"


#define STRINGIFY(A) #A
//#include "shader/Shader.frag"
//#include "shader/Shader.vert"

//#include "shader/Shader2.frag"
//#include "shader/Shader2.vert"
////////////////////////////////////////////////////////////////////////
// Name:       GLSL_BASE::GLSL_BASE()
// Purpose:    Implementation of GLSL_BASE::GLSL_BASE()
// Return:
////////////////////////////////////////////////////////////////////////
int GLSL_BASE::whichMatrix=0;
float* GLSL_BASE::MMatrix=(float*)malloc(16*sizeof(float));
float* GLSL_BASE::VMatrix=(float*)malloc(16*sizeof(float));
float* GLSL_BASE::MVMatrix=(float*)malloc(16*sizeof(float));
float* GLSL_BASE::PMatrix=(float*)malloc(16*sizeof(float));
float* GLSL_BASE::MVPMatrix=(float*)malloc(16*sizeof(float));
float* GLSL_BASE::NormalMatrix=(float*)malloc(16*sizeof(float));

GLSL_BASE::GLSL_BASE():
mMatrixQueue(0)
{
  chooseMatrix(VMATRIX);
  setIdentity();
  chooseMatrix(MMATRIX);
  setIdentity();
  chooseMatrix(MVMATRIX);
  setIdentity();
  chooseMatrix(PMATRIX);
  setIdentity();
  chooseMatrix(MVPMATRIX);
  setIdentity();

  for(int i=0; i<32; i++) TextureID[i] = 0;

  printGLString("Version", GL_VERSION);
  printGLString("Vendor", GL_VENDOR);
  printGLString("Renderer", GL_RENDERER);
  printGLString("Extensions", GL_EXTENSIONS);
}


void GLSL_BASE::printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

GLuint GLSL_BASE::init(const char* VertexShader,const char* FragmentShader)
{
  program = createProgram(VertexShader, FragmentShader);
  if (!program) {
      LOGE("Could not create program.");
      return 0;
  }

  return program;
}


////////////////////////////////////////////////////////////////////////
// Name:       GLSL_BASE::loadShader()
// Purpose:    Implementation of GLSL_BASE::loadShader()
// Return:     GLuint
////////////////////////////////////////////////////////////////////////

GLuint GLSL_BASE::loadShader(GLenum shaderType, const char* pSource)
{
   // TODO : implement
  GLuint shader = glCreateShader(shaderType);
      if (shader) {
          glShaderSource(shader, 1, &pSource, NULL);
          glCompileShader(shader);
          GLint compiled = 0;
          glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
          if (!compiled) {
              GLint infoLen = 0;
              glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
              if (infoLen) {
                  char* buf = (char*) malloc(infoLen);
                  if (buf) {
                      glGetShaderInfoLog(shader, infoLen, NULL, buf);
                      LOGE("Could not compile shader %d:\n%s\n",
                              shaderType, buf);
                      free(buf);
                  }
                  glDeleteShader(shader);
                  shader = 0;
              }
          }
      }
      return shader;
}

////////////////////////////////////////////////////////////////////////
// Name:       GLSL_BASE::createProgram()
// Purpose:    Implementation of GLSL_BASE::createProgram()
// Return:     GLuint
////////////////////////////////////////////////////////////////////////

GLuint GLSL_BASE::createProgram(const char* pVertexSource, const char* pFragmentSource)
{
   // TODO : implement
  GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
  if (!vertexShader) {
      return 0;
  }

  GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
  if (!pixelShader) {
      return 0;
  }

  GLuint program = glCreateProgram();
  if (program) {
      glAttachShader(program, vertexShader);
     // checkGlError("glAttachShader");
      glAttachShader(program, pixelShader);
    //  checkGlError("glAttachShader");
      glLinkProgram(program);
      GLint linkStatus = GL_FALSE;
      glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
      if (linkStatus != GL_TRUE) {
          GLint bufLength = 0;
          glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
          if (bufLength) {
              char* buf = (char*) malloc(bufLength);
              if (buf) {
                  glGetProgramInfoLog(program, bufLength, NULL, buf);
                  LOGE("Could not link program:\n%s\n", buf);
                  free(buf);
              }
          }
          glDeleteProgram(program);
          program = 0;
      }
  }
  return program;
}

void GLSL_BASE::setHandles()
{

  gvPositionHandle   = glGetAttribLocation(program, "VertexPosition");
  gvNormalHandle     = glGetAttribLocation(program, "VertexNormal");
  gvTexCoordHandle   = glGetAttribLocation(program, "VertexTexCoord");
  MVMatrixHandle     = glGetUniformLocation(program, "ModelViewMatrix" );
  NormalMatrixHandle = glGetUniformLocation(program, "NormalMatrix" );
 // PMatrixHandle      = glGetUniformLocation(program, "ProjectionMatrix" );
  MVPMatrixHandle    = glGetUniformLocation(program, "MVP" );

  MVPLightMatrixHandle= glGetUniformLocation(program, "MVPLight" );

  hasTextureHandle   = glGetUniformLocation(program, "hasTexture" );

  LOGI("glGetAttribLocation(\"VertexPosition\") = %d\n",
          gvPositionHandle);
  LOGI("glGetAttribLocation(\"VertexNormal\") = %d\n",
              gvNormalHandle);
  LOGI("glGetAttribLocation(\"VertexTexCoord\") = %d\n",
      gvTexCoordHandle);
  LOGI("glGetUniformLocation(\"ModelViewMatrix\") = %d\n",
              MVMatrixHandle);
  LOGI("glGetUniformLocation(\"MVP\") = %d\n",
              MVPMatrixHandle);

  LOGI("glGetUniformLocation(\"NormalMatrix\") = %d\n",
      NormalMatrixHandle);

  LOGI("glGetUniformLocation(\"MVPLight\") = %d\n",
      MVPLightMatrixHandle);

  LOGI("glGetUniformLocation(\"hasTexture\") = %d\n",
      hasTextureHandle);

}
////////////////////////////////////////////////////////////////////////
// Name:       GLSL_BASE::getProgramHandle()
// Purpose:    Implementation of GLSL_BASE::getProgramHandle()
// Return:     int
////////////////////////////////////////////////////////////////////////

GLuint GLSL_BASE::getProgramHandle(void)
{
  return program;
}

void GLSL_BASE::setLookAt( float eyeX, float eyeY, float eyeZ,
                float centerX, float centerY, float centerZ,
                float upX, float upY,float upZ)
{
  Matrix::setLookAtM(VMatrix, 0, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
}

void GLSL_BASE::ortho( float left, float right, float bottom, float top,
            float near, float far)
{
  Matrix::orthoM(PMatrix, 0, left, right, bottom, top, near, far);
}

void GLSL_BASE::frustum( float left, float right, float bottom, float top,
              float near, float far)
{
  Matrix::frustumM(PMatrix, 0, left, right, bottom, top, near, far);
}

void GLSL_BASE::chooseMatrix(int m)
{
  whichMatrix=m;
}

void GLSL_BASE::setIdentity(void)
{
  if(whichMatrix==VMATRIX) //VMatrix
    {
      Matrix::setIdentityM(VMatrix,0);
    }else if(whichMatrix==MMATRIX) //MMatrix
      {
        Matrix::setIdentityM(MMatrix,0);
        for(int i=0;i<mMatrixQueue.size();i++)
          free(mMatrixQueue[i]);
        mMatrixQueue.clear();

      }else if(whichMatrix==MVMATRIX) //MVMatrix
      {
          Matrix::setIdentityM(MVMatrix,0);
      }else if(whichMatrix==PMATRIX) //MVPMatrix
        {
          Matrix::setIdentityM(PMatrix,0);
        }else if(whichMatrix==MVPMATRIX) //MVPMatrix
          {
            Matrix::setIdentityM(MVPMatrix,0);
          }
}

void GLSL_BASE::perspective(float fovy, float aspect, float zNear, float zFar)
{
  Matrix::perspectiveM(PMatrix, 0, fovy, aspect, zNear, zFar);
}

void GLSL_BASE::scale(float x, float y, float z)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        Matrix::scaleM(VMatrix,0, x, y, z);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          Matrix::scaleM(MMatrix,0, x, y, z);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
        {
            Matrix::scaleM(MVMatrix,0, x, y, z);
        }
}

void GLSL_BASE::translate(float x, float y, float z)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        Matrix::translateM(VMatrix,0, x, y, z);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          Matrix::translateM(MMatrix,0, x, y, z);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
        {
            Matrix::translateM(MVMatrix,0, x, y, z);
        }
}

void GLSL_BASE::rotate(float a, float x, float y, float z)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        Matrix::setRotateM(VMatrix,0, a,x, y, z);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          Matrix::setRotateM(MMatrix,0, a, x, y, z);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
          {
            Matrix::setRotateM(MVMatrix,0, a, x, y, z);
          }
}

void GLSL_BASE::pushMatrix()
{
  float *matrix_now=(float*)malloc(sizeof(float)*16);

  Matrix::arrayCopy(MMatrix, 0, matrix_now, 0, 16);
  mMatrixQueue.push_back(matrix_now);
}

void GLSL_BASE::popMatrix()
{
  if(mMatrixQueue.size())
    {
      delete MMatrix;
      MMatrix=mMatrixQueue[mMatrixQueue.size()-1];
      mMatrixQueue.pop_back();
    }
}

void GLSL_BASE::multMatrix(float *matrix)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        Matrix::multiplyMM(VMatrix, 0, VMatrix, 0, matrix, 0);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          Matrix::multiplyMM(MMatrix, 0, MMatrix, 0, matrix, 0);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
          {
            Matrix::multiplyMM(MVMatrix, 0, MVMatrix, 0, matrix, 0);
          }
}

void GLSL_BASE::multiplyMVMatrix()
{
  Matrix::setIdentityM(MVMatrix,0);
  Matrix::multiplyMM(MVMatrix, 0, VMatrix, 0, MMatrix, 0);
}

void GLSL_BASE::multiplyMPMatrix()
{
  Matrix::setIdentityM(MVPMatrix,0);
  Matrix::multiplyMM(MVPMatrix, 0, PMatrix, 0, MVMatrix, 0);
}

void GLSL_BASE::sendMatrix()
{
    if(whichMatrix==GLSL_BASE::MVMATRIX) //MVMatrix
      {
          glUniformMatrix4fv( MVMatrixHandle, 1, false, MVMatrix);
      }else if(whichMatrix==GLSL_BASE::MVPMATRIX) //MVPMatrix
          {
            glUniformMatrix4fv( MVPMatrixHandle, 1, false, MVPMatrix);
          }
  //
}

void GLSL_BASE::attribPointer(int handle, GLuint ubo,
    GLsizei stride, const GLvoid* pointer)
{
    glBindBuffer(GL_ARRAY_BUFFER, ubo);
    glVertexAttribPointer(handle, 3, GL_FLOAT, GL_FALSE, stride, pointer);
    glEnableVertexAttribArray(handle);
}

unsigned GLSL_BASE::getATextureID()
{
  for(int i=0; i<32; i++)
    {
      if(TextureID[i]==0)
        {
          TextureID[i]=i+1;
          return i;
        }

    }
  return 0;
}

void GLSL_BASE::setHasTex(float has)
{
  glUniform1f( hasTextureHandle , has );
}

/*
 * ÉèÖÃ
 */
void GLSL_BASE::glslUpdate()
{

  GLfloat bias[16] = {
            0.5, 0.0, 0.0, 0.0,
            0.0, 0.5, 0.0, 0.0,
            0.0, 0.0, 0.5, 0.0,
            0.5, 0.5, 0.5, 1.0};

      //need to calculate the light matrixs
      //LOGI("4");
  float* tempMatrix=(float*)malloc(16*sizeof(float));
  float* tempMatrix2=(float*)malloc(16*sizeof(float));
  Matrix::setIdentityM(tempMatrix,0);
  Matrix::setIdentityM(tempMatrix2,0);

  for(int i;i<3;i++)
    {
      tempMatrix[i+1]=MVMatrix[i*4+1];
      tempMatrix[i+2]=MVMatrix[i*4+2];
      tempMatrix[i+3]=MVMatrix[i*4+3];
    }

  Matrix::invertM(tempMatrix2,0,tempMatrix,0);

  //Matrix::setIdentityM(NormalMatrix,0);

  Matrix::transposeM(NormalMatrix,0,tempMatrix,0);

  glUniformMatrix3fv(NormalMatrixHandle, 1, false, NormalMatrix);

  chooseMatrix(GLSL_BASE::MVMATRIX);

  multiplyMVMatrix();

  sendMatrix();

  chooseMatrix(GLSL_BASE::MVPMATRIX);
  //GLSL_BASE->setIdentity();
  multiplyMPMatrix();

  sendMatrix();

  free(tempMatrix);
  free(tempMatrix2);

}

