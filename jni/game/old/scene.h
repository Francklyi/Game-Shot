#ifndef _SCENE_H_
#define _SCENE_H_
//#include "role.h"
#include "GData.h"
#include "Entity.h"
#include "EntityFactory.h"
#include "map.h"
#include "Entitylist.h"
//#include "terrain.h"

/*
 * 该类是游戏中的主要的一个类，游戏以一个个场景为单位，绘制该场景中相关的物体和位置等
 *
 */
class Scene : public Entity
{
public:
	Scene(){};
	Scene(char *fileName_,Map *map_);
	~Scene();
	void init(char *fileName_);
	bool loadConfiguration();
	virtual void loadXml();
	bool loadData();
	bool update();
	void setData();
	EntityFactory * getRoleFactoryPtr(){return entityFactoryP;}
	void toNext();
	void toPre();
private:
	int next;
	int pre;
	Map *map;
	Graph *graph;//与该场景本身的固定图形
	//Terrain terrain;
	//vector<Role> Roles;//与该场景相联系的 单独的物体 ,可能是敌人、朋友、路人（物）
	//vector<Vector3> RolesPosition;//对应的物体的位置 与上一个vector一样长
	//vector<Entity> goods;
	char *chars;
	char *fileName;
	char *geoFile;
	EntityFactory *entityFactoryP;
	Entitylist *begin;
	Entitylist *end;
};
#endif  //_SCENE_H_
