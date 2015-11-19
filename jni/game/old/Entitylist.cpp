#include <libxml/parser.h>
#include <libxml/tree.h>
#include "Entitylist.h"
#include "fileDeal/loadFile.h"
#include "log.h"
#include <stdlib.h>
#include "leadRole.h"

char PATH[]="./sdcard/gameData/";

Entitylist::Entitylist(int id_,char * xmlfilePath_):
id(id_),
xmlfilePath(xmlfilePath_),
geoFile(0),
num(0)
{
  LOGE("Entitiylist begin");
        this->loadXml();
        this->load();
}

Entitylist::~Entitylist()
{
  if(shape) delete shape;
  for(int i=0; i< AEAs.size(); ++i)
    {
      if(AEAs[i]) delete AEAs[i];
      AEAs[i] = 0;
    }
  AEAs.clear();
  if(xmlfilePath) delete xmlfilePath;
  xmlfilePath=0;
  if(geoFile) delete geoFile;
  geoFile=0;
  if(attr) delete attr;
  attr=0;
  if(graph) delete graph;
  graph=0;

  if(having_movement_ids) delete having_movement_ids;
  having_movement_ids=0;
  for(int i=0; i< Movements.size(); ++i)
    {
      if(Movements[i]) delete Movements[i];
      Movements[i] = 0;
    }
  Movements.clear();
  if(shape) delete shape;
  shape=0;
}


void Entitylist::loadXml()
{
  //LOGI("loadXml 0");
        xmlDocPtr doc = NULL;
        xmlNodePtr node = NULL;

        xmlKeepBlanksDefault(0);

       // LOGI("loadXml 1:%s",PATH);
        doc = xmlParseFile(combineChars(PATH,xmlfilePath));
       // LOGI("loadXml 2");
        if(doc == NULL)
        {
                return ;
        }
        //get root node
        node = xmlDocGetRootElement(doc);//LeadRole
        if(node == NULL)
        {
                xmlFreeDoc(doc);
                return ;
        }
        //walk the tree
        //
        geoFile=strCopy((char*)node->properties->children->content);
       // LOGI("LeadRole geoFile: %s",geoFile);

        //****get shape****
        node = node->xmlChildrenNode;//Shape
//LOGI("1");
        xmlAttrPtr attrPtr1=node->properties;

        int type=atoi((char*)attrPtr1->children->content);
        attrPtr1=attrPtr1->next;

        mass = atof((char*)attrPtr1->children->content);
        attrPtr1=attrPtr1->next;

        restitution = atof((char*)attrPtr1->children->content);
        attrPtr1=attrPtr1->next;

        int num = atoi((char*)attrPtr1->children->content);
        attrPtr1=attrPtr1->next;

        float *paras=new float[num];

        for(int i=0; i<num; i++)
          {
          paras[i]=(float)atof((char*)attrPtr1->children->content);
          attrPtr1=attrPtr1->next;
          }

        setShape(type, paras);
//LOGI("Entitylist.type:%d",type);
        node = node->next;//MoveMents

        num_movements=atoi((char*)node->properties->children->content);

        Movements=vector<Movement*>(0);
       // LOGI("3");
        node = node->xmlChildrenNode;//moveMent

        for(int i=0;i<num_movements;i++)
        {
                xmlAttrPtr attrPtr=node->properties;
                int id=atoi((char*)attrPtr->children->content);

                if(select_movement(id)){

                attrPtr=attrPtr->next;
                int code=atoi((char*)attrPtr->children->content);

                //LOGI("code:%d",code);

                attrPtr=attrPtr->next;
                char *filePath=strCopy((char*)attrPtr->children->content);

                attrPtr=attrPtr->next;
                int attack=atoi((char*)attrPtr->children->content);

                Movement *movement=new Movement(id,code,attack,filePath);

                Movements.push_back(movement);

                }

                node = node->next;
        }
        xmlFreeDoc(doc);
        xmlCleanupParser();
}

bool Entitylist::select_movement(int id_)
{
        for(int i=0;i<num_movements;i++)
        {
                if(having_movement_ids[i]==id_)
                {
                        return true;
                }
        }
        return false;
}

Animation* Entitylist::match_movement(int code_)
{
        for(int i=0;i<num_movements;i++)
        {
                if(Movements[i]->getCode()==code_) return Movements[i]->getAnimation();
        }
        return NULL;
}


bool Entitylist::load()
{
        LOGI("LeadRole::load");
        //char geols[]="manModel2-1.ly";//"jiliye3_1.ly";
        graph=new Graph();
        graph->setFileName(geoFile);
        graph->load();
        return true;
}

bool Entitylist::update()
{
  //LOGI("Entitylist::update");
  for(int i=0; i< AEAs.size(); ++i)
    {


     // glUniform1i(glsl->isHereHandle,1);
     // glUniform1f(glsl->XHandle, LeadRole::pos->x());
     // glUniform1f(glsl->YHandle, LeadRole::pos->z());

      glsl->pushMatrix();

      //glsl->scale(0.2f,0.2f,0.2f);
      AEAs[i]->update();
      //graph->setAction(match_movement(AEAs[i]->getMovementCode()));

      glsl->chooseMatrix(GLSL::MMATRIX);

      graph->draw();

      glsl->popMatrix();


    }

  return true;
}

void Entitylist::setAction(int code_)
{
  graph->setAction(match_movement(code_));
}

int Entitylist::registerRole(AEA *aea_)
{
  AEAs.push_back(aea_);
  aea_->setOrder(AEAs.size()-1);
  num++;
  //****set physic************
  //if(shape) LOGI("have shape");
     // LOGI("have shape");
  aea_->setMass(mass);

  aea_->setRestitution(restitution);

  aea_->setShape(shape);

  aea_->setPhysic(physic);

  return AEAs.size();
}

int Entitylist::removeRole(int order)
{
        free(AEAs[order]);
        //rolePtrs.erase(order);
        num--;
        return num;
}

void Entitylist::setShape(int type , float *paras)
{
  //shape =new btSphereShape(1);

  switch(type)
  {
  case SPHERE://shpere
    LOGI("Sphere: %f ",paras[0]);
    shape =new btSphereShape(paras[0]);
    break;
  case BOX://box： 高是一半
    LOGI("Box:%f,%f,%f",paras[0],paras[1],paras[2]);
    shape =new btBoxShape(btVector3(paras[0],paras[1],paras[2]));
    break;
  case CYLINDER://cylinder  : r , 高的一半, 重心：从最上面开始是0
    LOGI("Cylinder:%f,%f,%f",paras[0],paras[1],paras[2]);
    shape =new btCylinderShape(btVector3(paras[0],paras[1],paras[1]*1.5f));
    break;
  default:
    shape =new btSphereShape(paras[0]);
    break;
  }

}
