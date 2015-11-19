#include "EnemyGroup.h"

EnemyGroup::EnemyGroup()
:teamN(0)
{
	vpET=vector<EnemyTeam*>(0);
}

void EnemyGroup::addEnemyTeam(EnemyTeam * pET)
{
	vpET.push_back(pET);
}

void EnemyGroup::loadData(float *pf)
{
	int n=1;
	//LOGI("EnemyGroup::load 0");
	for(int i=0; i< pf[0] ;i++)
	{
		EnemyTeam * pET=new EnemyTeam();
		//LOGI("EnemyGroup::load 1");
		pET->setEnemyRace(enemyRace);
		pET->setPLR(pLR);
		pET->setData(&pf[n]);
		n+=(pf[n]*5)+i+1;
		addEnemyTeam(pET);
		//LOGI("EnemyGroup::load 2");
	}
}

void EnemyGroup::addATeamToGame()
{
	vpET[teamN]->addToGame();
}

void EnemyGroup::setEnemyRace(GraphRace *pER)
{
	enemyRace=pER;
}

void EnemyGroup::setPLR(LeadRole *pLR_)
{
	pLR=pLR_;
}

bool EnemyGroup::update()
{
	//LOGI("teamN, size, %d, %d",teamN, vpET.size());
	if(teamN >= vpET.size())
		return false;
	else
	{
	if(!vpET[teamN]->update())
	{
		//LOGI("new team");
		teamN++;
		//addATeamToGame();
	}
		return true;
	}
}
