#ifndef _MATERIAL_H_
#define _MATERIAL_H_
#include <GLES2/gl2.h>
class Material
{
public:
   Material(){ Material(0,0,0,10.0f); }
   Material(float* Ka_, float* Kd_=0, float* Ks_=0,float Shininess=1);

   void setAmbient(float* Ka_);
   void setDiffuse(float* Kd_);
   void setSpecular(float* Ks_);
   void setShininess(float Shininess_);

   void setKaHandle(GLuint KaHandle_);
   GLuint getKaHandle();
   void setKdHandle(GLuint KdHandle_);
   GLuint getKdHandle();
   void setKsHandle(GLuint KsHandle_);
   GLuint getKsHandle();
   void setShininessHandle(GLuint ShininessHandle_);
   GLuint getShininessHandle();

   void link(int programHandle);
   void draw();

   void drawNoMaterial();

protected:
private:
   float* Ka;
   float* Kd;
   float* Ks;
   float Shininess;

   GLuint KaHandle;
   GLuint KdHandle;
   GLuint KsHandle;
   GLuint ShininessHandle;
};

#endif
