#include <jni.h>
#include <stdio.h>
#include "physic.h"
#include "log.h"

Physic *physic= new Physic();

Physic::Physic()
:g(-100)
{
  LOGI("init Physic");
  build();
  //setEnvironBox1();
}

void Physic::build()
{

        //LOGI("initBt 1");
        //float Restn = 1.2f;

        broadphase = new btDbvtBroadphase();

        collisionConfiguration = new btDefaultCollisionConfiguration();

        dispatcher = new btCollisionDispatcher(collisionConfiguration);

        solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0,g,0));
}

void Physic::setEnvironBox()
{
        float Restn = 0.4f;
        float one = 24.144f;
        btCollisionShape* groundShape ;

{//buttom
        groundShape =new btStaticPlaneShape(btVector3(0,1,0),0);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));

        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(0.0f);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//top
        groundShape =new btStaticPlaneShape(btVector3(0,-1,0),-8.0);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//front
        groundShape =new btStaticPlaneShape(btVector3(0,0,-1),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//back
        groundShape =new btStaticPlaneShape(btVector3(0,0,1),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//left
        groundShape =new btStaticPlaneShape(btVector3(1,0,0),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//right
        groundShape =new btStaticPlaneShape(btVector3(-1,0,0),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

}

void Physic::setEnvironBox1()
{
        float Restn = 0.4f;
        float width = 4.0f;
        float height = 100.0f;
        btCollisionShape* groundShape ;

{//buttom
        groundShape =new btStaticPlaneShape(btVector3(0,1,0),0);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));

        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(0.0f);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//top
        groundShape =new btStaticPlaneShape(btVector3(0,-1,0),-8.0);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//front
        groundShape =new btStaticPlaneShape(btVector3(0,0,-1),-height * 4);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}
/*
{//back
        groundShape =new btStaticPlaneShape(btVector3(0,0,1),-height * 15);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}
*/
{//left
        groundShape =new btStaticPlaneShape(btVector3(1,0,0),-width);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//right
        groundShape =new btStaticPlaneShape(btVector3(-1,0,0),-width);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

}

void Physic::exit()
{
        //dynamicsWorld->removeRigidBody(fallRigidBody);
        //delete fallRigidBody->getMotionState();
        //delete fallRigidBody;

        //delete fallRigidBody2->getMotionState();
        //delete fallRigidBody2;

        //remove the rigidbodies from the dynamics world and delete them
        int i;
        for (i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
        {
                btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = btRigidBody::upcast(obj);
                if (body && body->getMotionState())
                {
                        delete body->getMotionState();
                }
                dynamicsWorld->removeCollisionObject( obj );
                delete obj;
        }
        //dynamicsWorld->removeRigidBody(groundRigidBody);
        //delete groundRigidBody->getMotionState();
        //delete groundRigidBody;


        //delete fallShape;

        //delete fallShape2;

        //delete groundShape;
        //delete collision shapes

        for (int j=0;j<m_collisionShapes.size();j++)
        {
                btCollisionShape* shape = m_collisionShapes[j];
                delete shape;
        }
        m_collisionShapes.clear();

        delete dynamicsWorld;
        delete solver;
        delete collisionConfiguration;
        delete dispatcher;
        delete broadphase;
}

void Physic::step(int step)
{
  //LOGI("STEP 1");
  if(step)
  dynamicsWorld->stepSimulation(1/30.f,step);
 // else
   // dynamicsWorld->stepSimulation(0,-1,0);

  //dynamicsWorld->debugDrawWorld();
  //LOGI("STEP 2");
}

bool Physic::init()
{

}

bool Physic::bind()
{

}

Physic::~Physic()
{
  exit();
}
