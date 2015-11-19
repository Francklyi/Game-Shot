#ifndef _MAP_H_
#define _MAP_H_
#include <vector>
#include "stdio.h"
#include "stdlib.h"
#include "scene.h"
#include "graphic/GraphRace.h"
#include "roles/LeadRole.h"
#include "roles/ArmyRole.h"

using std::vector;

class Map
{
public:
	LeadRole *pLR;

	GraphRace* enemyRace;

	char *path;
	//static char folder[];
	int sceneNum;
	//char *scenePath;
	int scenePos;
	//vector<char *> scenePaths;
	vector<Scene *> vScenes;



	Map();
	~Map();

	void load(float *pf);

	void setPLR(LeadRole *pLR_);

	void setEnemyRace( GraphRace * enemyRace_);

	void update();

};

#endif //_MAP_H_
