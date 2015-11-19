#ifndef _SHAPE_H_
#define _SHAPE_H_

#include <btBulletDynamicsCommon.h>

class Shape
{
public:
  char *type;
  float mass;
  float restitution;

  Shape():type(0),mass(0),restitution(1){}
  Shape(char *type_, float mass_, float restitution_)
  :type(type_),mass(mass_),restitution(restitution_)
  {}
  char * getType() { return type; }

  btCollisionShape* getShape(int type)
  {
    btCollisionShape* shape_;
    switch(type)
    {
    case 1://shpere
      shape_ =new btSphereShape(1);
      break;
    default:
      break;
    }
  }
};

class ShapeSphere : public Shape
{
public:
  float R;
  ShapeSphere(char *type_, float mass_, float restitution_,float R_):
    Shape(type_,mass_,restitution_)
  ,R(R_){};
};

class ShapeBox:public Shape
{
public:
  float x , y , z;//³¤¿í¸ß
};
#endif //_SHAPE_H_
