/***********************************************************************
 * Module:  GLSL.cpp
 * Author:  OneTime
 * Modified: 2013��7��31�� 17:36:46
 * Purpose: Implementation of the class GLSL
 ***********************************************************************/

#include "GLSL.h"
#include <stdlib.h>
#include "log.h"


#define STRINGIFY(A) #A

const char* GLSL::FragmentShader_base=\

//#include "shader/noise.frag"
#include "shader/Shader.frag"
;

const char* GLSL::VertexShader_base=\

#include "shader/Shader.vert"


#include "shader/Shader2.frag"
#include "shader/Shader2.vert"


const char* GLSL::FragmentShader_se=\

#include "shader/Shader4.frag"


#include "shader/Shader_simple.vert"

const char* GLSL::FragmentShader_simple=\

#include "shader/simple.frag"

#include "shader/hdr_easy.frag"

const char* GLSL::FragmentShader_log=\

#include "shader/log.frag"

const char* GLSL::FragmentShader_hightLight=\

#include "shader/getHighLight.frag"


const char* GLSL::FragmentShader_gaosi_x=\

#include "shader/gaosi_x.frag"

const char* GLSL::FragmentShader_gaosi_y=\

#include "shader/gaosi_y.frag"

const char* GLSL::FragmentShader_bloom=\

#include "shader/bloom.frag"

////////////////////////////////////////////////////////////////////////
// Name:       GLSL::GLSL()
// Purpose:    Implementation of GLSL::GLSL()
// Return:
////////////////////////////////////////////////////////////////////////

GLuint GLSL::uboLast=0;

GLSL::GLSL():
		whichMatrix(0)
,MMatrix((float*)malloc(16*sizeof(float))),
VMatrix((float*)malloc(16*sizeof(float))),
MVMatrix((float*)malloc(16*sizeof(float))),
PMatrix((float*)malloc(16*sizeof(float))),
PVMatrix((float*)malloc(16*sizeof(float))),
MVPMatrix((float*)malloc(16*sizeof(float))),
NormalMatrix((float*)malloc(16*sizeof(float)))
,MVPMatrix_S((float*)malloc(16*sizeof(float)))
,VMatrix_S((float*)malloc(16*sizeof(float)))
,vertexShader0(0)
{
	mMatrixQueue=vector<float *>(0);
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

	light = new Light(0);
	material = new Material(0);
	printGLString("Version", GL_VERSION);
	printGLString("Vendor", GL_VENDOR);
	printGLString("Renderer", GL_RENDERER);
	printGLString("Extensions", GL_EXTENSIONS);
	funcType=FUNS_LIGHT_EASY;

	pGLSL_BASE=new GLSL_BASE((char*)VertexShader_base,(char*)FragmentShader_base);
	init(pGLSL_BASE);

	pGLSL_log=new GLSL_BASE((char*)VertexShader_base,(char*)FragmentShader_log);
	init(pGLSL_log);

	pGLSL_getHLight=new GLSL_getHLight((char*)VertexShader_base,//VertexShader_simple
			(char*)FragmentShader_hightLight);
	init(pGLSL_getHLight);

	pGLSL_gaosi_x=new GLSL_GAOSI_X((char*)VertexShader_base,//VertexShader_simple
			(char*)FragmentShader_gaosi_x);
	init(pGLSL_gaosi_x);

	pGLSL_gaosi_y=new GLSL_GAOSI_Y((char*)VertexShader_base,//VertexShader_simple
			(char*)FragmentShader_gaosi_y);
	init(pGLSL_gaosi_y);

	pGLSL_BLOOM=new GLSL_BLOOM((char*)VertexShader_base,//VertexShader_simple
			(char*)FragmentShader_bloom);
	init(pGLSL_BLOOM);

	//pGLSL_SE=new GLSL_SE((char*)VertexShader_base,(char*)FragmentShader_se);
	//init(pGLSL_SE);

	//init_S();
}

void GLSL::setMode(int mode_)
{
  mode = mode_;
}

bool GLSL::isNormalMode()
{
  return (mode==NORMAL);
}

bool GLSL::isShadowMode()
{
  return (mode==SHADOW);
}

void GLSL::setFuncType(int funcType_)
{
  funcType=funcType_;
}

void GLSL::printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

void GLSL::init(GLSLProj *pGP)
{
	program = createProgram(pGP->VertexShader, pGP->FragmentShader);
  if (!program) {
      LOGE("Could not create program.");
      return ;
  }

  pGP->gvPositionHandle   = glGetAttribLocation(program, "VertexPosition");
  pGP->gvNormalHandle     = glGetAttribLocation(program, "VertexNormal");
  pGP->gvTexCoordHandle   = glGetAttribLocation(program, "VertexTexCoord");
  pGP->MVMatrixHandle     = glGetUniformLocation(program, "ModelViewMatrix" );
  pGP->NormalMatrixHandle = glGetUniformLocation(program, "NormalMatrix" );
 // PMatrixHandle      = glGetUniformLocation(program, "ProjectionMatrix" );
  pGP->MVPMatrixHandle    = glGetUniformLocation(program, "MVP" );

  pGP->MVPLightMatrixHandle= glGetUniformLocation(program, "MVPLight" );

  pGP->hasTextureHandle   = glGetUniformLocation(program, "hasTexture" );

  pGP->timeHandle = glGetUniformLocation(program, "time_v" );

  pGP->funcTypeHandle = glGetUniformLocation(program, "funcType" );

  pGP->tex1Handle = glGetUniformLocation(program, "Tex1");
/*
  LOGI("glGetAttribLocation(\"VertexPosition\") = %d\n",
		  pGP->gvPositionHandle);
  LOGI("glGetAttribLocation(\"VertexNormal\") = %d\n",
		  pGP->gvNormalHandle);
  LOGI("glGetAttribLocation(\"VertexTexCoord\") = %d\n",
		  pGP->gvTexCoordHandle);
  LOGI("glGetUniformLocation(\"ModelViewMatrix\") = %d\n",
		  pGP->MVMatrixHandle);
  LOGI("glGetUniformLocation(\"MVP\") = %d\n",
		  pGP->MVPMatrixHandle);

  LOGI("glGetUniformLocation(\"NormalMatrix\") = %d\n",
		  pGP->NormalMatrixHandle);

  LOGI("glGetUniformLocation(\"MVPLight\") = %d\n",
		  pGP->MVPLightMatrixHandle);

  LOGI("glGetUniformLocation(\"hasTexture\") = %d\n",
		  pGP->hasTextureHandle);

  LOGI("glGetUniformLocation(\"time_v\") = %d\n",
		  pGP->timeHandle);
*/
  pGP->program=program;
  pGP->initSelf();


  pGP->light->link(pGP->program);
  pGP->material->link(pGP->program);
}


void GLSL::init_S()
{
  program_S = createProgram(GLSL::VertexShader_S, GLSL::FragmentShader_S);
  if (!program_S) {
      LOGE("Could not create program.");
      return ;
  }

  gvPositionHandle_S   = glGetAttribLocation(program_S, "VertexPosition");
  //MVMatrixHandle     = glGetUniformLocation(program, "ModelViewMatrix" );
 // PMatrixHandle      = glGetUniformLocation(program, "ProjectionMatrix" );
  MVPMatrixHandle_S    = glGetUniformLocation(program_S, "MVP" );

  LOGI("glGetAttribLocation(\"VertexPosition _S\") = %d\n",
          gvPositionHandle_S);

  LOGI("glGetUniformLocation(\"MVP _S\") = %d\n",
              MVPMatrixHandle_S);
}

////////////////////////////////////////////////////////////////////////
// Name:       GLSL::loadShader()
// Purpose:    Implementation of GLSL::loadShader()
// Return:     GLuint
////////////////////////////////////////////////////////////////////////

GLuint GLSL::loadShader(GLenum shaderType, const char* pSource)
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
// Name:       GLSL::createProgram()
// Purpose:    Implementation of GLSL::createProgram()
// Return:     GLuint
////////////////////////////////////////////////////////////////////////

GLuint GLSL::createProgram(const char* pVertexSource, const char* pFragmentSource)
{
	// TODO : implement
	if(vertexShader0==0)
		vertexShader0 = loadShader(GL_VERTEX_SHADER, pVertexSource);
	if (!vertexShader0) {
		LOGE("Could not compile vertexShader");
		return 0;
	}

	GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
	if (!pixelShader) {
		LOGE("Could not compile pixelShader");
		return 0;
	}

	GLuint program = glCreateProgram();
	if (program) {
		glAttachShader(program, vertexShader0);
		// checkGlError("glAttachShader");
		//glAttachShader(program, pixelShader_noise);

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

////////////////////////////////////////////////////////////////////////
// Name:       GLSL::getProgramHandle()
// Purpose:    Implementation of GLSL::getProgramHandle()
// Return:     int
////////////////////////////////////////////////////////////////////////

GLuint GLSL::getProgramHandle(void)
{
   // TODO : implement
  if(mode==NORMAL)
      return program;
  else
    if(mode==SHADOW)
      return program_S;

  return 0;
}

void GLSL::setLookAt( float eyeX, float eyeY, float eyeZ,
                float centerX, float centerY, float centerZ,
                float upX, float upY,float upZ)
{
  getMatrix().setLookAtM(VMatrix, 0, eyeX, eyeY, eyeZ, centerX, centerY, centerZ, upX, upY, upZ);
  getMatrix().multiplyMM(PVMatrix, 0, PMatrix, 0, VMatrix, 0 );

}

void GLSL::ortho( float left, float right, float bottom, float top,
            float near, float far)
{
  getMatrix().orthoM(PMatrix, 0, left, right, bottom, top, near, far);
}

void GLSL::frustum( float left, float right, float bottom, float top,
              float near, float far)
{
  getMatrix().frustumM(PMatrix, 0, left, right, bottom, top, near, far);
}

void GLSL::chooseMatrix(int m)
{
  whichMatrix=m;
}

void GLSL::setIdentity(void)
{
  if(whichMatrix==VMATRIX) //VMatrix
    {
      getMatrix().setIdentityM(VMatrix,0);
    }else if(whichMatrix==MMATRIX) //MMatrix
      {
        getMatrix().setIdentityM(MMatrix,0);
        for(int i=0;i<mMatrixQueue.size();i++)
          free(mMatrixQueue[i]);
        mMatrixQueue.clear();

      }else if(whichMatrix==MVMATRIX) //MVMatrix
      {
          getMatrix().setIdentityM(MVMatrix,0);
      }else if(whichMatrix==PMATRIX) //MVPMatrix
        {
          getMatrix().setIdentityM(PMatrix,0);
        }else if(whichMatrix==MVPMATRIX) //MVPMatrix
          {
            getMatrix().setIdentityM(MVPMatrix,0);
          }
}

void GLSL::perspective(float fovy, float aspect, float zNear, float zFar)
{
  getMatrix().perspectiveM(PMatrix, 0, fovy, aspect, zNear, zFar);
}

void GLSL::scale(float x, float y, float z)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        getMatrix().scaleM(VMatrix,0, x, y, z);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          getMatrix().scaleM(MMatrix,0, x, y, z);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
        {
            getMatrix().scaleM(MVMatrix,0, x, y, z);
        }
}

void GLSL::translate(float x, float y, float z)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        getMatrix().translateM(VMatrix,0, x, y, z);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          getMatrix().translateM(MMatrix,0, x, y, z);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
        {
            getMatrix().translateM(MVMatrix,0, x, y, z);
        }
}

void GLSL::rotate(float a, float x, float y, float z)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        getMatrix().rotateM(VMatrix,0, a,x, y, z);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          getMatrix().rotateM(MMatrix,0, a, x, y, z);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
          {
            getMatrix().rotateM(MVMatrix,0, a, x, y, z);
          }
}

void GLSL::pushMatrix()
{
  float *matrix_now=(float*)malloc(sizeof(float)*16);

  getMatrix().arrayCopy(MMatrix, 0, matrix_now, 0, 16);
  mMatrixQueue.push_back(matrix_now);
}

void GLSL::popMatrix()
{
  if(mMatrixQueue.size())
    {
      delete MMatrix;
      MMatrix=mMatrixQueue[mMatrixQueue.size()-1];
      mMatrixQueue.pop_back();
    }
}

void GLSL::multMatrix(float *matrix)
{
  if(whichMatrix==VMATRIX) //VMatrix
      {
        getMatrix().multiplyMM(VMatrix, 0, VMatrix, 0, matrix, 0);
      }else if(whichMatrix==MMATRIX) //MMatrix
        {
          getMatrix().multiplyMM(MMatrix, 0, MMatrix, 0, matrix, 0);
        }else if(whichMatrix==MVMATRIX) //MVMatrix
          {
            getMatrix().multiplyMM(MVMatrix, 0, MVMatrix, 0, matrix, 0);
          }
}

void GLSL::multiplyMVMatrix()
{
  getMatrix().setIdentityM(MVMatrix,0);
  getMatrix().multiplyMM(MVMatrix, 0, VMatrix, 0, MMatrix, 0);
}

void GLSL::multiplyMPMatrix()
{
  getMatrix().setIdentityM(MVPMatrix,0);
  getMatrix().multiplyMM(MVPMatrix, 0, PMatrix, 0, MVMatrix, 0);
}

void GLSL::sendMatrix()
{
    if(whichMatrix==GLSL::MVMATRIX) //MVMatrix
      {

          glUniformMatrix4fv( pGPNow->MVMatrixHandle, 1, false, MVMatrix);
      }else if(whichMatrix==GLSL::MVPMATRIX) //MVPMatrix
          {
            glUniformMatrix4fv( pGPNow->MVPMatrixHandle, 1, false, MVPMatrix);
          }
  //
}

void GLSL::attribPointer(int handle, GLuint ubo,int size,
    GLsizei stride, const GLvoid* pointer)
{
	//if(uboLast!=ubo)
		glBindBuffer(GL_ARRAY_BUFFER, ubo);
    glVertexAttribPointer(handle, size, GL_FLOAT, GL_FALSE, stride, pointer);
    glEnableVertexAttribArray(handle);
}

void GLSL::useProgram()
{
  if(mode==NORMAL)
    glUseProgram(program);
  else
    if(mode==SHADOW)
      glUseProgram(program_S);
}

unsigned GLSL::getATextureID()
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

void GLSL::setHasTex(float has)
{
  glUniform1f( pGPNow->hasTextureHandle , has );
}

void GLSL::setLight(){

	float light_position[]={ 200.0, 100.0, 200.0, 1.0};
	getMatrix().multiplyMV(light_position,0,VMatrix,0,light_position,0);
	// getMatrix().arrayCopy(VMatrix,0, light->VMatrix,0, 16);
	//if(light->pos[0]!=light_position[0])
	//LOGI("light_position:%f, %f, %f",light_position[0],light_position[1],light_position[2]);

	float light_ambient[]={1.0f,1.0f,1.0f};
	float light_diffuse[]={1.0f,1.0f,1.0f};
	float light_specular[]={1.0f,1.0f,1.0f};

	pGPNow->light->setPos(light_position);
	pGPNow->light->setAmbient(light_ambient);
	pGPNow->light->setDiffuse(light_diffuse);
	pGPNow->light->setSpecular(light_specular);
	pGPNow->light->draw();
}

void GLSL::setPGPNow(GLSLProj *pGPNow_)
{
	pGPNow=pGPNow_;
	light=pGPNow_->light;
	material=pGPNow_->material;
	glUseProgram(pGPNow->program);
	program=pGPNow->program;
}

void GLSL::startFrame()
{
	setLight();
}

void GLSL::glslUpdate()
{

	glUniform1f( pGPNow->funcTypeHandle,  (float)funcType );

	setLight();

	chooseMatrix(GLSL::MVMATRIX);

	multiplyMVMatrix();

	//LOGI("mvp : %f, %f ,%f, %f",MVPMatrix[0],MVPMatrix[1],MVPMatrix[2],MVPMatrix[3]);

	sendMatrix();

	chooseMatrix(GLSL::MVPMATRIX);
	//glsl->setIdentity();
	multiplyMPMatrix();

	sendMatrix();

	for(int i=0;i<3;i++)
	{
		pf16[i*3]=MVMatrix[i*4];
		pf16[i*3+1]=MVMatrix[i*4+1];
		pf16[i*3+2]=MVMatrix[i*4+2];
	}

	getMatrix().invertM3(pf16);

	//getMatrix().setIdentityM(NormalMatrix,0);

	getMatrix().transposeM3(pf16);

	glUniformMatrix3fv(pGPNow->NormalMatrixHandle, 1, false, pf16);

}

GLSL& getGL()
{
	static GLSL glsl;
	return glsl;
}

//GLSL *glsl=0;
