#pragma once
#include "physic.h"
#include "Entity.h"

class EntityPhysic
{
public:
	EntityPhysic();
	~EntityPhysic();

	Entity *pE;

	float *matrix;

	btVector3 posOrig;
	btVector3 *pos;
	btVector3 *velocity;
	btVector3 rotate;

	int shapeType; //shape type : 0: BOX, 1: SPHERE

	enum { BOX, SPHERE, CYLINDER };

	float mass;

	float restitution;

	btCollisionShape* shape;

	btRigidBody* body;

	bool isRemoved;

	void setPGraph(Entity *pE_) { pE= pE_;}

	bool update();

	void setPos(int x,int y,int z){ pos=new btVector3(x,y,z); }

	void setPhysic();

	void onStep();

	void setShapeType(int ST_){ shapeType= ST_; }

	void setMass(float mass_ ) { mass = mass_; }

	void setRestitution(float restitution_) { restitution = restitution_; }

	void setShape(int type, Entity *pMshape);

	void setVelocity(btVector3 *velocity_);

	void setPos(btVector3 *pos_);

	btVector3* getPos();

	//void setVelocity(btVector3 *v_);

	btVector3 getVelocity();

	//void setAV(btVector3 *pos_);

	//btVector3* getAV();

	void setRotate(btVector3 rotate_){ rotate = rotate_; }

	btVector3 getRotate(){ return rotate; }

	btRigidBody* getBody() { return body; }

	btCollisionShape*  getShape() { return shape; }

	btVector3 colVelocity(btVector3 velocity_);

	void setShape(btCollisionShape* shape_){ shape = shape_; }

	void destroy(Physic *physic_);

	void remove();

	void addToWorld();

	bool getIsRemoved();

	void reset();
};
