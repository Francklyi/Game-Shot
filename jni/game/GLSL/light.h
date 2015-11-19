/***********************************************************************
 * Module:  Light.h
 * Author:  OneTime
 * Modified: 2013Äê7ÔÂ31ÈÕ 16:31:02
 * Purpose: Declaration of the class Light
 ***********************************************************************/

#if !defined(__GLSL_Light_h)
#define __GLSL_Light_h
#include <GLES2/gl2.h>
class Light
{
public:
   Light(){ Light(0); }
   Light(float* pos_, float* La_=0, float* Ld_=0, float* Ls_=0);
   void setPos(float* pos_);
   void setAmbient(float* La_);
   void setDiffuse(float* Ld_);
   void setSpecular(float* Ls_);

   void setPosHandle(GLuint posHandle_);
   GLuint getPosHandle();
   void setLaHandle(GLuint LaHandle_);
   GLuint getLaHandle();
   void setLdHandle(GLuint LdHandle_);
   GLuint getLdHandle();
   void setLsHandle(GLuint LsHandle_);
   GLuint getLsHandle();

   void link(int programHandle);
   void draw();

   float* pos;
   float* La;
   float* Ld;
   float* Ls;
   GLuint posHandle;
   GLuint LaHandle;
   GLuint LdHandle;
   GLuint LsHandle;

   float* VMatrix;

};

#endif
