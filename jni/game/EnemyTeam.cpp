#include "EnemyTeam.h"

EnemyTeam::EnemyTeam()
:pfData(0)
,isInGame(false)
,numOfEnemy(0)
{
	vpf=vector<float*>(0);
}

void EnemyTeam::setPLR(LeadRole *pLR_)
{
	pLR=pLR_;
}

void EnemyTeam::setEnemyRace(GraphRace *pER)
{
	enemyRace=pER;
}

void EnemyTeam::setData(float *pf)
{
	for(int i=0; i<pf[0]; i++)
	{
		float *pf5=new float[5];
		pf5[0]=pf[i*5+1];
		pf5[1]=pf[i*5+2];
		pf5[2]=pf[i*5+3];
		pf5[3]=pf[i*5+4];
		pf5[4]=pf[i*5+5];
		vpf.push_back(pf5);
	}
	pfData=pf;
}

void EnemyTeam::addToGame()
{
	for(int i=0; i<vpf.size(); i++)
	{
		addOneToGame(vpf[i]);
	}
	numOfEnemy=vpf.size();
	isInGame=true;
}

bool EnemyTeam::update()
{
	//LOGI("EnemyTeam::update 0 : %d", (int)isInGame );
	if(!isInGame)
		addToGame();
	//LOGI("EnemyTeam::update 1");
	if(updateEnemys()<=0)
		return false;


}

void EnemyTeam::addOneToGame(float *info)
{
	char pARstr[]="model1.ly\0";
	ArmyRole *pAR=new ArmyRole(pARstr);
	//pAR->pTransform->copy(pLR->pTransform);
	//LOGI("info: %f, %f, %f",info[2],info[3],info[4]);
	pAR->setPos(info[2],info[3],info[4]);

	//pAR->setShape();
	//pAR->pMShape->copy(pLR->pMShape);
	//setGraphPhysic(pAR);

	pAR->setHPAll(info[1]);
	pAR->setHP(info[1]);

	pAR->setAim(pLR);


	pAR->nextDelay=100;
	pAR->timeBetweenDelay=0;


	pAR->state=0;

	enemyRace->add(pAR);
	pAR->setVpAFG();
	pAR->act(GraphRace::ID_RUN2);
}

void EnemyTeam::updateOneEnemy(ArmyRole* pAR)
{
	//LOGI("EnemyTeam::updateOneEnemy 0");
	pAR->getEnviromentInfo();
	//LOGI("EnemyTeam::updateOneEnemy 1");

	if(pAR->lengthToEnemy<64||pAR->lengthToEnemy>5500)
	{// 按攻击频率攻击
		if(pAR->moveCode==LeadRole::ID_RUN2
				&&pAR->pAFG->change_stay!=0
				)
		{
			//LOGI("STATE_STOP");
			pAR->pAFG->change_stay = 0;
			//pLR->pEP->body->setLinearVelocity(btVector3(0,0,0));
			pAR->moveState= pAR->STATE_STOP;
		}
	}
	//LOGI("EnemyTeam::updateOneEnemy 2");
	if(pAR->timeBetweenDelay<pAR->nextDelay)
	{
		pAR->timeBetweenDelay++;
	}
	else
	{
		pAR->timeBetweenDelay=0;
		pAR->turnToEnemy();
	if(pAR->lengthToEnemy<5500&&pAR->lengthToEnemy>64)
	{// 靠近

		//if(getRandom(0,100)<20)
			if(pAR->moveState== pAR->STATE_STOP)
			{
				//LOGI("ID_RUN2");
				pAR->act(LeadRole::ID_RUN2);
				pAR->pAFG->change_stay = 1;
				pAR->moveState= pAR->STATE_RUNNING;
			}

	}else
		if(pAR->lengthToEnemy<64)
		{// 按攻击频率攻击
			//if(getRandom(0,100)<40)
				if(pAR->moveState== pAR->STATE_STOP)
				{
					//LOGI("ID_HIT2");
					pAR->act(LeadRole::ID_HIT2);
					pAR->pAFG->change_stay = 1;
					pAR->moveState= pAR->STATE_RUNNING;
				}
		}

	}
	//LOGI("EnemyTeam::updateOneEnemy 3");

	getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//getGL().pushMatrix();
	//pAR->pEP->body->setLinearVelocity(btVector3(0,0,0));
	pAR->update();
	//LOGI("EnemyTeam::updateOneEnemy 4");
}

int EnemyTeam::updateEnemys()
{

	//pAR=(ArmyRole*)enemyRace->vpg.back();
	numOfEnemy=0;
	for(int i=0; i<enemyRace->vpg.size(); i++)
	{
		//if(((ArmyRole*)(enemyRace->vpg[i]))->HP<=0)
		//{
		//	enemyRace->remove(enemyRace->vpg[i]);
		//	continue;
		//}
		//LOGI("EnemyTeam::updateEnemys 1");
		if(enemyRace->vpg[i]->state==2)
		{

			continue;
		}
		numOfEnemy++;
		//LOGI("EnemyTeam::updateEnemys 2");
		updateOneEnemy( (ArmyRole*)(enemyRace->vpg[i]) );
		//LOGI("EnemyTeam::updateEnemys 3");
	}
	//if(numOfEnemy==0) enemyRace->vpg.clear();
	return numOfEnemy;

}
