#pragma once

#include "PhyPlane.h"
#include "graphic/GraphRace.h"
#include "roles/LeadRole.h"
#include "roles/ArmyRole.h"
#include "PhyScene.h"
#include "EnemyGroup.h"
#include "roles/coin.h"
#include "roles/ArmyRole.h"
#include "roles/Thorn.h"
#include "roles/Thornball.h"

#include <vector>

using std::vector;


class Scene
{
public:
	int id;

	bool isInGame;

	LeadRole *pLR;

	vector<ArmyRole*> vEnemy;

	int num_enemys;

	vector<GraphRace*> vEnemyRace;

	PhyScene *pPScene;

	float *enemyConfig;

	EnemyGroup *pEnemyGroup;

	Graph *pG_floor;

	vector<Coin*> vCoin;

	vector<Thorn*> vThorn;

	vector<Thornball*> vThornball;

	//vector<RoleSet*> vRoleSet;

	int num_coins;

	int num_thorns;

	int num_thornballs;

	static float *f4;

	float rangeOrg12[12];

	float *rangeNew;

	static float *f32;

	static int inScreenSum;

	int type;

	enum { TYPE_STATIC, TYPE_MOVE_TRANSLATE, TYPE_MOVE_SCALE, TYPE_MOVE_ROTATE };

	float posStart0[3];

	float posEnd[3];

	float posMove[3];

	float *pfMoveSteps;

	float *pfMoveStepNow;

	int step;

	int allSteps;

	int oneMaxSteps;

	int sceneType;

	enum {
		STYPE_QUAD=0, STYPE_CYLINDER, STYPE_PIPE, STYPE_CUBE
	};

	float pf3MoveAll[3];

	Scene();

	~Scene();

	void init();

	void setType(int type_);

	void addEnemyRace( GraphRace * pER);

	void setFloor(Graph* pGraph);

	void setfloorPhy(float *pf);

	void setPhyScene(PhyScene *pPScene_);

	void renewPhyScene();

	void setPLR(LeadRole *pLR_);

	void loadPhysic();

	bool loadData();

	bool update();

	bool updateLess();

	void updateMove();

	void updateMoveLess();

	void loadData(float *pdata);

	void updateOneEnemy(ArmyRole* pAR);

	bool isInScreen();

	void toNext();

	void toPre();

	void reset();

	void setfloorPhyNew();

	void initPhy();

	void getMovesteps();

	void setCirclePhy(float R, float h);

};

