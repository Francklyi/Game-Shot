#ifndef _ENTITYLIST_H_
#define _ENTITYLIST_H_
#include "AEA.h"
#include "Entity.h"
#include "movement.h"
#include <stdio.h>
#include <btBulletDynamicsCommon.h>

/*
 * list of AEAs
 */
class Entitylist :public Entity
{
public:
    int id;
    int order;//Entitylist 排序
    vector<AEA *> AEAs;
    int num; // number of aea
    char *xmlfilePath;
    char *geoFile;
    Attribute *attr; //用于暂时存储 AEA中的属性
    //Entity *entity;
    Graph *graph;
    int num_movements;
    int *having_movement_ids;
    vector<Movement*> Movements;

    btCollisionShape* shape;

    float mass;

    float restitution;

    Entitylist():xmlfilePath(0),geoFile(0),num(0),AEAs(0),Movements(0){}

    Entitylist(int id_,char * xmlfilePath_);

    ~Entitylist();

    void loadXml();

    bool load();

    bool update();

    bool add_movement(Movement *movement_)
    {
      Movements.push_back(movement_);
    }

    bool select_movement(int id_);

    void setAction(int code_);//运动对外接口

    Animation* match_movement(int code_);

    int registerRole(AEA *aea_);

    int removeRole(int order);

    void setOrder(int order_){order=order_;}

    void setFilePath(char *geoFile_){ geoFile=geoFile_; }

    void setHavingMovementids(int *ids) { having_movement_ids= ids; }

    void setNum_movements(int num) { num_movements = num; }

    void setShape(int type , float *paras);

    btCollisionShape*  getShape() { return shape; }
};

#endif //_ENTITYLIST_H_
