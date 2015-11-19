#include "GData.h"
#include "fourNumber.h"
//#include "getMtl.h"
#include "log.h"
#include "fileDeal/loadFile.h"
//#include "GLES/gl.h"


#define BUFFER_OFFSET(offset) ((GLvoid *) NULL + offset)

#define numofNewMtl numofMtls
#define Newmtl Mtls
#define Animation Movement
#define EntityData GraphData
//#define gp gp
#define Movement Movement1
GLSL *Graph::glsl=0;
//vector<float*> Entity::aniEntityp;

int Graph::TextureId=0;
bool Graph::useTexture=true;

void Graph::getOneMtl(char **cp,Mtls *Mp){
        Mp->Lmap_Ka=(*cp)++;
        (*cp)+=*Mp->Lmap_Ka;
        Mp->Lmap_Kd=(*cp)++;
        (*cp)+=*Mp->Lmap_Kd;
        Mp->Ns=(float*)(*cp);
        (*cp)+=4;
        Mp->Ni=(float*)(*cp);
        (*cp)+=4;
        Mp->d=(float*)(*cp);
        (*cp)+=4;
        Mp->Tr=(float*)(*cp);
        (*cp)+=4;
        Mp->illum=(int*)(*cp);
        (*cp)+=4;
        Mp->Tf=(float*)(*cp);
        (*cp)+=16;
        Mp->Ka=(float*)(*cp);
        (*cp)+=16;
        Mp->Kd=(float*)(*cp);
        (*cp)+=16;
        Mp->Ks=(float*)(*cp);
        (*cp)+=16;
        Mp->Ke=(float*)(*cp);
        (*cp)+=16;
        Mp->mTextureId=0;
}

GLuint Graph::setTexture(char **cp){
        GLuint texName;
        int w=getInt(cp);
        int h=getInt(cp);
        int imgLength=getInt(cp);
        glGenTextures(1, &texName);
        glBindTexture(GL_TEXTURE_2D, texName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
                    GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
                    GL_LINEAR);
       // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w,
                 h, 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 *cp);

        //glGenerateMipmap (GL_TEXTURE_2D);

        //LOGI("Postion %d",188);
        //(*cp)+=imgLength;
        return texName;
}

GLuint Graph::setTextureB(char *map_Kd){
        GLuint texName11;
      //  LOGI("setTextureB:%d", 155);
        char folder[]="./sdcard/textureFiles/";
        char *filgpath=combineChars(folder,map_Kd);
    char * chars=getCharsofFile(filgpath);
    texName11=setTexture(&chars);
    return texName11;
}

void Graph::getMtls(char **cp, GraphData *gp){
        gp->numofNewMtl=getNewmtlNum(cp);
      //  LOGI("gp->numofNewMtl:%d",gp->numofNewMtl);
        gp->np=(Newmtl *)malloc((gp->numofNewMtl)*sizeof(Newmtl)*4);
        int i=0 , j=gp->numofNewMtl;
        while(i<j)
        {
                getOneMtl(cp,&(gp->np[i]));
                i++;
        }
        i=0;
        //设置像素存储格式及开启纹理功能
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        //glEnable(GL_TEXTURE_2D);
        gp->pointerNum=*(int *)(*cp);
        (*cp)+=4;
        gp->vp=(float*)(*cp);
      //  LOGI("gp->vp %f\n", gp->vp[0]);
        (*cp)+=(4*3);
        gp->vnp=(float*)(*cp);
     //   LOGI("gp->vnp %f\n", gp->vnp[0]);
        (*cp)+=(4*3);
        gp->vtp=(float*)(*cp);
     //   LOGI("gp->vtp %f\n", gp->vtp[0]);
        (*cp)+=((gp->pointerNum)) * 8 * sizeof(float)-6*sizeof(float);
        gp->jointNump=*(int *)(*cp);
        (*cp)+=4;
        gp->weightsp=(float *)(*cp);
        (*cp)+=((gp->pointerNum))*((gp->jointNump))*4;

        glGenBuffers(1,gp->ubo);
     //   LOGI("ubo:%d",gp->ubo[0]);
        glBindBuffer(GL_ARRAY_BUFFER, gp->ubo[0]);
        glBufferData(GL_ARRAY_BUFFER, gp->pointerNum * 8 * sizeof(float)+ 6 * sizeof(float), gp->vp, GL_STATIC_DRAW);

       // glBindBuffer(GL_ARRAY_BUFFER, gp->ubo[1]);
       // glBufferSubData (GL_ARRAY_BUFFER, 3 * sizeof(float), gp->pointerNum*8*sizeof(float), const GLvoid* data);
       // glBufferData(GL_ARRAY_BUFFER, gp->pointerNum*8*sizeof(float), gp->vnp, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, 0); //移除缓存绑定

  //      LOGI("GD-numofNewMtl:%d",gp->numofNewMtl);
        i=0;
        //gp->ubois = (GLuint *)malloc(sizeof(GLuint)*gp->numofNewMtl);

        while(i<gp->numofNewMtl)
        {
                glGenBuffers(1,gp->np[i].uboi);
                gp->np[i].viL=*(int *)(*cp);

                (*cp)+=4;
                gp->np[i].vip=(unsigned short *)(*cp);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->np[i].uboi[0]);
                glBufferData(GL_ELEMENT_ARRAY_BUFFER, gp->np[i].viL *sizeof(unsigned short), gp->np[i].vip, GL_STATIC_DRAW);
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //移除缓存绑定

         //       LOGI("gp->np[i].vip %d\n", gp->np[i].vip[1]);
                (*cp)+=2*((gp->np[i].viL));
         //       LOGI("GD-Lmap_Kd:%d",(int)*(gp->np[i].Lmap_Kd));

                if(((int)*(gp->np[i].Lmap_Kd))!=0){

                     GLuint texName_=setTextureB((gp->np[i].Lmap_Kd)+1);

                     gp->np[i].mTextureId=texName_;
                     gp->np[i].active_texture_id=glsl->getATextureID();
                     //glActiveTexture( gp->np[i].active_texture_id + GL_TEXTURE0);

//                      LOGI("gp->np[i].mTextureId %d\n", gp->np[i].mTextureId);
                     glBindTexture(GL_TEXTURE_2D,gp->np[i].mTextureId);

                        }
                i++;
        }
      //  LOGI("Graph::getMtls end");
}

void Graph::loadModel(GraphData *gp,char * fileNamei)
{
 // LOGI("Graph1");
    //jiliye3_1.ly//"my_scene1_7.ly"
        char folder[]="./sdcard/lyfile/";
        char * filgpath=combineChars(folder,fileName);
        LOGI("Graph2:%s",filgpath);
        gp->fileData=getCharsofFile(filgpath);
        //gp->fileData=chars;
        if(*(gp->fileData)++!=76) return;

        getMtls(&(gp->fileData),gp);
}

void Graph::load(){
  gp=(GraphData *)malloc(sizeof(GraphData)*4);
        loadModel(gp , fileName);
        if((gp->jointNump)!=0){
                //char fileName1[]="cylinder1-2.amn\0";
            //animation 不再在该类中初始化 而是在role 相关类中初始化，并传递指针过来
                //loadAnimation(gp ,&(fileNamea[0]));
                gp->vp_=(float*)malloc((gp->pointerNum)*3*4);
        }
}

void Graph::setMaterial(Mtls *Mp)
{
  glsl->material->setAmbient(Mp->Ka);

  glsl->material->setDiffuse(Mp->Kd);
  glsl->material->setSpecular(Mp->Ks);
  //LOGI("ks: %f,%f,%f",Mp->Ks[0],Mp->Ks[1],Mp->Ks[2]);
  glsl->material->setShininess(*(Mp->illum));
  //LOGI("Ns: %f",Mp->Ns);
}

void Graph::drawMovement()
{
  if(gp->animations->change_stay==0){
          gp->animations->change_stay=1;
  }else{

  if(gp->animations->frame>(*(gp->animations->frameNum)-2)
                  ||gp->animations->frame<1)
  gp->animations->direction*=-1;
  (gp->animations->frame)+=gp->animations->direction;

  float *floats=(float*)malloc(3*4);
  FourNumber fourNumber=FourNumber(0,0,0);

  for(int k=0; k<(gp->pointerNum); k++){
          gp->vp_[k*3]=0;
          gp->vp_[k*3+1]=0;
          gp->vp_[k*3+2]=0;
          float w=0;
          for(int j=0;j<(gp->jointNump);j++){
                  w=(*(gp->weightsp+(((gp->jointNump))*k+j)));
                  if(*(gp->weightsp+(((gp->jointNump))*k+j))>0.0001) {
                      fourNumber=FourNumber(
                          (gp->vp+k*8)[0]-(gp->animations->joints[j].posf[0]),
                          (gp->vp+k*8)[1]-(gp->animations->joints[j].posf[1]),
                          (gp->vp+k*8)[2]-(gp->animations->joints[j].posf[2]));

                          fourNumber.productMatrix( floats,
                              (&(gp->animations->joints[j].Matrix[16*(gp->animations->frame)]) ));
                          gp->vp_[k*3]+=floats[0]*w;
                          gp->vp_[k*3+1]+=floats[1]*w;
                          gp->vp_[k*3+2]+=floats[2]*w;
                  }

          }
  }
free(floats);
gp->animations->change_stay=0;
  }

  glsl->attribPointer(glsl->gvPositionHandle, 0, 0, gp->vp_);
  glsl->attribPointer(glsl->gvNormalHandle, gp->ubo[0], 8*4, (GLvoid *)(12 ));
  glsl->attribPointer(glsl->gvTexCoordHandle, gp->ubo[0], 8*4, (GLvoid *)(24 ));

  int i=0;
  while(i<(gp->numofNewMtl)){
      if(gp->np[i].viL>0){
          setMaterial(&(gp->np[i]));
          glsl->material->draw();

          if (gp->np[i].mTextureId>0)
                  {
                    glBindTexture(GL_TEXTURE_2D,gp->np[i].mTextureId);
                    //GLuint loc = glGetUniformLocation(glsl->getProgramHandle(), "Tex1");
                    //if( loc >= 0 )
                    //glUniform1i(loc, gp->np[i].active_texture_id);
                  }
                  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->np[i].uboi[0]);
                  glDrawElements(GL_TRIANGLES,gp->np[i].viL,GL_UNSIGNED_SHORT,0);

      }
i++;
     }
}

void Graph::drawNoMovement(){
  if(glsl->isNormalMode())
    {
   glsl->attribPointer(glsl->gvPositionHandle, gp->ubo[0], 8*4, 0);
   glsl->attribPointer(glsl->gvNormalHandle, gp->ubo[0], 8*4, (GLvoid *)(12 ));
   glsl->attribPointer(glsl->gvTexCoordHandle, gp->ubo[0], 8*4, (GLvoid *)(24 ));
   int i=0;
   while(i<(gp->numofNewMtl)){
       if(gp->np[i].viL>0){
           setMaterial(&(gp->np[i]));
           glsl->material->draw();

            if (useTexture && gp->np[i].mTextureId>0)
              {
                glsl->setHasTex(1.0f);

                GLuint loc0 = glGetUniformLocation(glsl->getProgramHandle(), "Tex_shadow");
                //LOGE("loc, %d, %d",(int)loc,(int)glsl->active_shadowTexId);

                glActiveTexture( glsl->active_shadowTexId+ GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, glsl->shadowTexId);

                if( loc0 >= 0 )
                glUniform1i(loc0, glsl->active_shadowTexId);

                glActiveTexture( gp->np[i].active_texture_id + GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D,gp->np[i].mTextureId);
                //LOGE("gp->np[i].mTextureId %d\n", gp->np[i].mTextureId);
                int loc = glGetUniformLocation(glsl->getProgramHandle(), "Tex1");
                if( loc >= 0 )
                glUniform1i(loc, gp->np[i].active_texture_id);
              }
            else
              {
                glsl->setHasTex(0.0f);

                GLuint loc0 = glGetUniformLocation(glsl->getProgramHandle(), "Tex_shadow");
                //LOGE("loc, %d, %d",(int)loc,(int)glsl->active_shadowTexId);

                glActiveTexture( glsl->active_shadowTexId+ GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, glsl->shadowTexId);

                if( loc0 >= 0 )
                glUniform1i(loc0, glsl->active_shadowTexId);

              }
           // if(TextureId) glBindTexture(GL_TEXTURE_2D,TextureId);
             // LOGI("gp->np[i].vip %d\n", gp->np[i].vip[gp->np[i].viL-1]);
             // glDrawElements(GL_TRIANGLES,gp->np[i].viL,GL_UNSIGNED_SHORT,gp->np[i].vip);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->np[i].uboi[0]);
            glDrawElements(GL_TRIANGLES,gp->np[i].viL,GL_UNSIGNED_SHORT,0);

           }
       i++;
   }
 }
  else
        if(glsl->isShadowMode())
          {
         glsl->attribPointer(glsl->gvPositionHandle_S, gp->ubo[0], 8*4, 0);
         int i=0;
         while(i<(gp->numofNewMtl)){
             if(gp->np[i].viL>0)
               {

                  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->np[i].uboi[0]);
                  glDrawElements(GL_TRIANGLES,gp->np[i].viL,GL_UNSIGNED_SHORT,0);
               }
             i++;
         }
       }
}

void Graph::draw(){

  glsl->glslUpdate();
       // LOGI("(gp->jointNump)  %d\n",(gp->jointNump));
 if(gp->jointNump>0 && gp->animations){
     drawMovement();
 }
 else
   {
     drawNoMovement();
   }
 glBindTexture(GL_TEXTURE_2D,0);
}


Graph::~Graph(){
        //free(gp->np->VTNp);
        LOGI("~Graph0");
        if(gp->np) delete gp->np;
        gp->np=0;

        LOGI("~Graph1");
/*
        if(gp->vp) delete gp->vp;
        gp->vp=0;
        LOGI("~Graph2");
        if(gp->vnp) delete gp->vnp;
        gp->vnp=0;
        LOGI("~Graph3");
        if(gp->vtp) delete gp->vtp;
        gp->vtp=0;

        LOGI("~Graph5");
*/
        //free(gp->fileData);

        if(gp->vp_) delete gp->vp_;
        gp->vp_=0;
        LOGI("~Graph2");
        if(gp->fileData) delete gp->fileData;
        gp->fileData=0;
        LOGI("~Graph3");
        //free(gp->vp);
        //free(gp->vnp);
        //free(gp->vtp);
        //free(gp->vp_);
        if(gp->weightsp)    delete gp->weightsp;
        LOGI("~Graph8");
        if(gp->animations)  delete gp->animations;
        LOGI("~Graph9");
        if(gp) delete gp;
        LOGI("~Graph10");
}


#undef numofNewMtl
#undef Newmtl
#undef Animation
#undef EntityData
#undef gp
#undef Movement
