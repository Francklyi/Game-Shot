#pragma once
#include "EnemyTeam.h"
#include <vector>

using std::vector;

class EnemyGroup
{
public:
	int teamN;

	LeadRole *pLR;

	GraphRace *enemyRace;

	vector<EnemyTeam*> vpET;

	EnemyGroup();

	void loadData(float *pf);

	void addEnemyTeam(EnemyTeam * pET);

	void addATeamToGame();

	void setEnemyRace(GraphRace *pER);

	void setPLR(LeadRole *pLR_);

	bool update();
};
