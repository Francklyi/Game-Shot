#include "AEA.h"
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <string.h>
#include "GLSL/GLSL.h"
//#include "GData.h"
#include "log.h"
#include "physic.h"
#include "game.h"

//btCollisionShape* AEA::shape = 0;

AEA::AEA(Attribute *attrp_,int classid_):
attrp(attrp_)
,filePath(0)
,classid(classid_)
, order(-1)//,LObject()
,movement_code(-1)
,velocity(new btVector3(0,0,0))
,matrix((float*)malloc(16 * sizeof(float)))
{

}

bool AEA::update()
{
        //LOGI("Role::AEA 0");

  if(Game::state1==1)
    {
      onStep();
    }
  //body->setAngularVelocity(btVector3(0, 3.14f, 0));

  //***获取变换矩阵
        //if(!body) LOGI("Role::AEA 0.5");
        //Matrix::setIdentityM(matrix,0);
  glsl->chooseMatrix(GLSL::MMATRIX);

  //glsl->translate(24.0f,0.0f,24.0f);
  //***获取变换矩阵
  float matrix_[16];

  body->getWorldTransform().getOpenGLMatrix(matrix_);

  glsl->multMatrix(matrix_);

  //glsl->scale(2.4f,1.0f,0.2f);
  //LOGI("AEA:pos:%f,%f",pos->x(),pos->y());

  //glsl->translate(pos->x() , pos->y() , pos->z() );
        /* other thing about attr*/
        //LOGI("Role::update 1");
  return true;
}

void AEA::destroy(Physic *physic_)
{
        if(body && body->getMotionState())
        {
                delete body->getMotionState();
        }
        physic->dynamicsWorld->removeCollisionObject(body);
        delete body;

        delete shape;
}

void AEA::setPos(btVector3 *pos_)
{
    pos=pos_;
    btTransform trans=body->getWorldTransform();
    trans.setOrigin(*pos);
    body->setWorldTransform(trans);
}

/*
 * renew pos
 */
btVector3* AEA::getPos()
{
        *pos=body->getWorldTransform().getOrigin();
        return pos;
}

void AEA::setVelocity(btVector3 *velocity_)
{
        velocity=velocity_;
        body->setLinearVelocity(*velocity);
}

void AEA::setPhysic(Physic *physic_)
{
        //shape=setShape();
        if(shape!=NULL)
        {
                //m_collisionShapes.push_back(fallShape2);

               // LOGI("AEA::setPhysic1:,%f,%f",pos->x(),pos->y());

            //rotate=btVector3(0, 1.5f, 0);

                btTransform startTransform;
                startTransform.setIdentity();
                startTransform.setOrigin(*pos);
                startTransform.setRotation(btQuaternion(0,rotate.y(),0,1));
//LOGI("AEA::setPhysic2");
                btDefaultMotionState* fallMotionState =
                                new btDefaultMotionState(startTransform);
//LOGI("AEA::setPhysic3");
                restitution=1.5f;
                mass = 20.0f;
                btVector3 fallInertia(0,0,0);
                shape->calculateLocalInertia(mass,fallInertia);
                btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,shape,fallInertia);
                body = new btRigidBody(fallRigidBodyCI);
                //LOGI("AEA::setPhysic3.5");
                //move_RigidBodys.push_back(fallRigidBody);
                body->setRestitution(restitution);
                const btVector3 v(0,0,0);
                body->setLinearVelocity(v);//*velocity
                //body->setAngularVelocity(btVector3(0,15,10));
                LOGI("AEA::setPhysic4");
                physic->dynamicsWorld->addRigidBody(body);
        }
}

void AEA::onStep()
{
    //glPushMatrix();
    if (body && body->getMotionState())
      {

        //renew pos
        getPos();
        (*velocity)=body->getLinearVelocity();
        (*velocity)=colVelocity((*velocity));
        body->setLinearVelocity((*velocity));
       }
        //draw();
        //glPopMatrix();
}

btVector3 AEA::colVelocity(btVector3 velocity_)
{
  float damp=0.99f; //阻尼
  velocity_*=damp;

        float max=20 ,  max1=20;
        float min=0.0f ,  min1=0.0f;

        btVector3 va=body->getAngularVelocity();
        if(va.length())
          {
        va*=0.95;
        body->setAngularVelocity(va);
          }
////X////
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
/////Y//////////////////////////////
        //velocity_.setY(0);

        if(velocity_.getY()>max1||velocity_.getY()<-max1)
        {
                if(velocity_.getY()>0)
                        velocity_.setY(max);//max
                else
                        if(velocity_.getY()<0)
                                velocity_.setY(-max);
        }
        else
                if(velocity_.getY()<min1&&velocity_.getY()>-min1)
                        {
                                if(velocity_.getY()>0)
                                        velocity_.setY(min);//min
                                else
                                        if(velocity_.getY()<=0)
                                                velocity_.setY(-min);
                        }

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

AEA::~AEA()
{
  if(attrp) delete attrp;
  attrp= 0;
  if(pos) delete pos;
  pos= 0;
  if(velocity) delete velocity;
  velocity= 0;
  if(shape) delete shape;
  shape= 0;
  if(attrp) delete attrp;
  attrp= 0;

  if(filePath) delete filePath;
  filePath= 0;
  if(typeName) delete typeName;
  typeName= 0;
  if(matrix) delete matrix;
  matrix= 0;
  if (body && body->getMotionState())
  {
      physic->dynamicsWorld->removeRigidBody(body);
      delete body->getMotionState();
  }
  if(body) delete body;
  body= 0;
  //dynamicsWorld->removeCollisionObject( obj );
}
