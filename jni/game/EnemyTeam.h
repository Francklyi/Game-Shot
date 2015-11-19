#pragma once
#include "graphic/GraphRace.h"
#include "roles/LeadRole.h"
#include "roles/ArmyRole.h"
#include <vector>

using std::vector;

class EnemyTeam
{
public:
	bool isInGame;

	int numOfEnemy;

	float *pfData;

	LeadRole *pLR;

	GraphRace *enemyRace;

	vector<float *> vpf;

	EnemyTeam();

	void setPLR(LeadRole *pLR_);

	void setEnemyRace(GraphRace *pER);

	void setData(float *pfData_);

	void addToGame();

	bool update();

	void addOneToGame(float *info);

	int updateEnemys();

	void updateOneEnemy(ArmyRole* pAR);
};
