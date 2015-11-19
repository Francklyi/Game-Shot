#ifndef _LOBJECT_H_
#define _LOBJECT_H_

#include <btBulletDynamicsCommon.h>
#include <GLES/gl.h>

#include "physic.h"

class Lmaterial
{
public:
        float r,g,b,a;//color;
        btScalar mass;
        btScalar restitution;
        Lmaterial();
        Lmaterial(float r, float g, float b, float a);
        ~Lmaterial();
        void set();
        void setMass(btScalar mass_){ mass = mass_; }
        void setRestitution(btScalar restitution_){ restitution = restitution_; }
};


class LObject
{
public:
        LObject();
        ~LObject();
        virtual bool update();

        void stop();
        void build();
        void destroy(Physic *physic_);
        void setPhysic(Physic *physic_);
        btVector3* getPos();

        virtual btCollisionShape* setShape()=0;
        void setVelocity(btVector3 *velocity_);
        void setPos(btVector3 *pos_);
        //virtual void draw()=0;
        btRigidBody* getBody() { return body; }
        btCollisionShape*  getShape() { return shape; }
        btVector3 colVelocity(btVector3 velocity_);
        void onStep();
        bool isType(LObject *object_);
protected:
        btVector3 *pos;
        btVector3 *velocity;

        btCollisionShape* shape;
        btRigidBody* body;
        //btDiscreteDynamicsWorld* dynamicsWorld;
};

#endif //_LOBJECT_H_
