#include <libxml/parser.h>
#include <libxml/tree.h>
#include "leadRole.h"
#include "fileDeal/loadFile.h"
#include "log.h"
#include <stdlib.h>
#include "game.h"

char PATH1[]="./sdcard/gameData/";

btVector3 *LeadRole::pos =new btVector3(10,0,5);

int LeadRole::state = 0;

LeadRole::LeadRole(Attribute *attr_,char * xmlfilePath_,int num_movements_,int *movement_id_, btVector3 *pos_):
attr(attr_)
,xmlfilePath(xmlfilePath_)
,num_movements(num_movements_)
,movement_id(movement_id_)
,velocity(new btVector3(0,0,0))
,matrix((float*)malloc(16 * sizeof(float)))
,vx(0)
,vy(0)
,vz(0)
{
  LOGE("LeadRole begin");
  Game::state1=1;
	this->loadXml();
	this->load();
	pos=pos_;
	setPhysic(physic);
}

LeadRole::~LeadRole()
{
  LOGI("~LeadRole0");
  if(xmlfilePath) delete xmlfilePath;
  xmlfilePath = 0;
  LOGI("~LeadRole1");
  if(geoFile) delete geoFile;
  geoFile = 0;
  LOGI("~LeadRole2");
  if(attr) delete attr;
  attr = 0;
  LOGI("~LeadRole3");
  if(graph) delete graph;
  graph = 0;
  LOGI("~LeadRole4");
  if(movement_id) delete movement_id;
  movement_id = 0;
  LOGI("~LeadRole5");
    for(int i=0; i<Movements.size();i++)
      {
        if(Movements[i]) delete Movements[i];
      }
    LOGI("~LeadRole6");
    if (body && body->getMotionState())
    {
        physic->dynamicsWorld->removeRigidBody(body);
        delete body->getMotionState();
    }
  LOGI("~LeadRole end");
}

bool LeadRole::select_movement(int id_)
{
	for(int i=0;i<num_movements;i++)
	{
		if(movement_id[i]==id_)
		{
			return true;
		}
	}
	return false;
}

Animation* LeadRole::match_movement(int code_)
{
	for(int i=0;i<num_movements;i++)
	{
		if(Movements[i]->getCode()==code_) return Movements[i]->getAnimation();
	}
	return NULL;
}


bool LeadRole::load()
{
	//LOGI("LeadRole::load");
	//char geols[]="manModel2-1.ly";//"jiliye3_1.ly";
	graph=new Graph();
	graph->setFileName(geoFile);
	graph->load();
	return true;
}

bool LeadRole::update()
{

  //btVector3 velocity_ls=body->getLinearVelocity();

  *pos=body->getWorldTransform().getOrigin();

  LeadRole::pos->setY(0);

  btTransform trans=body->getWorldTransform();
  trans.setOrigin(*pos);
  body->setWorldTransform(trans);

  body->setAngularVelocity(btVector3(0,0,0));

  glsl->chooseMatrix(GLSL::MMATRIX);
  glsl->pushMatrix();
  //glsl->rotate( 90, 0, 1, 0 );
  //glsl->scale(0.4f,0.4f,0.4f);
  //glsl->scale(0.4f,0.4f,0.4f);
  //glsl->translate(24.0f,0.0f,24.0f);
  //***获取变换矩阵
  float matrix_[16];

  body->getWorldTransform().getOpenGLMatrix(matrix_);

  glsl->multMatrix(matrix_);



  graph->draw();

  glsl->popMatrix();

  //setVelocity(&velocity_ls);

  //LOGI("Lead pos:%f,%f",pos->x(),pos->z());
  return true;
}

/*
bool LeadRole::update()
{

  btVector3 velocity_ls=body->getLinearVelocity();

  if(Game::state1==1)
    {
  velocity_ls=colVelocity(velocity_ls);
  body->setLinearVelocity(velocity_ls);

  float a=velocity_ls.x();
  float b=velocity_ls.z();

  //float x=-pos->x()-pos->z()*velocity_ls.z()/a;
  //float z=pos->z()-velocity_ls.x()*pos->x()/b;

  if(a<0.1f) b=0,a=0.1f;
  //if(!b) z=pos->z();

  float l=velocity_ls.length();

  body->setAngularVelocity(btVector3(-1*l*b/a,0,l)*.5);

  if(velocity_ls.length()>2) state = 1;//moving
  else state = 0;//stop

  if(!state) setPos(*pos);
  *pos=body->getWorldTransform().getOrigin();

    }

  glsl->chooseMatrix(GLSL::MMATRIX);
  glsl->pushMatrix();

  //glsl->scale(0.4f,0.4f,0.4f);
  //glsl->translate(24.0f,0.0f,24.0f);
  //***获取变换矩阵
  float matrix_[16];

  body->getWorldTransform().getOpenGLMatrix(matrix_);

  glsl->multMatrix(matrix_);

  graph->draw();

  glsl->popMatrix();


  //LOGI("Lead pos:%f,%f",pos->x(),pos->z());
  return true;
}
*/

void LeadRole::setVelocity(float x, float y, float z)
{

  btVector3 velocity_ls(x,y,z);

  velocity_ls=colVelocity(velocity_ls);
  body->setLinearVelocity(velocity_ls);

  body->activate(true);
}

void LeadRole::addVelocity(float x, float y, float z)
{
  //vx += x;
  //vy += y;
  //vz += z;

  btVector3 velocity_ls1(x,y,z);

  btVector3 velocity_ls2=body->getLinearVelocity();

  velocity_ls2 = velocity_ls2 + velocity_ls1;

  velocity_ls2=colVelocity(velocity_ls2);
  body->setLinearVelocity(velocity_ls2);

  body->activate(true);
}

void LeadRole::move(float x, float y, float z)
{

       btVector3 velocity_ls(x,0,z);

       float mx,mz;
       mx= x>0 ? 0.5: -0.5;
       mz= z>0 ? 0.5: -0.5;

       velocity_ls=colVelocity(velocity_ls);
       body->setLinearVelocity(velocity_ls);

       //body->translate(btVector3(mx,0.0,mz) );
       body->activate(true);


  //body->setLinearVelocity(btVector3(30,0,-30));
}

/*
void LeadRole::move(float x, float y, float z)
{
  if(!state)
    {
       btVector3 velocity_ls(x,0,z);
       velocity_ls=colVelocity(velocity_ls);
       body->setLinearVelocity(velocity_ls);

       body->activate(true);
    }

  //body->setLinearVelocity(btVector3(30,0,-30));
}
*/

void LeadRole::setAction(int code_)
{
  //LOGE("LeadRole::setAction");
  //graph->setAction(match_movement(code_));
  body->setLinearVelocity(btVector3(30,0,-30));
}

void LeadRole::destroy(Physic *physic_)
{
        if(body && body->getMotionState())
        {
                delete body->getMotionState();
        }
        physic_->dynamicsWorld->removeCollisionObject(body);
        delete body;

        delete shape;
}

void LeadRole::setPos(btVector3 pos_)
{
  LeadRole::pos->setX(pos_.x());
  LeadRole::pos->setY(pos_.y());
  LeadRole::pos->setZ(pos_.z());

    btTransform trans=body->getWorldTransform();
    trans.setOrigin(pos_);
    body->setWorldTransform(trans);
}

/*
 * renew pos
 */
btVector3* LeadRole::getPos()
{
        *pos=body->getWorldTransform().getOrigin();
        return pos;
}

void LeadRole::setVelocity(btVector3 *velocity_)
{
        velocity=velocity_;
        body->setLinearVelocity(*velocity);
}

void LeadRole::setShape(int type , float *paras)
{
  switch(type)
  {
  case SPHERE://shpere
    shape =new btSphereShape(paras[0]);
    break;
  case BOX://box
    shape =new btBoxShape(btVector3(paras[0],paras[1],paras[2]));
    break;
  case 3://shpere
    shape =new btSphereShape(paras[0]);
    break;
  default:
    shape =new btSphereShape(paras[0]);
    break;
  }
}

void LeadRole::setPhysic(Physic *physic_)
{
        //shape=setShape();
        if(shape!=NULL)
        {
              //  LOGI("LeadRole::setPhysic");

                btTransform startTransform;
                startTransform.setIdentity();
                startTransform.setOrigin(btVector3(1,4,5));//*pos+

                btDefaultMotionState* fallMotionState =
                                new btDefaultMotionState(startTransform);
                mass=5000;
                restitution=0.4f;
                //mass = 10;
                btVector3 fallInertia(0,0,0);
                shape->calculateLocalInertia(mass,fallInertia);
                btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,shape,fallInertia);
                body = new btRigidBody(fallRigidBodyCI);

                //move_RigidBodys.push_back(fallRigidBody);*velocity
                body->setRestitution(restitution);
              //  const btVector3 v(0.2f,0,0.2f);
              //  body->setLinearVelocity(v);
                //fallRigidBody->setAngularVelocity(btVector3(5,0,0));

                physic_->dynamicsWorld->addRigidBody(body);
        }
}

btVector3 LeadRole::colVelocity(btVector3 velocity_)
{
  float damp=0.99f; //阻尼
  velocity_*=damp;
        float max=100 ,   max1=100;
        float min=1 ,   min1=1;
/////X////////////
        if(velocity_.getX()>max1||velocity_.getX()<-max1)
        {
                if(velocity_.getX()>0)
                        velocity_.setX(max);
                else
                        if(velocity_.getX()<0)
                                velocity_.setX(-max);
        }
        else
                if(velocity_.getX()<min1&&velocity_.getX()>-min1)
                        {
                                if(velocity_.getX()>0)
                                        velocity_.setX(min);
                                else
                                        if(velocity_.getX()<=0)
                                                velocity_.setX(-min);
                        }
//////Y/////////////////////////////
        velocity_.setY(0);
        /*
        if(velocity_.getY()>max1||velocity_.getY()<-max1)
        {
                if(velocity_.getY()>0)
                        velocity_.setY(max);
                else
                        if(velocity_.getY()<0)
                                velocity_.setY(-max);
        }
        else
                if(velocity_.getY()<min1&&velocity_.getY()>-min1)
                        {
                                if(velocity_.getY()>0)
                                        velocity_.setY(min);
                                else
                                        if(velocity_.getY()<=0)
                                                velocity_.setY(-min);
                        }
        */
//////Z//////////////////////////////
        if(velocity_.getZ()>max1||velocity_.getZ()<-max1)
        {
                if(velocity_.getZ()>0)
                        velocity_.setZ(max);
                else
                        if(velocity_.getZ()<0)
                                velocity_.setZ(-max);
        }
        else
                if(velocity_.getZ()<min1&&velocity_.getZ()>-min1)
                        {
                                if(velocity_.getZ()>0)
                                        velocity_.setZ(min);
                                else
                                        if(velocity_.getZ()<=0)
                                                velocity_.setZ(-min);
                        }

        return velocity_;
}


void LeadRole::loadXml()
{
        xmlDocPtr doc = NULL;
        xmlNodePtr node = NULL;

        xmlKeepBlanksDefault(0);

        doc = xmlParseFile(strcat(PATH1,xmlfilePath));
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
        //LOGE("LeadRole geoFile: %s",geoFile);

        //****get shape****
        node = node->xmlChildrenNode;//Shape

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
        //LOGI("LeadRole.type:%d",type);

        for(int i=0; i<num; i++)
          {
          paras[i]=(float)atof((char*)attrPtr1->children->content);
          attrPtr1=attrPtr1->next;
          }
        //LOGI("LeadRole geoFile 4");
        setShape(type, paras);
        //LOGI("LeadRole geoFile 5");

        node = node->next;//MoveMents

        num_movements=atoi((char*)node->properties->children->content);

        Movements=vector<Movement*>(0);

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
