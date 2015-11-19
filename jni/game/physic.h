#ifndef _PHYSIC_H_
#define _PHYSIC_H_
#include <btBulletDynamicsCommon.h>
#include "log.h"

enum Shapes { SPHERE=1, BOX, CYLINDER, PLANE };

class Physic
{
public:

        Physic();
        ~Physic();
        void build();
        btDiscreteDynamicsWorld* getDDWorld(){ return dynamicsWorld;}
        void addRigidBody(btRigidBody* fallRigidBody){ dynamicsWorld->addRigidBody(fallRigidBody); }

        void setEnvironBox();

        void setEnvironBox1();

        void step(int step);

        void exit();

        btDiscreteDynamicsWorld* dynamicsWorld ;
	bool init();
	bool bind();
protected:

        btBroadphaseInterface* broadphase;

        btDefaultCollisionConfiguration* collisionConfiguration ;

        btCollisionDispatcher* dispatcher ;

        btSequentialImpulseConstraintSolver* solver ;

        btAlignedObjectArray<btCollisionShape*> m_collisionShapes;

        btAlignedObjectArray<btRigidBody*>      move_RigidBodys;

        int g;

};

extern Physic *physic;
#endif //_PHYSIC_H_
