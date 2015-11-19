/***********************************************************************
 * Module:  GLSL.h
 * Author:  OneTime
 * Modified: 2013��7��31�� 17:36:46
 * Purpose: Declaration of the class GLSL
 ***********************************************************************/

#if !defined(__GLSL_GLSL_h)
#define __GLSL_GLSL_h

#include "light.h"
#include "material.h"
#include <GLES2/gl2.h>
#include "matrix.h"
#include <vector>
#include "GLSLProj.h"
#include "GLSL_BASE.h"
#include "GLSL_SE.h"

#include "GLSL_GAOSI_X.h"
#include "GLSL_GAOSI_Y.h"
#include "GLSL_BLOOM.h"
#include "GLSL_getHLight.h"

using std::vector;
enum Modes { NORMAL=1, SHADOW};

class GLSL
{
public:
   GLSL();

   void setMode(int mode);

   bool isNormalMode();

   bool isShadowMode();

   void init(GLSLProj *pGP);// for create and link

   void init_S(); //for shadow

   GLuint loadShader(GLenum shaderType, const char* pSource);

   GLuint createProgram(const char* pVertexSource, const char* pFragmentSource);

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

   void attribPointer(int handle, GLuint ubo,int size,
       GLsizei stride, const GLvoid* pointer);

   void useProgram();

   void popMatrix();

   void pushMatrix();

   void glslUpdate();

   unsigned getATextureID();

   void setLight();

   void setHasTex(float has);

   void setFuncType(int funcType_);

   void setPGPNow(GLSLProj *pGPNow_);

   void startFrame();

   GLSLProj *pGPNow;

   GLSL_BASE *pGLSL_BASE;

   GLSL_BASE *pGLSL_simple;

   GLSL_SE *pGLSL_SE;

   GLSL_getHLight *pGLSL_getHLight;

   GLSL_GAOSI_X *pGLSL_gaosi_x;

   GLSL_GAOSI_Y *pGLSL_gaosi_y;

   GLSL_BLOOM *pGLSL_BLOOM;

   GLSL_BASE *pGLSL_log;

   int mode;

   unsigned TextureID[32];

   int funcType;

   enum{FUNS_LIGHT_EASY=0, FUNS_LIGHT_PHONG, FUNS_NORMAL,FUNS_N_TEXTRUE};

   Light *light;

   Material *material;

   const static int VMATRIX = 1;
   const static int MMATRIX = 2;
   const static int MVMATRIX = 3;
   const static int PMATRIX = 4;
   const static int MVPMATRIX = 5;
//protected:
//private:
   int whichMatrix; // 1->ViewMatrix , 2->ModelMatrix , 3->ModelViewMatrix , 4->projection Matrix
   float* VMatrix;
   float* MMatrix;
   float* MVMatrix;
   float* NormalMatrix;
   float* PMatrix;
   float* PVMatrix;
   float* MVPMatrix;

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

   GLuint funcSpecHandle;

   GLuint timeHandle;

   static const char* VertexShader_base;
   static const char* FragmentShader_base;

   static const char* VertexShader_se;
   static const char* FragmentShader_se;

   static const char* VertexShader_simple; //for simple squre
   static const char* FragmentShader_simple;

   static const char* FragmentShader_log;

   static const char* FragmentShader_hightLight; //

   static const char* FragmentShader_gaosi_x; //
   static const char* FragmentShader_gaosi_y; //

   static const char* FragmentShader_bloom; //

   static const char* FragmentShader_hdr; //for shadow

   GLuint vertexShader0;

   //*****shadow *********
   float* VMatrix_S;

   float* MVPMatrix_S;

   GLuint shadowTexId;

   GLuint active_shadowTexId;

   GLuint program_S;

   GLuint MVPMatrixHandle_S;

   GLuint gvPositionHandle_S;

   static const char* VertexShader_S; //for shadow
   static const char* FragmentShader_S; //for shadow

   vector<float *> mMatrixQueue; //

   int viewport[4];

   float pf16[16];

   static GLuint uboLast;
};

GLSL& getGL();

//extern GLSL *glsl;
#endif
