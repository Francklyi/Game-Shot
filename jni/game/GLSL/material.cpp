
/***********************************************************************
 * Module:  Material.cpp
 * Author:  OneTime
 * Modified: 2013��7��31�� 16:31:02
 * Purpose: Implementation of the class Material
 ***********************************************************************/
#include "material.h"
#include <stdlib.h>
#include <GLES2/gl2.h>
#include "log.h"

////////////////////////////////////////////////////////////////////////
// Name:       Material::Material(flaot* pos_, flaot* La_, float* Ld_, float* Ls_)
// Purpose:    Implementation of Material::Material()
// Parameters:
// - pos_
// - La_
// - Ld_
// - Ls_
// Return:
////////////////////////////////////////////////////////////////////////

Material::Material(float* Ka_, float* Kd_, float* Ks_, float Shininess_)
{
  if(Ka_!=0)
    {
      Ka = Ka_;
    }else
    {
        Ka=(float*)malloc(3*sizeof(float));
        Ka[0]=1.0f;
        Ka[1]=1.0f;
        Ka[2]=1.0f;
    }

  if(Kd_!=0)
    {
      Kd = Kd_;
    }else
    {
        Kd=(float*)malloc(3*sizeof(float));
        Kd[0]=1.0f;
        Kd[1]=1.0f;
        Kd[2]=1.0f;
    }

  if(Ks_!=0)
    {
      Ks = Ks_;
    }else
    {
        Ks=(float*)malloc(3*sizeof(float));
        Ks[0]=1.0f;
        Ks[1]=1.0f;
        Ks[2]=1.0f;
    }

  Shininess=Shininess_;
}

////////////////////////////////////////////////////////////////////////
// Name:       Material::setAmbient(float* La_)
// Purpose:    Implementation of Material::setAmbient()
// Parameters:
// - La_
// Return:     void
////////////////////////////////////////////////////////////////////////

void Material::setAmbient(float* Ka_)
{
   // TODO : implement
  Ka = Ka_;
}

////////////////////////////////////////////////////////////////////////
// Name:       Material::setDiffuse(float* Ld_)
// Purpose:    Implementation of Material::setDiffuse()
// Parameters:
// - Ld_
// Return:     void
////////////////////////////////////////////////////////////////////////

void Material::setDiffuse(float* Kd_)
{
   // TODO : implement
  Kd = Kd_;
}

////////////////////////////////////////////////////////////////////////
// Name:       Material::setSpecular(float* Ls_)
// Purpose:    Implementation of Material::setSpecular()
// Parameters:
// - Ls_
// Return:     void
////////////////////////////////////////////////////////////////////////

void Material::setSpecular(float* Ks_)
{
   // TODO : implement
  Ks = Ks_;
}

void Material::setShininess(float Shininess_)
{
  Shininess=Shininess_;
}

void Material::setKaHandle(GLuint KaHandle_)
{
  KaHandle=KaHandle_;
}

GLuint Material::getKaHandle()
{
  return KaHandle;
}

void Material::setKdHandle(GLuint KdHandle_)
{
  KdHandle= KdHandle_;
}

GLuint Material::getKdHandle()
{
  return KdHandle;
}

void Material::setKsHandle(GLuint KsHandle_)
{
  KsHandle=KsHandle_;
}

GLuint Material::getKsHandle()
{
  return KsHandle;
}

void Material::setShininessHandle(GLuint ShininessHandle_)
{
  ShininessHandle=ShininessHandle_;
}

GLuint Material::getShininessHandle()
{
  return ShininessHandle;
}

////////////////////////////////////////////////////////////////////////
// Name:       Material::link(int programHandle)
// Purpose:    Implementation of Material::link()
// Parameters:
// - programHandle
// Return:     void
////////////////////////////////////////////////////////////////////////

void Material::link(int programHandle)
{
  if (!programHandle) {
      LOGE("programHandle is null.");
      return ;
  }
   // TODO : implement
  KaHandle    = glGetUniformLocation(programHandle, "Ka" );
  KdHandle    = glGetUniformLocation(programHandle, "Kd" );
  KsHandle    = glGetUniformLocation(programHandle, "Ks" );
  ShininessHandle=glGetUniformLocation(programHandle, "Shininess" );

  LOGI("glGetUniformLocation(\"KaHandle\") = %d\n",
      KaHandle);
  LOGI("glGetUniformLocation(\"KdHandle\") = %d\n",
      KdHandle);
  LOGI("glGetUniformLocation(\"KsHandle\") = %d\n",
      KsHandle);
  LOGI("glGetUniformLocation(\"ShininessHandle\") = %d\n",
      ShininessHandle);
}

void Material::drawNoMaterial()
{
	  glUniform3f( KaHandle,  0.0f,  0.0f,  0.0f  );
	  glUniform3f( KdHandle,  0.0f,  0.0f,  0.0f  );
	  glUniform3f( KsHandle,  1.0f,  1.0f,  1.0f  );
	  glUniform1f( ShininessHandle,  8.0f );
}

////////////////////////////////////////////////////////////////////////
// Name:       Material::draw()
// Purpose:    Implementation of Material::draw()
// Return:     void
////////////////////////////////////////////////////////////////////////

void Material::draw(void)
{
  glUniform3f( KaHandle,  Ka[0],  Ka[1],  Ka[2]  );
  glUniform3f( KdHandle,  Kd[0],  Kd[1],  Kd[2]  );
  glUniform3f( KsHandle,  Ks[0],  Ks[1],  Ks[2]  );
  glUniform1f( ShininessHandle,  Shininess );
  //LOGI("Shininess: %f",Shininess);
   // TODO : implement
}
