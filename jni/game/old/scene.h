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
 * ��������Ϸ�е���Ҫ��һ���࣬��Ϸ��һ��������Ϊ��λ�����Ƹó�������ص������λ�õ�
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
	Graph *graph;//��ó�������Ĺ̶�ͼ��
	//Terrain terrain;
	//vector<Role> Roles;//��ó�������ϵ�� ���������� ,�����ǵ��ˡ����ѡ�·�ˣ��
	//vector<Vector3> RolesPosition;//��Ӧ�������λ�� ����һ��vectorһ����
	//vector<Entity> goods;
	char *chars;
	char *fileName;
	char *geoFile;
	EntityFactory *entityFactoryP;
	Entitylist *begin;
	Entitylist *end;
};
#endif  //_SCENE_H_
