#include "EntityPhysic.h"
#include <GLES/gl.h>
#include <EGL/egl.h>
#include <string.h>
#include "GLSL/GLSL.h"
//#include "GData.h"
#include "log.h"

EntityPhysic::EntityPhysic()
:matrix(NULL)
,pos(NULL)
,velocity(NULL)
,mass(0)
,restitution(0)
,shape(NULL)
,body(NULL)
,isRemoved(false)
{

}

EntityPhysic::~EntityPhysic()
{

}

void EntityPhysic::reset()
{
	//if(isRemoved) addToWorld();
	//btTransform startTransform;
	//startTransform.setIdentity();
	//startTransform.setFromOpenGLMatrix(matrix);
	body->getWorldTransform().setIdentity();
	body->getWorldTransform().setFromOpenGLMatrix(matrix);

	body->setLinearVelocity(btVector3(80,0,50));
	//body->setLinearVelocity(btVector3(0,0,0));//*velocity
	//body->setAngularVelocity(btVector3(0,0,0));
}

void EntityPhysic::addToWorld()
{
	if (body && body->getMotionState())
	{
		isRemoved=false;
		physic->dynamicsWorld->addRigidBody(body);

	}
}

void EntityPhysic::remove()
{
	if (body && body->getMotionState())
	{
		isRemoved=true;
		physic->dynamicsWorld->removeCollisionObject( body );

	}
}

bool EntityPhysic::getIsRemoved()
{
	return isRemoved;
}

void EntityPhysic::setShape(int type, Entity *pMShape)
{
	LOGI("shapeType: %d",type);
	switch(type)
	{
	case BOX:
		shape=new btBoxShape(
				btVector3(	pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[0]*0.5,
							pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[5]*0.5,
							pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[10]*0.5) );
		break;
	case SPHERE:
		//shape =new btCylinderShape(
		//		btVector3(pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[0]*0.5,\
		//		pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[5]*0.5,\
		//		pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[5]*0.48));
		shape =new btSphereShape(pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[0]*0.5);
		break;
	case CYLINDER://cylinder  : r , 高的一半, 重心：从最上面开始是0
		//shape =new btCylinderShape(
		//		btVector3(pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[0]*0.5,\
				pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[5]*0.5,\
				pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[5]*0.48));
		//shape=new btBoxShape(\
				btVector3(	pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[0]*0.5,\
							pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[5]*0.5,\
							pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[10]*0.5) );
		shape =new btSphereShape(pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[0]*0.5);
		break;
	}
}

bool EntityPhysic::update()
{
  //LOGI("EntityPhysic::update 0");
	//body->activate(true);
  //onStep();
  //LOGI("EntityPhysic::update 01");
  //body->setAngularVelocity(btVector3(0, 3.14f, 0));

  //***获取变换矩阵
        //if(!body) LOGI("Role::AEA 0.5");
        //Matrix::setIdentityM(matrix,0);
  glsl->chooseMatrix(GLSL::MMATRIX);

  //glsl->translate(24.0f,0.0f,24.0f);
  //***获取变换矩阵
  float matrix_[16];

  body->getWorldTransform().getOpenGLMatrix(matrix_);
  //LOGI("EntityPhysic::update 02");
  glsl->multMatrix(matrix_);

  glsl->multMatrix(pE->pTransform->pSOmatrix->mMatrixQueue.back());
  //delete matrix_;
  //glsl->scale(2.4f,1.0f,0.2f);
  //LOGI("AEA:pos:%f,%f",pos->x(),pos->y());

  //glsl->translate(pos->x() , pos->y() , pos->z() );
        /* other thing about attr*/
  //LOGI("EntityPhysic::update 1");
  return true;
}

void EntityPhysic::destroy(Physic *physic)
{
	if(body && body->getMotionState())
	{
		delete body->getMotionState();
	}
	physic->dynamicsWorld->removeCollisionObject(body);
	delete body;

	delete shape;
}

void EntityPhysic::setPos(btVector3 *pos_)
{
    pos=pos_;
    btTransform trans=body->getWorldTransform();
    trans.setOrigin(*pos);
    body->setWorldTransform(trans);
}

/*
 * renew pos
 */
btVector3* EntityPhysic::getPos()
{
	*pos=body->getWorldTransform().getOrigin();
	return pos;
}

void EntityPhysic::setVelocity(btVector3 *velocity_)
{
	velocity=velocity_;
	body->setLinearVelocity(*velocity);
}

btVector3 EntityPhysic::getVelocity()
{
	if (body && body->getMotionState())
	{
		btVector3 velocity_=body->getLinearVelocity();
		return velocity_;
	}

	return btVector3(0,0,0);
}

void EntityPhysic::setPhysic()
{
	//shape=setShape();
	if(shape!=NULL)
	{
		//m_collisionShapes.push_back(fallShape2);

		// LOGI("AEA::setPhysic1:,%f,%f",pos->x(),pos->y());

		//rotate=btVector3(0, 1.5f, 0);

		btTransform startTransform;
		startTransform.setIdentity();
		startTransform.setFromOpenGLMatrix(matrix);
		//startTransform.setOrigin(*pos);
		//startTransform.setRotation(btQuaternion(0,rotate.y(),0,1));
		//LOGI("AEA::setPhysic2");
		btDefaultMotionState* fallMotionState =
				new btDefaultMotionState(startTransform);
		//LOGI("AEA::setPhysic3");
		//restitution=1.5f;
		//mass = 20.0f;
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

void EntityPhysic::onStep()
{
    //glPushMatrix();
    if (body && body->getMotionState())
      {

        //renew pos
        //getPos();
        //(*velocity)=body->getLinearVelocity();
        //(*velocity)=colVelocity((*velocity));
        //body->setLinearVelocity((*velocity));
    	btVector3 velocity_=body->getLinearVelocity();
    	velocity_=colVelocity(velocity_);
    	body->setLinearVelocity(velocity_);
       }
        //draw();
        //glPopMatrix();
}

btVector3 EntityPhysic::colVelocity(btVector3 velocity_)
{

	int MAX=120;
	int MIN=50;
	if(velocity_.length()>MAX)
	{
		float radio=velocity_.length()/MAX;
		velocity_=btVector3(velocity_.getX()/radio,
				velocity_.getY()/radio,
				velocity_.getZ()/radio);
	}
	if(velocity_.length()<MIN)
	{
		float radio=velocity_.length()/MIN;
		velocity_=btVector3(velocity_.getX()/radio,
				velocity_.getY()/radio,
				velocity_.getZ()/radio);
	}
	return velocity_;
}
