#include "scene.h"
//#include "fileDeal/loadFile.h"
#include "log.h"
#include "PhySceneQuad.h"
#include "PhySceneCylinder.h"

Scene::Scene()
:pEnemyGroup(0)
,isInGame(false)
,pG_floor(0)
,pPScene(0)
,num_enemys(0)
,num_coins(0)
,num_thorns(0)
,num_thornballs(0)
,id(0)
,sceneType(STYPE_QUAD)
{
	vEnemyRace=vector<GraphRace*>(0);
	vCoin=vector<Coin*>(0);
	vThorn=vector<Thorn*>(0);
	vEnemy=vector<ArmyRole*>(0);
	vThornball=vector<Thornball*>(0);

	type=TYPE_STATIC;
	oneMaxSteps= 400;
	pf3MoveAll[0]=0;
	pf3MoveAll[1]=0;
	pf3MoveAll[2]=0;
}

bool Scene::loadData()
{

}

void Scene::init()
{
	return;

	pLR->setPhyScene(pPScene);
	//pEnemyGroup->addATeamToGame();
}

void Scene::reset()
{
	pf3MoveAll[0]=0;
	pf3MoveAll[1]=0;
	pf3MoveAll[2]=0;
	/*
	for(int i=0;i<num_coins;i++)
	{
		vCoin[i]->reset();
	}
	for(int i=0;i<num_enemys;i++)
	{
		vEnemy[i]->reset();
	}
	*/
	for(int i=0;i<num_thorns;i++)
	{
		vThorn[i]->reset();
	}
	for(int i=0;i<num_thornballs;i++)
	{
		vThornball[i]->reset();
	}
	if(pG_floor->pMovepath)
	{
		for(int j=0;j<pG_floor->pMovepath->vMPs.size();j++)
		{
			delete pG_floor->pMovepath->vMPs[j];
			pG_floor->pMovepath->vMPs[j]=0;
		}
		pG_floor->pMovepath->vMPs.clear();
		delete pG_floor->pMovepath;
		pG_floor->pMovepath=0;
	}
	sceneType=STYPE_QUAD;
	if(pPScene!=0) delete pPScene;
	pPScene=0;
}

bool Scene::update()
{
	if(pG_floor->pMovepath)
	{
		/*
		if(step>allSteps)
		{
			step=0;
			pfMoveStepNow[0]=-pfMoveStepNow[0];
			pfMoveStepNow[1]=-pfMoveStepNow[1];
			pfMoveStepNow[2]=-pfMoveStepNow[2];
		}
		*/
		getMovesteps();
		updateMove();
		renewPhyScene();
		//step++;
	}else
	{
		//LOGI("update 0");
		pG_floor->draw();
		//LOGI("num_coins: %d",num_coins);
		for(int i=0;i<num_coins;i++)
		{//LOGI("num_coins i: %d",i);
			vCoin[i]->update(pLR);
		}
/*
		for(int i=0;i<num_enemys;i++)
		{
			if(vEnemy[i]->state==2)
				continue;
			updateOneEnemy( vEnemy[i]);
		}
		*/
		for(int i=0;i<num_thorns;i++)
		{
			vThorn[i]->update(pLR);
		}

		for(int i=0;i<num_thornballs;i++)
		{
			vThornball[i]->update(pLR);
		}

	}

	return true;
}

bool Scene::updateLess()
{
	//LOGI("updateL0");
	if(pG_floor->pMovepath)
	{
		getMovesteps();
		updateMoveLess();
		//renewPhyScene();
		//step++;
	}else
	{
		pG_floor->draw();
/*
		for(int i=0;i<num_coins;i++)
		{
			vCoin[i]->updateLess();
		}

		for(int i=0;i<num_enemys;i++)
		{
			if(vEnemy[i]->state==2)
				continue;
			updateOneEnemy( vEnemy[i]);
		}
		*/
		for(int i=0;i<num_thorns;i++)
		{
			//if(vThorn[i])
			vThorn[i]->updateLess();
		}

		for(int i=0;i<num_thornballs;i++)
		{
			vThornball[i]->updateLess();//update(pLR);//
		}
	}
	//LOGI("updateL1");
	return true;
}

void Scene::updateMove()
{
	//pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
	//pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
	//pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
	pG_floor->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
	pG_floor->draw();
/*
	for(int i=0;i<num_coins;i++)
	{
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
		vCoin[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		vCoin[i]->update(pLR);
	}

	for(int i=0;i<num_enemys;i++)
	{
		if(vEnemy[i]->state==2)
			continue;
		//vEnemy[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
		//vEnemy[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
		//vEnemy[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
		vEnemy[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		updateOneEnemy( vEnemy[i]);
	}
	*/
	for(int i=0;i<num_thorns;i++)
	{
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
		vThorn[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		vThorn[i]->update(pLR);
	}

	for(int i=0;i<num_thornballs;i++)
	{
		if(vThornball[i]->followType)
			vThornball[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		vThornball[i]->update(pLR);
	}

	if(pG_floor->pMovepath==0) return;
	if(pPScene==0) return;
	//LOGI("renew 0");
}

void Scene::updateMoveLess()
{
	//pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
	//pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
	//pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
	pG_floor->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
	pG_floor->draw();
/*
	for(int i=0;i<num_coins;i++)
	{
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
		vCoin[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		vCoin[i]->updateLess();
	}

	for(int i=0;i<num_enemys;i++)
	{
		if(vEnemy[i]->state==2)
			continue;
		//vEnemy[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
		//vEnemy[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
		//vEnemy[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
		vEnemy[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		updateOneEnemy( vEnemy[i]);
	}
	*/
	for(int i=0;i<num_thorns;i++)
	{
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveStepNow[0];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveStepNow[1];
		//vCoin[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveStepNow[2];
		vThorn[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		vThorn[i]->updateLess();
	}

	for(int i=0;i<num_thornballs;i++)
	{
		if(vThornball[i]->followType)
			vThornball[i]->move(pfMoveStepNow[0],pfMoveStepNow[1],pfMoveStepNow[2]);
		vThornball[i]->updateLess();
	}

	if(pG_floor->pMovepath==0) return;
	if(pPScene==0) return;
	//LOGI("renew 0");
}

void Scene::setType(int type_)
{
	type=type_;
}

void Scene::setFloor(Graph* pGraph)
{
	pG_floor=pGraph;

	posStart0[0]=pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12];
	posStart0[1]=pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[13];
	posStart0[2]=pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14];

	//posEnd[0]=pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12]+10.0f;
	//posEnd[1]=pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[13]+20.0f;
	//posEnd[2]=pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14];

	//posMove[0]=posStart0[0];
	//posMove[1]=posStart0[1];
	//posMove[2]=posStart0[2];

	//allSteps=200;

	//moveStep[0]=(posEnd[0]-posStart0[0])/allSteps;
	//moveStep[1]=(posEnd[1]-posStart0[1])/allSteps;
	//moveStep[2]=(posEnd[2]-posStart0[2])/allSteps;

	//step=0;

}

void Scene::setPLR(LeadRole *pLR_)
{
	pLR=pLR_;
}

void Scene::setPhyScene(PhyScene *pPScene_)
{
	pPScene=pPScene_;
}

void Scene::addEnemyRace( GraphRace * pER)
{
	vEnemyRace.push_back(pER);
}

void Scene::loadData(float *pdata)
{

	pPScene=new PhyScene();

	//pPScene->setPlane(pdata);

	pEnemyGroup=new EnemyGroup();

	pEnemyGroup->setPLR(pLR);

	pEnemyGroup->setEnemyRace(vEnemyRace[0]);

	pEnemyGroup->loadData(&pdata[3*4]);

	pLR->setPhyScene(pPScene);

	pEnemyGroup->addATeamToGame();

}

void Scene::setfloorPhy(float *pf)
{
	sceneType=STYPE_QUAD;
	if(pPScene!=0)
		delete pPScene;
	pPScene=new PhySceneQuad();
	rangeOrg12[0]=pf[0];
	rangeOrg12[1]=pf[1];
	rangeOrg12[2]=pf[2];
	rangeOrg12[3]=pf[3];
	rangeOrg12[4]=pf[4];
	rangeOrg12[5]=pf[5];
	rangeOrg12[6]=pf[6];
	rangeOrg12[7]=pf[7];
	rangeOrg12[8]=pf[8];
	rangeOrg12[9]=pf[9];
	rangeOrg12[10]=pf[10];
	rangeOrg12[11]=pf[11];
}

void Scene::setCirclePhy(float R, float h)
{
	sceneType=STYPE_CYLINDER;
	if(pPScene==0)
		pPScene=new PhySceneCylinder(R,h);
}

void Scene::initPhy()
{

	if(sceneType==STYPE_QUAD)
	{
		step=0;
		allSteps=oneMaxSteps;
		float *pf4Temp=new float[4];
		float *pTMatrix=pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();
		if(pG_floor->pMovepath)
		{
			rangeNew=new float[(pG_floor->pMovepath->vMPs.size()+1) * 12];
			//pfMoveSteps=new float[pG_floor->pMovepath->vMPs.size() * 3];
			for(int i=0;i<4;i++)
			{
				rangeNew[i*3]=rangeOrg12[i*3]+pTMatrix[12];
				rangeNew[i*3+1]=rangeOrg12[i*3+1]+pTMatrix[13];
				rangeNew[i*3+2]=rangeOrg12[i*3+2]+pTMatrix[14];
				//pf4Temp[3]=1.0f;
				//getMatrix().multiplyMV(rangeNew,i*3
				//		,pG_floor->pTransform->pTOmatrix->mMatrixQueue.back(),0
				//		,pf4Temp,0);
			}
			/*
		for(int j=0;j<pG_floor->pMovepath->vMPs.size();j++)
		{
			if(j==0)
			{
				pfMoveSteps[0]=pG_floor->pMovepath->vMPs[j]->pTransform->pTOmatrix->mMatrixQueue.back()[12]/oneMaxSteps;
				pfMoveSteps[1]=pG_floor->pMovepath->vMPs[j]->pTransform->pTOmatrix->mMatrixQueue.back()[13]/oneMaxSteps;
				pfMoveSteps[2]=pG_floor->pMovepath->vMPs[j]->pTransform->pTOmatrix->mMatrixQueue.back()[14]/oneMaxSteps;
			}else
			{
				pfMoveSteps[j*3]=(pG_floor->pMovepath->vMPs[j]->pTransform->pTOmatrix->mMatrixQueue.back()[12]-
						pG_floor->pMovepath->vMPs[j-1]->pTransform->pTOmatrix->mMatrixQueue.back()[12])/oneMaxSteps;
				pfMoveSteps[j*3+1]=(pG_floor->pMovepath->vMPs[j]->pTransform->pTOmatrix->mMatrixQueue.back()[13]-
						pG_floor->pMovepath->vMPs[j-1]->pTransform->pTOmatrix->mMatrixQueue.back()[13])/oneMaxSteps;
				pfMoveSteps[j*3+2]=(pG_floor->pMovepath->vMPs[j]->pTransform->pTOmatrix->mMatrixQueue.back()[14]-
						pG_floor->pMovepath->vMPs[j-1]->pTransform->pTOmatrix->mMatrixQueue.back()[14])/oneMaxSteps;
			}
		}
		pfMoveStepNow=pfMoveSteps;
			 */
		}
		else
		{
			rangeNew=new float[12];
			for(int i=0;i<4;i++)
			{
				rangeNew[i*3]=rangeOrg12[i*3]+pTMatrix[12];
				rangeNew[i*3+1]=rangeOrg12[i*3+1]+pTMatrix[13];
				rangeNew[i*3+2]=rangeOrg12[i*3+2]+pTMatrix[14];
			}
		}
		((PhySceneQuad*)pPScene)->setPlane(rangeNew);
	}
}

void Scene::getMovesteps()
{
	pfMoveStepNow=pG_floor->pMovepath->getMovestep();//pfMoveSteps
	if(!pfMoveStepNow)
		return;
	pf3MoveAll[0]+=pfMoveStepNow[0];
	pf3MoveAll[1]+=pfMoveStepNow[1];
	pf3MoveAll[2]+=pfMoveStepNow[2];
}

void Scene::renewPhyScene()
{
	pPScene->addNewPos(pf3MoveAll[0],pf3MoveAll[1],pf3MoveAll[2]);
	//LOGI("renew 1");
	/*
	rangeNew[0]+=pf3MoveAll[0];
	rangeNew[1]+=pf3MoveAll[1];
	rangeNew[2]+=pf3MoveAll[2];
	rangeNew[3]+=pf3MoveAll[0];
	rangeNew[4]+=pf3MoveAll[1];
	rangeNew[5]+=pf3MoveAll[2];
	rangeNew[6]+=pf3MoveAll[0];
	rangeNew[7]+=pf3MoveAll[1];
	rangeNew[8]+=pf3MoveAll[2];
	rangeNew[9]+=pf3MoveAll[0];
	rangeNew[10]+=pf3MoveAll[1];
	rangeNew[11]+=pf3MoveAll[2];
	*/
	//pPScene->resetPlane(rangeNew);
	//LOGI("renew 2");
}

void Scene::setfloorPhyNew()
{

}

void Scene::updateOneEnemy(ArmyRole* pAR)
{

	pAR->pAFG->getFrameNum();
	pAR->getEnviromentInfo();

	if(pAR->lengthToEnemy<64||pAR->lengthToEnemy>5500)
	{// 按攻击频率攻击
		if(pAR->moveCode==ArmyRole::ID_RUN2
				&&pAR->pAFG->change_stay!=0
				)
		{
			//LOGI("STATE_STOP");
			pAR->pAFG->change_stay = 0;
			//pLR->pEP->body->setLinearVelocity(btVector3(0,0,0));
			pAR->moveState= pAR->STATE_STOP;
		}
	}
	if(pAR->timeBetweenDelay<pAR->nextDelay)
	{
		pAR->timeBetweenDelay++;
	}
	else
	{
		float *pTemp1=
				pAR->pTransform->pTOmatrix->mMatrixQueue.back();
		//LOGI("pos0: %f,%f, %f",pTemp1[12],pTemp1[13],pTemp1[14]);
		pAR->timeBetweenDelay=0;
		pAR->turnToEnemy();

		//LOGI("pos1: %f,%f, %f",pTemp1[12],pTemp1[13],pTemp1[14]);

	if(pAR->lengthToEnemy<25500&&pAR->lengthToEnemy>64)
	{// 靠近
		//LOGI("updateOneEnemy 2");
		//if(getRandom(0,100)<20)
			if(pAR->moveState== pAR->STATE_STOP)
			{
				//LOGI("ID_RUN2");
				pAR->act(ArmyRole::ID_RUN2);
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
					pAR->act(ArmyRole::ID_HIT2);
					pAR->pAFG->change_stay = 1;
					pAR->moveState= pAR->STATE_RUNNING;
				}
		}

	}
	//getGL().pushMatrix();
	//pAR->pEP->body->setLinearVelocity(btVector3(0,0,0));
	pAR->update();
}

int Scene::inScreenSum=0;
float *Scene::f4=new float[4];

float *Scene::f32=new float[32];

bool Scene::isInScreen()
{
	if(sceneType==STYPE_CYLINDER)
		return true;
	//getMatrix().setIdentityM(f32,16);
	//getMatrix().multiplyMM(f32,16, f32,16,
	//			pG_floor->pTransform->pTOmatrix->mMatrixQueue.back(), 0);

	//getMatrix().multiplyMM(f32,16, f32,16,
	//			pG_floor->pTransform->pROmatrix->mMatrixQueue.back(), 0);

	//getMatrix().multiplyMM(f32,16, f32,16,
	//			pG_floor->pTransform->pSOmatrix->mMatrixQueue.back(), 0);



	//getMatrix().multiplyMM(f32, 0, getGL().VMatrix, 0, f32,16);
	getMatrix().multiplyMM(f32, 0, getGL().PMatrix, 0, getGL().VMatrix, 0 );
	inScreenSum=0;
	for(int i=0;i<4;i++)
	{
		f4[0]=rangeOrg12[i*3];
		f4[1]=rangeOrg12[i*3+1];
		f4[2]=rangeOrg12[i*3+2];
		f4[3]=1;
		getMatrix().multiplyMV(f4, 0,f32,0, f4, 0);
		if(fabs(f4[0]/f4[3])>1 || fabs(f4[1]/f4[3])>1 || f4[3]<0 )
			inScreenSum++;
		else
			return true;
	}

	if(inScreenSum==4) return false;

}

void Scene::toNext()
{

}

void Scene::toPre()
{

}

Scene::~Scene()
{

}
