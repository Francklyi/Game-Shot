#include "map.h"
#include "log.h"
#include "PhyScene.h"
//char Map::folder[]="./sdcard/gameData/";

Map::Map()
:sceneNum(0)
,scenePos(0)
{
	vScenes=vector<Scene *>(0);
}

void Map::update()
{
	//LOGI("map update0");
	if(!vScenes[scenePos]->update())
	{
		LOGI("map update1");
		scenePos++;
		vScenes[scenePos]->init();
		LOGI("map update2");
	}
	//LOGI("map update end");
}

void Map::load(float *pf)
{

	char pARstr[]="model1.ly\0";
	ArmyRole *pAR=new ArmyRole(pARstr);

	pAR->setHPAll(100);
	pAR->setHP(100);

	pAR->setAim(pLR);

	pAR->nextDelay=100;
	pAR->timeBetweenDelay=0;

	//pMatrix->translateM(pAR->pTransform->pTOmatrix->mMatrixQueue.back(),0,
	//		30,0,15);

	pAR->setPos(30,0,15);

	//GraphRace *enemyRace=new GraphRace();
	enemyRace->add(pAR);
	enemyRace->setMovements();
	pAR->setVpAFG();
	pAR->act(GraphRace::ID_RUN2);


	pLR->setEnemyRace(enemyRace);

	LOGI("Map::load 1");

	Scene *pScene=new Scene();

	float pfs[]={
			-50, 0.0f, 50,
				50, 0.0f, 50,
				50, 0.0f, -50,
				-50, 0.0f, -50,

				2, //2组

				3, //每组3个
				1,100, 38, 0, 27,
				1,100, 38, 0, 2,
				1,100, 38, 0, -25,

				3, //每组3个
				1,100, 37, 0, 27,
				1,100, 37, 0, 2,
				1,100, 37, 0, -25

	};

	pScene->setPLR(pLR);

	pScene->addEnemyRace(enemyRace);

	LOGI("Map::load 2");
	pScene->loadData(pfs);
	LOGI("Map::load 3");
	vScenes.push_back(pScene);
	LOGI("Map::load finished");
}

void Map::setPLR(LeadRole *pLR_)
{
	pLR=pLR_;
}

void Map::setEnemyRace( GraphRace * enemyRace_)
{
	enemyRace=enemyRace_;
}

Map::~Map()
{

}
