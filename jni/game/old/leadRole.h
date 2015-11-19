#ifndef _LEADROLE_H_
#define _LEADROLE_H_
#include "Entity.h"
#include "Vector3.h"
#include "attribute.h"
#include "movement.h"
#include <btBulletDynamicsCommon.h>
#include "physic.h"

class LeadRole : public Entity
{
public:
    char *xmlfilePath;

    char *geoFile;

    Attribute *attr;

    static int state;// 0: stop, 1:moving
    //Entity *entity;
    Graph *graph;

    int num_movements;

    int *movement_id;

    vector<Movement*> Movements;

    float r;

    static btVector3 *pos;

    btVector3 *velocity;

    float mass;

    float restitution;

    btCollisionShape* shape;

    btRigidBody* body;

    float  *matrix;

    float vx, vy, vz;
public:
	LeadRole():xmlfilePath(0),geoFile(0),attr(0),graph(0)
                   {}

	LeadRole(Attribute *attr_,char * xmlfilePath_,int num_movements_
	    ,int *movement_id_, btVector3 *pos_);

	~LeadRole();

	void loadXml();

	bool load();

	bool update();

	bool select_movement(int id_);

	void setAction(int code_);//运动对外接口

	Animation* match_movement(int code_);

	//*******physic:*********
	void setPhysic(Physic *physic_);

	void setShape(int type , float *paras);

	void setVelocity(btVector3 *velocity_);

	void setVelocity(float x, float y, float z);

	void addVelocity(float x, float y, float z);

	void setPos(btVector3 pos_);

	btVector3* getPos();

	btRigidBody* getBody() { return body; }

	btCollisionShape*  getShape() { return shape; }

	btVector3 colVelocity(btVector3 velocity_);

	void destroy(Physic *physic_);

	void move(float x, float y, float z);
};
#endif //_LEADROLE_H_
