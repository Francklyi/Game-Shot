#ifndef _AEA_H_
#define _AEA_H_
#include <stdio.h>
#include "attribute.h"
//#include "Vector3.h"
#include <btBulletDynamicsCommon.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "physic.h"
#include "shape.h"
/*
 * a Entity attribute
 * only restore a entity attribute data
 */
class AEA
{
public:
  int classid;
  int order;
  int movement_code;

  Attribute *attrp;
  btVector3 *pos;
  btVector3 *velocity;
  btVector3 *avelocity;
  btVector3 rotate;

  float mass;

  float restitution;

  btCollisionShape* shape;
  btRigidBody* body;

private:
   char *typeName;
   char *filePath;
   float  *matrix;

public:
  AEA(): attrp(0), classid(-1), order(-1),//LObject(),
      movement_code(-1){}

  AEA(Attribute *attrp_ ,int classid_);

  ~AEA();

  void setTypeName(char *name){ typeName=name; }

  char *getTypeName(){ return  typeName; }

  void setPath(char *pathi){ filePath=pathi; }

  char *getPath(){ return filePath; }

  bool setMovementCode(int movement_code_ )
  { movement_code=movement_code_; }

  int getMovementCode() { return movement_code; }

  void setOrder(int order_){order=order_;}

  void setPos(int x,int y,int z){ pos=new btVector3(x,y,z); }

  virtual bool update();

  //*******physic:*********
  void setPhysic(Physic *physic_);

  void onStep();

  void setMass(float mass_ ) { mass = mass_; }

  void setRestitution(float restitution_) { restitution = restitution_; }

  void setVelocity(btVector3 *velocity_);

  void setPos(btVector3 *pos_);

  btVector3* getPos();

  void setV(btVector3 *pos_);

  btVector3* getV();

  void setAV(btVector3 *pos_);

  btVector3* getAV();

  void setRotate(btVector3 rotate_){ rotate = rotate_; }

  btVector3 getRotate(){ return rotate; }

  btRigidBody* getBody() { return body; }

  btCollisionShape*  getShape() { return shape; }

  btVector3 colVelocity(btVector3 velocity_);

  void setShape(btCollisionShape* shape_){ shape = shape_; }

  void destroy(Physic *physic_);

};

#endif //_AEA_H_
