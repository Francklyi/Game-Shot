#include "LObject.h"
#include "log.h"

LObject::LObject()
:
pos(new btVector3(btScalar(0),btScalar(0),btScalar(0))),
velocity(new btVector3(btScalar(0),btScalar(0),btScalar(0)))
{
        //build();
}

LObject::~LObject()
{
        //destroy();
        if(shape!=NULL) delete shape;
        shape=0;
}

bool LObject::update()
{
        //onMeterial();
        //onStep();
}

void LObject::build()
{
        //set bt
        //setBT();
        //set meterial
}

void LObject::destroy(Physic *physic_)
{
        if(body && body->getMotionState())
        {
                delete body->getMotionState();
        }
        physic_->dynamicsWorld->removeCollisionObject(body);
        delete body;

        delete shape;
}

void LObject::setPos(btVector3 *pos_)
{
        pos=pos_;
    btTransform trans=body->getWorldTransform();
    trans.setOrigin(*pos);
    body->setWorldTransform(trans);
}

btVector3* LObject::getPos()
{
        *pos=body->getWorldTransform().getOrigin();
        return pos;
}

void LObject::setVelocity(btVector3 *velocity_)
{
        velocity=velocity_;
        body->setLinearVelocity(*velocity);
}

void LObject::setPhysic(Physic *physic_)
{
        shape=setShape();
        if(shape!=NULL&&body!=NULL)
        {
                //m_collisionShapes.push_back(fallShape2);

                LOGI("LObject::setPhysic");
                btDefaultMotionState* fallMotionState =
                                new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,10,0)));
                btScalar mass;
                btScalar restitution;
                mass = 10;
                btVector3 fallInertia(0,0,0);
                shape->calculateLocalInertia(mass,fallInertia);
                btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,shape,fallInertia);
                body = new btRigidBody(fallRigidBodyCI);

                //move_RigidBodys.push_back(fallRigidBody);
                body->setRestitution(restitution);
                body->setLinearVelocity(*velocity);
                //fallRigidBody->setAngularVelocity(btVector3(5,0,0));

                physic_->dynamicsWorld->addRigidBody(body);
        }
}

void LObject::onStep()
{
    //glPushMatrix();
    if (false&&body && body->getMotionState())
      {
        LOGI("onStep: %f, %f, %f",pos->getX(), pos->getY() , pos->getZ());
        pos=getPos();
        glTranslatef( pos->getX(), pos->getY() , pos->getZ());

        (*velocity)=body->getLinearVelocity();
        (*velocity)=colVelocity((*velocity));
        body->setLinearVelocity((*velocity));
       }
        //draw();
        //glPopMatrix();
}

btVector3 LObject::colVelocity(btVector3 velocity_)
{
        float max=50 , max1=50;
        float min=5,   min1=5;
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
///////////////////////////////////
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
////////////////////////////////////
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

bool LObject::isType(LObject *object_)
{

}



///////Lmaterial///////////////////////////////
Lmaterial::Lmaterial():r(1),g(1), b(1), a(1)
,mass(1)
,restitution(1)
{

}

Lmaterial::Lmaterial(float r_, float g_, float b_, float a_)
:r(r_),g(g_), b(b_), a(a_)
,mass(1)
,restitution(1)
{

}

Lmaterial::~Lmaterial()
{

}

void Lmaterial::set()
{
        float mat_amb[]={r,g,b,a};
        float mat_diff[]={1.0f,1.0f,1.0f,1.0f};
        float mat_spe[]={0.0f,1.0f,1.0f,1.0f};
        float mat_shin[]={0.0f,1.0f,0.0f,1.0f};
        glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, mat_amb);
        glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diff);
        glMaterialfv(GL_FRONT,GL_SPECULAR, mat_spe);
        glMaterialfv(GL_FRONT,GL_SHININESS, mat_shin);
}
