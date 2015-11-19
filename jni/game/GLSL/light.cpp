/***********************************************************************
 * Module:  Light.cpp
 * Author:  OneTime
 * Modified: 2013Äê7ÔÂ31ÈÕ 16:31:02
 * Purpose: Implementation of the class Light
 ***********************************************************************/

#include "light.h"
#include <stdlib.h>
#include <GLES2/gl2.h>
#include "log.h"

////////////////////////////////////////////////////////////////////////
// Name:       Light::Light(flaot* pos_, flaot* La_, float* Ld_, float* Ls_)
// Purpose:    Implementation of Light::Light()
// Parameters:
// - pos_
// - La_
// - Ld_
// - Ls_
// Return:
////////////////////////////////////////////////////////////////////////

Light::Light(float* pos_, float* La_, float* Ld_, float* Ls_)
{
  if(pos_!=0)
    {
      pos = pos_;
    }else
    {
      pos=(float*)malloc(4*sizeof(float));
      pos[0]=0;
      pos[1]=0;
      pos[2]=0;
      pos[3]=0;
    }

  if(La_!=0)
    {
      La = La_;
    }else
    {
        La=(float*)malloc(3*sizeof(float));
        La[0]=0.6f;
        La[1]=0.6f;
        La[2]=0.6f;
    }

  if(Ld_!=0)
    {
      Ld = Ld_;
    }else
    {
        Ld=(float*)malloc(3*sizeof(float));
        Ld[0]=0.6f;
        Ld[1]=0.6f;
        Ld[2]=0.6f;
    }

  if(Ls_!=0)
    {
      Ls = Ls_;
    }else
    {
        Ls=(float*)malloc(3*sizeof(float));
        Ls[0]=0.6f;
        Ls[1]=0.6f;
        Ls[2]=0.6f;
    }

  VMatrix=(float *)malloc(16*sizeof(float));

}

////////////////////////////////////////////////////////////////////////
// Name:       Light::setPos(float* pos_)
// Purpose:    Implementation of Light::setPos()
// Parameters:
// - pos_
// Return:     void
////////////////////////////////////////////////////////////////////////

void Light::setPos(float* pos_)
{
   // TODO : implement
  pos = pos_;
}

////////////////////////////////////////////////////////////////////////
// Name:       Light::setAmbient(float* La_)
// Purpose:    Implementation of Light::setAmbient()
// Parameters:
// - La_
// Return:     void
////////////////////////////////////////////////////////////////////////

void Light::setAmbient(float* La_)
{
   // TODO : implement
  La = La_;
}

////////////////////////////////////////////////////////////////////////
// Name:       Light::setDiffuse(float* Ld_)
// Purpose:    Implementation of Light::setDiffuse()
// Parameters:
// - Ld_
// Return:     void
////////////////////////////////////////////////////////////////////////

void Light::setDiffuse(float* Ld_)
{
   // TODO : implement
  Ld = Ld_;
}

////////////////////////////////////////////////////////////////////////
// Name:       Light::setSpecular(float* Ls_)
// Purpose:    Implementation of Light::setSpecular()
// Parameters:
// - Ls_
// Return:     void
////////////////////////////////////////////////////////////////////////

void Light::setSpecular(float* Ls_)
{
   // TODO : implement
  Ls = Ls_;
}

void Light::setPosHandle(GLuint posHandle_)
{
  posHandle=posHandle_;
}

GLuint Light::getPosHandle()
{
  return posHandle;
}

void Light::setLaHandle(GLuint LaHandle_)
{
  LaHandle=LaHandle_;
}

GLuint Light::getLaHandle()
{
  return LaHandle;
}

void Light::setLdHandle(GLuint LdHandle_)
{
  LdHandle= LdHandle_;
}

GLuint Light::getLdHandle()
{
  return LdHandle;
}

void Light::setLsHandle(GLuint LsHandle_)
{
  LsHandle=LsHandle_;
}

GLuint Light::getLsHandle()
{
  return LsHandle;
}

////////////////////////////////////////////////////////////////////////
// Name:       Light::link(int programHandle)
// Purpose:    Implementation of Light::link()
// Parameters:
// - programHandle
// Return:     void
////////////////////////////////////////////////////////////////////////

void Light::link(int programHandle)
{
  if (!programHandle) {
      LOGE("programHandle is null.");
      return ;
  }
   // TODO : implement
  posHandle    = glGetUniformLocation(programHandle, "LPosition" );
  LaHandle    = glGetUniformLocation(programHandle, "La" );
  LdHandle    = glGetUniformLocation(programHandle, "Ld" );
  LsHandle    = glGetUniformLocation(programHandle, "Ls" );

  LOGI("glGetUniformLocation(\"LPosition\") = %d\n",
      posHandle);
  LOGI("glGetUniformLocation(\"LaHandle\") = %d\n",
      LaHandle);
  LOGI("glGetUniformLocation(\"LdHandle\") = %d\n",
      LdHandle);
  LOGI("glGetUniformLocation(\"LsHandle\") = %d\n",
      LsHandle);
}

////////////////////////////////////////////////////////////////////////
// Name:       Light::draw()
// Purpose:    Implementation of Light::draw()
// Return:     void
////////////////////////////////////////////////////////////////////////

void Light::draw(void)
{
  //glUniform4fv( posHandle, 1,pos);
  //LOGI("light_position:%f , %f, %f , %f",pos[0], pos[1], pos[2], pos[3]);
  glUniform4f( posHandle , pos[0]+0.01f, pos[1]+0.01f, pos[2]+0.01f, pos[3]+0.01f);
  glUniform3f( LaHandle,  La[0],  La[1],  La[2]  );
  glUniform3f( LdHandle,  Ld[0],  Ld[1],  Ld[2]  );
  glUniform3f( LsHandle,  Ls[0],  Ls[1],  Ls[2]  );
   // TODO : implement
}
