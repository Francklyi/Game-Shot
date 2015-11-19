#include "game.h"
#include "log.h"
#include "Screen2d.h"
#include <pthread.h>
#include "MJNI.h"
#include "commond.h"
#include "fileDeal/loadFile.h"
#include "math/m_math.h"
#include "SysTime.h"
#include "RenderManager.h"
#include "RenderEntityGroup.h"
#include "Graphic/graph.h"
#include "roles/coin.h"
#include "roles/ArmyRole.h"
#include "roles/Thorn.h"
#include "Movepoint.h"
#include "MovepathCircle.h"
#include "PhySceneCylinder.h"

#include <string>
using std::string;

char scenelayout0[] = "fin05.lyout";//"skyNew58.lyout";
char scenelayout1[] = "skyNew58.lyout";//"skyNew7.lyout";//"sky1.lyout";

char scenelayout2[] = "skyNew25.lyout";//"skyNew8.lyout";//"fight39.lyout";
char scenelayout3[] = "skyNew31.lyout";//"skyNew1.lyout";//"fight38.lyout";

char scenelayout4[] = "skyNew30.lyout";//"fightNew12.lyout";//"sky2.lyout";
char scenelayout5[] = "skyNew27.lyout";//"fightNew13.lyout";//"fight38.lyout";

char scenelayouts[][16]={
		"fin01.lyout", "fight1_3.lyout", "fight1_5.lyout",//0 "skyNew33.lyout", "skyNew53.lyout"
		"fin07.lyout", "fin05.lyout", "fin10.lyout",//1
		"fin04.lyout", "fin09.lyout", "fin06.lyout",//2
		"fin12.lyout", "final13.lyout", "final14.lyout",//3
		"final15.lyout", "skyNew58.lyout", "skyNew25.lyout",//4
		"skyNew31.lyout", "skyNew30.lyout", "skyNew27.lyout",//5
		"skyNew58.lyout", "skyNew58.lyout", "skyNew25.lyout",//6
		"skyNew31.lyout", "skyNew30.lyout", "skyNew27.lyout",//7
		"skyNew58.lyout", "skyNew58.lyout", "skyNew25.lyout",//8
		"skyNew31.lyout", "skyNew30.lyout", "skyNew27.lyout",//9
};

char scenelayouts2[][15]={
		"skyNew69.lyout", "skyNew58.lyout", "skyNew25.lyout",//0
		"skyNew31.lyout", "skyNew30.lyout", "skyNew27.lyout",//1
		"skyNew32.lyout", "skyNew36.lyout", "skyNew25.lyout",//2
};
bool isRunning1=false;
int calculateTime=0;

char graphFiles[][20]={
		"cylinder1.ly","crystal.ly","house_1.ly",
		"plane_grass.ly","square.ly",
		"thorn1.ly","thornball.ly","bullet2.ly"
};
int graphSize=8;

void *newThread(void *ptr)
{
	//LOGI("fthread1 0");

	//sem_post(&(pGame->semCal));
	//LOGI("fthread1 1");
	while(isRunning1){
		//sem_wait(&(pGame->semRen) );
		//calculateTime++;
		//LOGI("cal:%d",(pGame->semCal));
		((Game*)ptr)->calculateThread();
	}
    return 0;
}

Game::Game()
:AppScene()
,pGRG(0),pMHP(0),enemyRace(0),pShadow(0)
,pcData(0),pLR(0),pAR(0),pSkyBox(0),pPScene(0),pMap(0)
,layoutReadinVersion(0),TouchTheta1(0),TouchTheta2(0)
,numVScene(STATE_RUNNING)
,sceneID(0),pLine(0),pLHP(0),pTerrain(0),pShowWord(0),pShowInfo(0)
,state(0)
,pth_calculate(),pth_render(),semCal(),semRen()
,buttonState(BUTTONSTATE_UP),commandCode(0),numCrystalAll(0),numCrystalGet(0),isAddCrystal(false)
,pBS(new BulletSystem())
{
	vpScene=vector<Scene*>(200,(Scene*)0);//limit 200 scene
	vpScene.clear();
	//LOGI("%f, %f, %f, %f",vpScene[0],vpScene[1],vpScene[2],vpScene[3]);
	vGData=vector<GraphData*>(0);
	vGDataforScene=vector<GraphData*>(8);
	vGDataforScene.clear();
	vFunctionIndex=vector<int>(8);
	vFunctionIndex.clear();
	//pShadow=new Shadow();
	pLine=new Line();
	pLHP=new LeadHP();
	pTerrain=new Terrain();
	vISceneNum=vector<int>(0);
	vISceneNum.reserve(100);

	vGCrystal=vector<Graph*>(0);
	vGCrystal.reserve(10);

	//pcSceneDraw=0;
}

void Game::onSize()
{
	glGetIntegerv(GL_VIEWPORT, getGL().viewport);
	//LOGI("viewport:%d,%d,%d,%d",getGL().viewport[0],getGL().viewport[1],getGL().viewport[2],getGL().viewport[3]);

	pShowWord=new ShowWord();

	pShowInfo=new ShowInfo();



	//pLHP->init();

	pShowInfo->init();


}

bool Game::load(int layoutID)
{
	if(layoutID>=10)
		gameLevelNow=1;
	else
		gameLevelNow=layoutID+1;
	if(vGData.size()==0) loadCommon();

	loadLayout(gameLevelNow-1);
	pSetScene->frameGetIn=0;
	pSetScene->state=SetScene::STATE_GETOUT;
	LOGI("load finished");
}

void Game::loadLayout(int num)
{
	if(gameMode==GAMEMOVE_LEVEL)
		loadScene(scenelayouts[num]);
	else
		loadScene(scenelayouts2[num]);
	LOGI("load finish");
	/*
	if(false)
	switch(num)
	{
	case 0:
		loadScene(scenelayout0);
		break;
	case 1:
		loadScene(scenelayout1);
		break;
	case 2:
		loadScene(scenelayout2);
		break;
	case 3:
		loadScene(scenelayout3);
		break;
	case 4:
		loadScene(scenelayout4);
		break;
	case 5:
		loadScene(scenelayout5);
		break;

	}
	*/
	getLoadFile().clear();
}

void Game::loadCommon()
{
	//char *pchars=(char*)malloc(sizeof(SkyBox)+10);
	//pSkyBox=new(pchars+4) SkyBox();

	//pSkyBox=new (pSkyBox) SkyBox();
	loadLeadRole(0, 0);
	pLR->gp->setBatchs(1);

	pRenderManager->addREG(pLR->gp);
	//pGRG = new GraphRaceGroup();

	//char filgpath[] = "fight41.lyout";
	//"sky2.lyout";//"fight7.lyout";//combineChars(layoutPath, pSceneName);
	//LOGI("Graph2:%s", filgpath);
	pcData = getLoadFile().getDataOfLyoutFile(scenelayout0);//getCharsofFile(filgpath);
	//pGRG->clear();
	int dataPos = 0;
	int NumGraphRace = 0;
	int Numentity = 0;
	int entityNum = 0;
	int LfileName = 0;

	if(pcData[0]=='L') // if is new standard
	{
		dataPos++;
		layoutReadinVersion=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);
	}

	NumGraphRace = *((int*)(pcData+dataPos));
	dataPos+=sizeof(int);

	vGData=vector<GraphData*>(graphSize);//NumGraphRace

	for (int h = 0; h < graphSize; h++)
	{
		/*
		LfileName = pcData[dataPos];
		dataPos++;
		char *strFN = (char*) malloc(LfileName + 1);
		for (int j = 0; j < LfileName; j++)
		{
			strFN[j] = pcData[dataPos];
			dataPos++;
		}
		strFN[LfileName] = 0;
		for(int i=0;i<6;i++){
			if(!strcmp(strFN,graphFiles[i]))
				LOGI("same:%s",strFN);
		}
		*/
		GraphData *pGD=GraphData::getInstance(graphFiles[h]);
		//new GraphData();
		//pGD->setFileName(graphFiles[h]);//strFN
		//pGD->load();
		if(pGD->jointNump==0)
		{
			LOGI("patchs:30");
			pGD->setBatchs(MAX_OBJS_BATCH);
		}
		else
		{
			LOGI("patchs:1");
			pGD->setBatchs(1);
		}
		//else
		//	pGD->setBatchs(1);

		vGData[h]=pGD;

		pRenderManager->addREG(pGD);

		//free(strFN);
		//dataPos+=64;
	}




	pSkyBox=new SkyBox();
	pSkyBox->gp->setBatchs(1);
	pRenderManager->addREG(pSkyBox->gp);

	pShadow=new Shadow();
	pShadow->gp->setBatchs(1);
	pRenderManager->addREG(pShadow->gp);
	pShadow->setPLR(pLR);
}

void Game::startGame()
{
	LOGI("startGame0");
	isRunning1=true;
	sem_init(&semRen, 0, 0);
	sem_post(&semRen);
	sem_init(&semCal, 0, 0);
	state=STATE_RUNNING;
	//calculateThread();
	LOGI("startGame1");
	pLR->calVelocity();
	pLR->getSceneID();
	LOGI("startGame2");
	getGL().startFrame();
	int status = pthread_create(&pth_calculate, NULL, newThread, this);
	LOGI("startGame3");
	//sem_post(&semRen);
}

void Game::calculateThread(){

	sem_wait(&semRen);
	//sem_post(&semCal);

	pSysTime->renewTime();

	if(isRunning)
	{
		updateLeadRole();
		//LOGE("calculateThread 1");
		float *posTemp0=0;
		if(pLR->sceneID==-1)
			posTemp0=vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();
		else
			posTemp0=vpScene[pLR->sceneID]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();
		//LOGE("calculateThread 2");
		float *posTemp1=0;
		for(int i=0;i<numVScene; i++)
		{
			if(abs(pLR->sceneID-i)>20)
			{
				posTemp1=vpScene[i]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();
				if(abs(posTemp0[12]-posTemp1[12])>1000
						||abs(posTemp0[14]-posTemp1[14])>1000)
					continue;
			}
			//LOGE("calculateThread 3");
			if(i!=pLR->sceneID)
			{
			//	if(vpScene[i]->isInScreen())
					vpScene[i]->updateLess();
			}
			else
				vpScene[i]->update();
			//LOGE("calculateThread 4");
		}

		//LOGE("calculateThread 5");
		//pMap->update();


		//pShadow->update();

		//LOGE("calculateThread 5");
		//pMHP->setTheta();
		//updateEnemys();
		for(int i=0;i<vCrystal.size();i++)
		{
			vCrystal[i]->update(pLR);
		}

		//draw bullet
		pBS->update();
	//	pG_door->draw();

		//LOGE("calculateThread 6");
	}
	else
	{
		//pLR->pAFG->getFrameNum();

		float lPosX=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12];
		float lPosY=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13];//pLR->floorHeight;
		float lPosZ=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14];


		//float dPosX=pG_door->pTransform->pTOmatrix->mMatrixQueue.back()[12];
		//float dPosY=pG_door->pTransform->pTOmatrix->mMatrixQueue.back()[13];//pLR->floorHeight;
		//float dPosZ=pG_door->pTransform->pTOmatrix->mMatrixQueue.back()[14];

		camera1->setLook();
		//updateLeadRole();

		//pMHP->setTheta();

		float *posTemp0=0;
		if(pLR->sceneID==-1)
			posTemp0=vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();
		else
			posTemp0=vpScene[pLR->sceneID]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();

		float *posTemp1=0;

		for(int i=0;i<numVScene; i++)
		{
			if(abs(pLR->sceneID-i)>20)
			{
				posTemp1=vpScene[i]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();
				if(abs(posTemp0[12]-posTemp1[12])>500
						||abs(posTemp0[14]-posTemp1[14])>500)
					continue;
			}
			if(i!=pLR->sceneID)
			{
				//if(vpScene[i]->isInScreen())
					vpScene[i]->updateLess();
			}
			else
				vpScene[i]->updateLess();
		}

	}
	pSkyBox->pTransform->pTOmatrix->mMatrixQueue.back()[12]=camera1->m_View.x;//0;//
	pSkyBox->pTransform->pTOmatrix->mMatrixQueue.back()[13]=-450;//camera1->m_View.y
	pSkyBox->pTransform->pTOmatrix->mMatrixQueue.back()[14]=camera1->m_View.z;//0;//
	pSkyBox->update();


	//gettimeofday(&t_end, NULL);

	//t_start.tv_sec=t_end.tv_sec;
	//t_start.tv_usec=t_end.tv_usec;

	//LOGE("calculateThread end");
	sem_post(&semCal);
}

bool Game::store()
{

}

bool Game::stop()
{
	return true;
}

bool Game::resume()
{
	return true;
}

void Game::nextScene()
{

}

bool Game::update()
{


	//if(layoutReadinVersion>=16)
		if(isAddCrystal)
		{
			numCrystalGet++;
			isAddCrystal=false;
			if(state!=STATE_SUCCESS&&numCrystalGet==numCrystalAll)
			{
				//pthread_kill(pth_calculate, SIGQUIT);
				LOGI("update success");
				state=STATE_SUCCESS;

				if(gameLevelNow==gameLevelRecord)
					gameLevelRecord++;
				if(gameLevelRecord>10)
					gameLevelRecord=10;

				pMjni->success(gameLevelNow);
				return false;
			}
			else
			{
				pMjni->addCrystalNum();
			}
		}


sem_wait(&semCal);


	//calculateThread();

	//int status = pthread_create(&pth_calculate, NULL, newThread, this);
	//void *thread_result;

	//status = pthread_join(pth_calculate, &thread_result);

//LOGI("update 0");
	//getGL().startFrame();
//	LOGI("update 1");
	//glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//getGL().useProgram();
	getGL().setMode(NORMAL);

	//glDisable(GL_DITHER);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear( GL_COLOR_BUFFER_BIT |GL_DEPTH_BUFFER_BIT);//


    getGL().setHasTex(1.0f);

    //glActiveTexture( GL_TEXTURE0);
//gp->pmtls[i]->active_texture_id +
    //glBindTexture(GL_TEXTURE_2D,pLR->gp->pmtls[0]->mTextureId);
    //LOGE("gp->pmtls[i]->mTextureId %d\n", gp->pmtls[i]->mTextureId);
    //int loc = glGetUniformLocation(getGL().getProgramHandle(), "Tex1");
    //if( loc >= 0 )gp->pmtls[i]->active_texture_id
    //glUniform1i(getGL().pGPNow->tex1Handle, 0);

	//pRenderManager->clear();

	//
    //calculateThread();

	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();

	getGL().glslUpdate();

	pShowInfo->setShowNum(6);

	pShowInfo->update();

	pRenderManager->changeFrame();


	pRenderManager->render();




	sem_post(&semRen);


	//




	//glFlush();

	//LOGE("render end");
//	LOGI("update 2");

	return true;

}

void Game::updateUI()
{
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	getGL().glslUpdate();

	pLHP->draw();

	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	getGL().glslUpdate();

	pShowInfo->setShowNum(pLR->coinNum);

	pShowInfo->update();

}

double Game::getTouchTheta(float x, float y)
{
	//if(x==0.0) LOGI("x: %f",x);//x+=0.01;

	if(y==0.0) {y+=0.02;}//LOGI("y: %f",y);
	else
	if(y==-0.0) {y=0.01;}

	double a=x*InvSqrt(x*x + y*y+0.0001f);
	//if(abs(a)==1.0f)
		//LOGI("a: %f",a);
	//a= abs(a)==1.0f?a+0.01f:a;

	a=asin(a);
	if(-y<0) a=M_PI-a;
	return a*180/M_PI;
}

void Game::setGraphPhysic(Graph* pg_)
{
	//*********set EntityPhysic***********
	/*
	pg_->pEP=new EntityPhysic();

	pg_->pEP->setPGraph(pg_);

	pg_->pEP->setShapeType(pg_->pMShape->shapeType);

	pg_->pEP->setMass(pg_->pMShape->mass);

	pg_->pEP->setRestitution(pg_->pMShape->restn);

	pg_->pEP->setShape(pg_->pMShape->shapeType,pg_->pMShape);

	LOGI("Box: %f, %f, %f",
			pg_->pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[0]*0.5,
			pg_->pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[5]*0.5,
			pg_->pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[10]*0.5
			);

	float *pmatrix = (float*)malloc(16*sizeof(float));
	pMatrix->setIdentityM(pmatrix,0);

	pMatrix->multiplyMM(pmatrix, 0,
			pg_->pMShape->pTransform->pTOmatrix->mMatrixQueue.back(), 0,
			pg_->pMShape->pTransform->pROmatrix->mMatrixQueue.back(), 0);

	pMatrix->multiplyMM(pmatrix, 0, pg_->pTransform->pTOmatrix->mMatrixQueue.back(), 0, pmatrix, 0);

	pg_->pEP->posOrig=btVector3(pmatrix[12],pmatrix[13],pmatrix[14]);

	pg_->pEP->matrix=pmatrix;

	pg_->pEP->setPhysic();

	pg_->pEP->body->setUserPointer(&(pg_->roleType) );

	if(pg_->roleType==Graph::ROLE_MOVE0)
	{
		//pg_->pEP->remove();
		//vBalls.push_back(pg_);
		//ballLeft++;
		//pLR=gdata2;
	}
	*/
}

int Game::onShapeRead(Graph* pg_, char *data, int dataPos)
{

	int hasShape=data[dataPos];
	LOGI("hasShape:%d", hasShape);
	dataPos++;
	if(hasShape==0) return dataPos;
	pg_->setShape();
	for (int j=0; j<16; j++)
	{
		float f_=*(float*)(data+dataPos);
		dataPos+=4;
		pg_->pMShape->pTransform->pTOmatrix->mMatrixQueue.back()[j]=f_;
	}
	for (int j=0; j<16; j++)
	{
		float f_=*(float*)(data+dataPos);
		dataPos+=4;
		pg_->pMShape->pTransform->pSOmatrix->mMatrixQueue.back()[j]=f_;
	}
	for (int j=0; j<16; j++)
	{
		float f_=*(float*)(data+dataPos);
		dataPos+=4;
		pg_->pMShape->pTransform->pROmatrix->mMatrixQueue.back()[j]=f_;
	}

	pg_->pMShape->shapeType=*(int*)(data+dataPos);
	dataPos+=sizeof(int);

	pg_->pMShape->mass=*(float*)(data+dataPos);
	dataPos+=sizeof(float);

	pg_->pMShape->restn=*(float*)(data+dataPos);
	dataPos+=sizeof(float);

	return dataPos;
}

void Game::setLeadRole()
{
	char fileName5[]="model1.ly\0";//"walkManNew1-1.ly\0";//my_scene2_2.ly\0";//"my_scene1_7.ly";

	pLR=new LeadRole(fileName5);

	pLR->setMovements();

	//pLR->act(2);
}

void Game::loadOneEnemy(float *info)
{
	char pARstr[]="model1.ly\0";
	pAR=new ArmyRole(pARstr);
	//pAR->pTransform->copy(pLR->pTransform);
	pAR->setPos(info[2],info[3],info[4]);

	//pAR->setShape();
	//pAR->pMShape->copy(pLR->pMShape);
	//setGraphPhysic(pAR);

	pAR->setHPAll(info[1]);
	pAR->setHP(info[1]);

	pAR->setAim(pLR);


	pAR->nextDelay=100;
	pAR->timeBetweenDelay=0;



	enemyRace->add(pAR);
	pAR->setVpAFG();
	pAR->act(GraphRace::ID_RUN2);
}

int Game::loadLeadRole(int dataPos, char * pcData)
{
	LOGI("set LeadRole 0");
	//set LeadRole
	char leadstr[]="leadRole.ly\0";
	pLR=new LeadRole(leadstr);
	pLR->setPos(0,0,0);
	pLR->calculateMatrix();
	pLHP->setPLR(pLR);

	if(pLR)
	{
		pLR->setMovements();

		pLR->act(pLR->ID_RUN);

		pLR->setHPAll(20);
		pLR->setHP(20);


	}


	pMHP=new MHP();
	pMHP->setData();

	return dataPos;

}

void Game::loadScene(char* fileName)
{
	//char filgpath[] = "sky1.lyout";//"fight7.lyout";//combineChars(layoutPath, pSceneName);
	//LOGI("Graph2:%s", filgpath);
	pcData = getLoadFile().getDataOfLyoutFile(fileName);//getCharsofFile(filgpath);
	//pGRG->clear();
	int dataPos = 0;
	int NumGraphRace = 0;
	int Numentity = 0;
	int entityNum = 0;
	int LfileName = 0;

	if(pcData[0]=='L') // if is new standard
	{
		dataPos++;
		layoutReadinVersion=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);
	}

	NumGraphRace = *((int*)(pcData+dataPos));
	dataPos+=sizeof(int);
	//vGData=vector<GraphData*>(NumGraphRace);

	for (int h = 0; h < NumGraphRace; h++)
	{
		vGDataforScene.push_back((GraphData*)0);

		LfileName = pcData[dataPos];
		dataPos++;
		char *strFN = (char*) malloc(LfileName + 1);
		for (int j = 0; j < LfileName; j++)
		{
			strFN[j] = pcData[dataPos];
			dataPos++;
		}
		strFN[LfileName] = 0;
		string strTemp(strFN);
		for(int i=0;i<vGData.size();i++)
		{
			if (vGData[i]->strFile==strTemp)
			{
				vGDataforScene.back()=vGData[i];
				vFunctionIndex.push_back(i);
			}
		}
		//GraphData *pGD=new GraphData();
		//pGD->setFileName(strFN);
		//pGD->load();
		//vGData[h]=pGD;

		free(strFN);
		//dataPos+=64;
	}

	//int numofcores=sysconf(_SC_NPROCESSORS_ONLN);
	//LOGI("numOfCores: %d",numofcores);

	float *pfca=(float *)(pcData+dataPos);

	//camera1=new Camera();
	//camera1->setCamera(pfca[0],pfca[1],pfca[2], pfca[3],pfca[4],pfca[5], pfca[6],pfca[7],pfca[8]);
	//camera1->setCamera(pfca[0],pfca[1],pfca[2], pfca[3],pfca[4],pfca[5], pfca[6],pfca[7],pfca[8]);
	//camera1->setCamera(0.5,50,-100, 0,0,0, 0, 1, 0);

	dataPos+=9*sizeof(float);
	LOGI("setCamera 1");

	//dataPos=loadScenePhy(dataPos, pcData);

	dataPos=loadTerrain(dataPos, pcData);
	LOGI("loadTerrain");
	dataPos=loadVScene(dataPos, pcData);
	LOGI("loadVScene");

	pLR->setPos(vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12],
			vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[13],
			vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14] );


	float lPosX=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12];
	float lPosY=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13];//pLR->floorHeight;
	float lPosZ=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14];

	camera1->setCamera(50,
			30,
			50,
			0, 10, 0, 0,1, 0);
	camera1->extHeight=20;
	camera1->setLook();

	LOGI("loadScene 3");
	if(layoutReadinVersion>=16&&layoutReadinVersion<30)
	{
		numCrystalAll=0;
		numCrystalGet=0;
		vCrystal.clear();
		int numCrystal=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);
		LOGI("load Crystal:%d",numCrystal);
		numCrystalAll=numCrystal;
		pLine->num=numCrystal+1;
		pLine->pfPointers=new float[pLine->num*3];
		pLine->pfPointers[0]=vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12];
		pLine->pfPointers[1]=8.0f;
		pLine->pfPointers[2]=vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14];
		for(int i=0;i<numCrystal;i++)
		{
			int numTemp=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);//GraphDataID
			Crystal *pC=new Crystal();
			pC->gp=vGDataforScene[numTemp];
			//pC->reset();
			dataPos=loadTransform(dataPos, pcData, pC);
			//pMatrix->rotateM(pC->pTransform->pTOmatrix->mMatrixQueue.back(),0,
			//		pC->theta,0,1,0);
			pC->calculateMatrix();
			vCrystal.push_back(pC);
			pLine->pfPointers[i*3+3]
			    =pC->pTransform->pTOmatrix->mMatrixQueue.back()[12];
			pLine->pfPointers[i*3+1+3]
				=pC->pTransform->pTOmatrix->mMatrixQueue.back()[13];
			pLine->pfPointers[i*3+2+3]
				=pC->pTransform->pTOmatrix->mMatrixQueue.back()[14];
		}


	}

	LOGI("loadScene 4");
	/*
	int hasDoor=*((int*)(pcData+dataPos));
	dataPos+=sizeof(int);
	if (hasDoor==1)
	{
	int numTemp=*((int*)(pcData+dataPos));
	dataPos+=sizeof(int);//GraphDataID
	LOGI("door:%d",numTemp);
	if(pG_door==0)
	{
		pG_door=new Graph();
		pG_door->gp=vGDataforScene[numTemp];
	}
	dataPos=loadTransform(dataPos, pcData, pG_door);

	pG_door->calculateMatrix();
	}
	*/
	//pShadow->setGraphRace(enemyRace);
	//pShadow->setPLR(pLR);

	pRenderManager->clear();

	LOGI("loadScene end");

}

int Game::loadScenePhy(int dataPos, char * pcData)
{
	for(int i=0;i<numVScene;i++)
	{
		vpScene[i]->setfloorPhy((float*)(pcData+dataPos) );
		dataPos+=sizeof(float) * 12;
	}
	//LOGI("vpScene: %d",numVScene);
	return dataPos;
}

int Game::loadTerrain(int dataPos, char * pcData)
{
	int LfileName = 0;
	int hasTR=pcData[dataPos];
	dataPos++;
	if (hasTR==1)//has Terrain
	{
		pTerrain->mapWidth=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);

		pTerrain->mapHeight=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);

		pTerrain->mapCellWidth=*(float*)(pcData+dataPos);
		dataPos+=sizeof(float);

		int length=(pTerrain->mapWidth-1)*(pTerrain->mapHeight-1);
		//LOGI("map w,h,cw,len: %d, %d, %f, %d",pTerrain->mapHeight, pTerrain->mapWidth
		//		, pTerrain->mapCellWidth, length);

		//store by char ,so only allow 255 scenes.
		//if(pTerrain->pMapData==0) pTerrain->pMapData=new char[length];
		//if(pTerrain->pIScenes==0)
			//pTerrain->pIScenes=new int[length];
		//LOGI("layoutReadinVersion:%d",layoutReadinVersion);
		if(layoutReadinVersion<25)
		{
			if(pTerrain->vSceneIDs.size()==0)
			{
				pTerrain->vSceneIDs=vector<vector<int>*>(length);


			}

			for(int i=0;i<length;i++)
			{
				pTerrain->vSceneIDs[i]=0;
				//pTerrain->pIScenes[0]=0;
			}
			/*
		else
		{
			for(int i=0;i<length;i++)
			{
				if(pTerrain->vSceneIDs[i])
				pTerrain->vSceneIDs[i]->clear();
			}
		}
			 */
			//memset(pTerrain->pMapData,255,length);

			int sceneNum=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);

			int sceneId=0, idSize=0, j=0, id=0;
			for(int i=0;i<sceneNum; i++)
			{
				sceneId=*((int*)(pcData+dataPos));
				dataPos+=sizeof(int);

				idSize=*((int*)(pcData+dataPos));
				dataPos+=sizeof(int);
				//if(sceneId==0) LOGI("000000000000: %d",idSize);

				for(j=0;j<idSize;j++)
				{
					id=*((int*)(pcData+dataPos));
					dataPos+=sizeof(int);
					if(pTerrain->vSceneIDs[id]==0)
					{
						vector<int> *pvInt=new vector<int>(5);
						pTerrain->vSceneIDs[id]=pvInt;
						pTerrain->vSceneIDs[id]->clear();
					}
					pTerrain->vSceneIDs[id]->push_back(sceneId);
					//pTerrain->pMapData[id]=sceneId;
				}

			}
		}
		else if(layoutReadinVersion>=25)
		{
			if(pTerrain->pIScenes==0)
				pTerrain->pIScenes=new int[length];
			for(int i=0;i<length;i++)
			{
				pTerrain->pIScenes[i]=-1;
			}
			int terCellNum=*(int*)(pcData+dataPos);
			dataPos+=sizeof(int);
			int terCellDataNum=*(int*)(pcData+dataPos);
			dataPos+=sizeof(int);
			//LOGI("t: %d, %d",terCellNum,terCellDataNum);

			if(pTerrain->pISceneIDs==0)
				//free(pTerrain->pISceneIDs);
			pTerrain->pISceneIDs=(char*)malloc(10000);//terCellDataNum
			int num1=0;
			int num2=0;
			int num3=0;
			pTerrain->pIScenes[0]=0;
			for(int i=0;i<terCellNum;i++)
			{
				num3=*(int*)(pcData+dataPos);
				dataPos+=sizeof(int);
				//LOGI("N3 0: %d",num3);
				pTerrain->pIScenes[num3]=num1;
				num2=pcData[dataPos++];
				pTerrain->pISceneIDs[num1++]=num2;
				for(int j=0;j<num2;j++)
				{
					pTerrain->pISceneIDs[num1++]=pcData[dataPos++];
				}


			}
			//dataPos+=terCellNum*sizeof(int)+terCellDataNum;

		}
		pTerrain->mapX0=-pTerrain->mapCellWidth * (pTerrain->mapWidth-1)/2;

		pTerrain->mapY0=-pTerrain->mapCellWidth * (pTerrain->mapHeight-1)/2;

	}
	return dataPos;
}

int Game::loadVScene(int dataPos, char * pcData)
{
	numVScene=*((int*)(pcData+dataPos));
	dataPos+=sizeof(int);
	LOGI("numVScene0:%d",numVScene);
	//vpScene.clear();
	if(vpScene.size()<numVScene)
		vpScene.insert(vpScene.end(),numVScene-vpScene.size(),(Scene*)0);
	//dataPos++;
	int numTemp=0;
	//if(pcSceneDraw) delete[] pcSceneDraw;
	//pcSceneDraw=new char[numVScene];

	for(int i=0; i<numVScene; i++)
	{
		LOGI("numVScenei: %d",i);
		if(vpScene[i]==(Scene*)0)
		{

			vpScene[i]=new Scene();
		}
		else
			vpScene[i]->reset();
		vpScene[i]->id=i;
		vpScene[i]->setPLR(pLR);

		Graph *pGTemp;
		// scene floor
		if(vpScene[i]->pG_floor==0)
			pGTemp=new Graph();
		else
			pGTemp=vpScene[i]->pG_floor;
		pGTemp->reset();

		numTemp=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);
		LOGI("numTemp: %d",numTemp);
		pGTemp->gp=vGDataforScene[numTemp];
		dataPos=loadTransform(dataPos, pcData, pGTemp);


		pGTemp->calculateMatrix();

		vpScene[i]->setFloor(pGTemp);

	LOGI("loadVScene 2");

		if(layoutReadinVersion>=19)
		{
			if(pGTemp->roleType==Graph::ROLE_SCENE0)
			{
				vpScene[i]->setfloorPhy((float*)(pcData+dataPos) );
				dataPos+=sizeof(float) * 12;
			}else if(pGTemp->roleType==Graph::ROLE_SCENE1)
			{
				vpScene[i]->setCirclePhy(pGTemp->pTransform->pSOmatrix->mMatrixQueue.back()[0]*5.0f
						,pGTemp->pTransform->pTOmatrix->mMatrixQueue.back()[13]);
				((PhySceneCylinder*)(vpScene[i]->pPScene))->setCenterPos(
						pGTemp->pTransform->pTOmatrix->mMatrixQueue.back()[12],
						pGTemp->pTransform->pTOmatrix->mMatrixQueue.back()[13],
						pGTemp->pTransform->pTOmatrix->mMatrixQueue.back()[14]);

			}

		}else if(layoutReadinVersion>=10)
		{
			vpScene[i]->setfloorPhy((float*)(pcData+dataPos) );
			dataPos+=sizeof(float) * 12;
		}

		vpScene[i]->initPhy();
		LOGI("loadVScene 3");

		if( layoutReadinVersion>=28 )
		{
			int numGraphRaces=*(int*)(pcData+dataPos);
			dataPos+=sizeof(int);
			LOGI("numGraphRaces %d",numGraphRaces);
			for (int j=0;j<numGraphRaces;j++)
			{
				int numGraphs=*(int*)(pcData+dataPos);
				dataPos+=sizeof(int);
				int roleType_=*(int*)(pcData+dataPos);
				dataPos+=sizeof(int);

				LOGI("numGraphRace %d",j);
				if(numGraphs>0)
				{
					LOGI("loadVScene new 2");
					LOGI("roleType_:%d",roleType_);
					LOGI("ID1,%d",vFunctionIndex[numGraphs]);
					switch(vFunctionIndex[numGraphs]){
					case 0:// crystal
						dataPos=readRoleMove2(*vpScene[i], numGraphs, dataPos, pcData);
						break;
					case 1:
						LOGI("readRoleMove1:%d",roleType_);
						dataPos=readRoleMove3(*vpScene[i], numGraphs, dataPos, pcData);
						break;
					case 2:
						dataPos=readRoleMove3(*vpScene[i], numGraphs, dataPos, pcData);
						break;
					case 3:
						dataPos=readRoleMove2(*vpScene[i], numGraphs, dataPos, pcData);
						break;
					case 4://thorn
						dataPos=readRoleMove3(*vpScene[i], numGraphs, dataPos, pcData);
						break;
					case 5:
						dataPos=readRoleMove2(*vpScene[i], numGraphs, dataPos, pcData);
						break;
					case 6:
						dataPos=readRoleMove3(*vpScene[i], numGraphs, dataPos, pcData);
						break;
					}
					//numTemp=data[dataPos++];//GraphDataID
				}
			}


		}else
		{

			//scene coins
			int numObjects=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);
			//LOGI("vCoin: %d",numObjects);
			vpScene[i]->num_coins=numObjects;
			if(vpScene[i]->vCoin.size()==0)
				vpScene[i]->vCoin=vector<Coin*>(numObjects);
			else if(vpScene[i]->vCoin.size()<numObjects)
				vpScene[i]->vCoin.insert(vpScene[i]->vCoin.end(),numObjects-vpScene[i]->vCoin.size(),(Coin*)0);
			else if(vpScene[i]->vCoin.size()>numObjects)
			{

			}

			if(numObjects>0)
			{

				for(int j=0;j<numObjects;j++)
				{
					numTemp=*((int*)(pcData+dataPos));
					dataPos+=sizeof(int);//GraphDataID
					if(vpScene[i]->vCoin[j]==(Coin*)0)
						vpScene[i]->vCoin[j]=new Coin();
					vpScene[i]->vCoin[j]->gp=vGDataforScene[numTemp];
					vpScene[i]->vCoin[j]->reset();
					dataPos=loadTransform(dataPos, pcData, vpScene[i]->vCoin[j]);
					//pMatrix->rotateM(vpScene[i]->vCoin[j]->pTransform->pTOmatrix->mMatrixQueue.back(),0,
					//		vpScene[i]->vCoin[j]->theta,0,1,0);
					vpScene[i]->vCoin[j]->calculateMatrix();
				}
			}


			//scene enemys
			numObjects=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);
			vpScene[i]->num_enemys=numObjects;
			if(vpScene[i]->vEnemy.size()==0)
				vpScene[i]->vEnemy=vector<ArmyRole*>(numObjects,(ArmyRole*)NULL);
			else if(vpScene[i]->vEnemy.size()<numObjects)
				vpScene[i]->vEnemy.insert(vpScene[i]->vEnemy.end(),numObjects-vpScene[i]->vEnemy.size(),(ArmyRole*)0);
			//LOGI("num_enemys: %d",numObjects);
			//LOGI("vEnemy.size(): %d",vpScene[i]->vEnemy.size());
			ArmyRole* pARTemp=0;
			if(numObjects>0)
			{

				for(int j=0;j<numObjects;j++)
				{
					numTemp=*((int*)(pcData+dataPos));
					dataPos+=sizeof(int);//GraphDataID

					//LOGI("enemy: %d,%s",j,vGDataforScene[numTemp]->fileName);

					if(vpScene[i]->vEnemy[j]==(ArmyRole*)0)
						vpScene[i]->vEnemy[j]=new ArmyRole(vGDataforScene[numTemp]->fileName);
					//LOGI("enemy 1");
					pARTemp=vpScene[i]->vEnemy[j];
					pARTemp->state=1;
					//LOGI("enemy 2");
					pARTemp->gp=vGDataforScene[numTemp];
					dataPos=loadTransform(dataPos, pcData, pARTemp);
					//LOGI("enemy 3");
					pARTemp->calculateMatrix();

					pARTemp->setPhyScene( vpScene[i]->pPScene );
					//LOGI("enemy 4");
					//LOGI("vpScene[i]->pPScene->h: %f" ,vpScene[i]->pPScene->h);
					if(pARTemp->objectID==0) pARTemp->setMovements();

					pARTemp->setHPAll(120);
					pARTemp->setHP(120);

					pARTemp->setAim(pLR);


					pARTemp->nextDelay=100;
					pARTemp->timeBetweenDelay=0;



					//enemyRace->add(pAR);
					pARTemp->setVpAFG();
					pARTemp->act(ArmyRole::ID_RUN2);
					/*
				pARTemp->vAR.clear();
				for(int k=0;k<j;k++)
				{
					pARTemp->vAR.push_back(vpScene[i]->vEnemy[k]);
					vpScene[i]->vEnemy[k]->vAR.push_back(pARTemp);
				}
					 */
				}

				for(int j=0;j<numObjects;j++)
				{
					if(vpScene[i]->vEnemy[j]->vAR.size()!=0)
						vpScene[i]->vEnemy[j]->vAR.clear();
					vpScene[i]->vEnemy[j]->vAR=vector<ArmyRole*>(numObjects-1);
					for(int k=0;k<j;k++)
					{
						vpScene[i]->vEnemy[k]->vAR[j-1]=vpScene[i]->vEnemy[j];
						vpScene[i]->vEnemy[j]->vAR[k]=vpScene[i]->vEnemy[k];
					}
				}

			}

			//scene thorns
			numObjects=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);
			//LOGI("num_thorns0: %d,%d",numObjects,dataPos);
			vpScene[i]->num_thorns=numObjects;
			if(vpScene[i]->vThorn.size()==0)
				vpScene[i]->vThorn=vector<Thorn*>(numObjects);
			else if(vpScene[i]->vThorn.size()<numObjects)
				vpScene[i]->vThorn.insert(vpScene[i]->vThorn.end(),numObjects-vpScene[i]->vThorn.size(),(Thorn*)0);
			else if(vpScene[i]->vThorn.size()>numObjects)
			{

			}
			//LOGI("num_thorns1: %d",numObjects);
			if(numObjects>0)
			{

				for(int j=0;j<numObjects;j++)
				{
					numTemp=*((int*)(pcData+dataPos));
					dataPos+=sizeof(int);//GraphDataID
					if(vpScene[i]->vThorn[j]==(Thorn*)0)
						vpScene[i]->vThorn[j]=new Thorn();
					vpScene[i]->vThorn[j]->gp=vGDataforScene[numTemp];
					vpScene[i]->vThorn[j]->reset();
					dataPos=loadTransform(dataPos, pcData, vpScene[i]->vThorn[j]);
					vpScene[i]->vThorn[j]->calculateMatrix();
					vpScene[i]->vThorn[j]->setRange();
				}
			}

			vpScene[i]->num_thornballs=0;
			if(layoutReadinVersion>=23)
			{
				//scene thornballs
				numObjects=*((int*)(pcData+dataPos));
				dataPos+=sizeof(int);
				//LOGI("num_thornballs0: %d,%d",numObjects,dataPos);
				vpScene[i]->num_thornballs=numObjects;
				if(vpScene[i]->vThornball.size()==0)
					vpScene[i]->vThornball=vector<Thornball*>(numObjects);
				else if(vpScene[i]->vThornball.size()<numObjects)
					vpScene[i]->vThornball.insert(vpScene[i]->vThornball.end(),numObjects-vpScene[i]->vThornball.size(),(Thornball*)0);
				else if(vpScene[i]->vThornball.size()>numObjects)
				{

				}
				//LOGI("num_thornballs1: %d",numObjects);
				if(numObjects>0)
				{
					for(int j=0;j<numObjects;j++)
					{
						numTemp=*((int*)(pcData+dataPos));
						dataPos+=sizeof(int);//GraphDataID
						if(vpScene[i]->vThornball[j]==(Thornball*)0)
							vpScene[i]->vThornball[j]=new Thornball();
						vpScene[i]->vThornball[j]->gp=vGDataforScene[numTemp];
						vpScene[i]->vThornball[j]->reset();
						dataPos=loadTransform(dataPos, pcData, vpScene[i]->vThornball[j]);
						vpScene[i]->vThornball[j]->R=7.0f*vpScene[i]->vThornball[j]->pTransform
								->pSOmatrix->mMatrixQueue.back()[0];
						vpScene[i]->vThornball[j]->calculateMatrix();

					}
				}

			}
		}

		//LOGI("ns2: %d",i);
	}

	return dataPos;
}

int Game::loadTransform(int dataPos, char *pcData, Graph *gdata2)
{
	LOGI("LT:1");
	/*
	for (int j = 0; j < 16; j++)
	{
		float f_ = *(float*) (pcData + dataPos);
		dataPos += 4;
		gdata2->pTransform->pTOmatrix->mMatrixQueue.back()[j] = f_;
	}
	for (int j = 0; j < 16; j++)
	{
		float f_ = *(float*) (pcData + dataPos);
		dataPos += 4;
		gdata2->pTransform->pSOmatrix->mMatrixQueue.back()[j] = f_;
	}
	for (int j = 0; j < 16; j++)
	{
		float f_ = *(float*) (pcData + dataPos);
		dataPos += 4;
		gdata2->pTransform->pROmatrix->mMatrixQueue.back()[j] = f_;
	}
	*/

	memcpy(gdata2->pTransform->pTOmatrix->mMatrixQueue.back(),pcData + dataPos,16*sizeof(float));
	dataPos += 16*sizeof(float);

	memcpy(gdata2->pTransform->pSOmatrix->mMatrixQueue.back(),pcData + dataPos,16*sizeof(float));
	dataPos += 16*sizeof(float);

	memcpy(gdata2->pTransform->pROmatrix->mMatrixQueue.back(),pcData + dataPos,16*sizeof(float));
	dataPos += 16*sizeof(float);

	//LOGI("LT:2");
	if(layoutReadinVersion>=10)
	{
		// read type of the  role
		gdata2->roleType=*((int*)(pcData+dataPos));
		dataPos+=4;
	}

	if(layoutReadinVersion>=24)
	{
		gdata2->followType=*((int*)(pcData+dataPos));
		dataPos+=4;
	}

	//LOGI("LT:3");
	if(layoutReadinVersion>=13)
	{
		//if(gdata2->pMovepath)
		//	gdata2->pMovepath->vMPs.clear();
		// read movepath
		int numMovepoint=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);
		LOGI("numMovepoint: %d",numMovepoint);
		if(numMovepoint>0)
		{
			Movepoint *pMpoint=new Movepoint();

			if(layoutReadinVersion>=21)
			{
				pMpoint->moveType=*((int*)(pcData+dataPos));
				dataPos+=4;
				pMpoint->frameSum=*((int*)(pcData+dataPos));
				dataPos+=4;
				pMpoint->delay=*((int*)(pcData+dataPos));
				dataPos+=4;
				pMpoint->movePathType=*((int*)(pcData+dataPos));
				dataPos+=4;
				pMpoint->direct=*((int*)(pcData+dataPos));
				dataPos+=4;
				pMpoint->numCirclePoints=40;
				if (layoutReadinVersion>=22)
				{
					pMpoint->numCirclePoints=*((int*)(pcData+dataPos));
					dataPos+=sizeof(int);
				}
				if (layoutReadinVersion>=27)
				{
					pMpoint->offsetFrame=*((int*)(pcData+dataPos));
					dataPos+=sizeof(int);
				}
			}

			if(pMpoint->movePathType==Movepoint::PATH_LINE)
			{
				gdata2->pMovepath=new Movepath();

			}else if(pMpoint->movePathType==Movepoint::PATH_CIRCLE)
			{
				gdata2->pMovepath=new MovepathCircle();
				((MovepathCircle*)(gdata2->pMovepath))->NumPoints=pMpoint->numCirclePoints;
			}
			gdata2->pMovepath->isMoving= pMpoint->moveType==0?true:false ;

			for (int k = 0; k < 16; k++)
			{
				float f_ = *(float*) (pcData + dataPos);
				dataPos += 4;
				pMpoint->pTransform->pTOmatrix->mMatrixQueue.back()[k] = f_;
			}
			gdata2->addMovepoint(pMpoint);
			for (int j=1;j<numMovepoint;j++)
			{
				pMpoint=new Movepoint();

				for (int k = 0; k < 16; k++)
				{
					float f_ = *(float*) (pcData + dataPos);
					dataPos += 4;
					pMpoint->pTransform->pTOmatrix->mMatrixQueue.back()[k] = f_;
				}
				gdata2->addMovepoint(pMpoint);
			}

			if(layoutReadinVersion<21)
			{
				gdata2->pMovepath->vMPs[0]->moveType=*((int*)(pcData+dataPos));
				dataPos+=4;
				gdata2->pMovepath->vMPs[0]->frameSum=*((int*)(pcData+dataPos));
				dataPos+=4;
				if(layoutReadinVersion>=14)
				{
					gdata2->pMovepath->vMPs[0]->delay=*((int*)(pcData+dataPos));
					dataPos+=4;
				}

				if(layoutReadinVersion>=18)
				{
					gdata2->pMovepath->vMPs[0]->movePathType=*((int*)(pcData+dataPos));
					dataPos+=4;
					gdata2->pMovepath->vMPs[0]->direct=*((int*)(pcData+dataPos));
					dataPos+=4;
				}

			}

			if(layoutReadinVersion>=26)
			{
				if(gdata2->pMovepath->vMPs[0]->movePathType==Movepoint::PATH_CIRCLE)
				{
					//read circle path
					gdata2->pMovepath->setSpecData((float*)(pcData+dataPos));
					dataPos+=3*(gdata2->pMovepath->vMPs[0]->numCirclePoints)*sizeof(float);
				}
			}else
			if(layoutReadinVersion==21)
			{
				if(gdata2->pMovepath->vMPs[0]->movePathType==Movepoint::PATH_CIRCLE)
				{
					//read circle path
					gdata2->pMovepath->setSpecData((float*)(pcData+dataPos));
					dataPos+=3*39*sizeof(float);
				}
			}
			else
				if(layoutReadinVersion>=22)
				{
					if(gdata2->pMovepath->vMPs[0]->movePathType==Movepoint::PATH_CIRCLE)
					{
						//read circle path
						gdata2->pMovepath->setSpecData((float*)(pcData+dataPos));
						dataPos+=3*(gdata2->pMovepath->vMPs[0]->numCirclePoints-1)*sizeof(float);
					}
				}

			gdata2->initMovepath();
		}


	}
	else
	{
		if(layoutReadinVersion>10)
		{
			//if(gdata2->pMovepath)
			//	gdata2->pMovepath->vMPs.clear();
			// read movepath
			int numMovepoint=pcData[dataPos++];
			//LOGI("numMovepoint: %d",numMovepoint);
			if(numMovepoint>0)
			{
				for (int j=0;j<numMovepoint;j++)
				{
					Movepoint *pMpoint=new Movepoint();

					for (int k = 0; k < 16; k++)
					{
						float f_ = *(float*) (pcData + dataPos);
						dataPos += 4;
						pMpoint->pTransform->pTOmatrix->mMatrixQueue.back()[k] = f_;
					}
					gdata2->addMovepoint(pMpoint);
				}

				gdata2->initMovepath();
			}
		}
		//LOGI("LT:4");
		if(layoutReadinVersion>=12)
		{
			gdata2->moveType=pcData[dataPos++];
		}
	}
	LOGI("LT:5");
	return dataPos;
}

int Game::readOneGraph(int dataPos, char *pcData, Graph *gdata2)
{
	for (int j = 0; j < 16; j++)
	{
		float f_ = *(float*) (pcData + dataPos);
		dataPos += 4;
		gdata2->pTransform->pTOmatrix->mMatrixQueue.back()[j] = f_;
	}
	for (int j = 0; j < 16; j++)
	{
		float f_ = *(float*) (pcData + dataPos);
		dataPos += 4;
		gdata2->pTransform->pSOmatrix->mMatrixQueue.back()[j] = f_;
	}
	for (int j = 0; j < 16; j++)
	{
		float f_ = *(float*) (pcData + dataPos);
		dataPos += 4;
		gdata2->pTransform->pROmatrix->mMatrixQueue.back()[j] = f_;
	}

	if(layoutReadinVersion>=10)
	{

		// read type of the  role
		gdata2->roleType=*((int*)(pcData+dataPos));
		//LOGI("layoutReadinVersion>=10: %d",gdata2->roleType);
		dataPos+=4;
	}



	return dataPos;
}

int Game::readRoleMove0(Scene& scene, int numGraphs, int dataPos, char *pcData){

/*
	for(int k=0; k<numGraphs; k++)
	{
		Graph *pGTemp=new Graph();
		int numTemp=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);//GraphDataID
		pGTemp->gp=vGDataforScene[numTemp];
		dataPos=loadTransform(dataPos, pcData, pGTemp);
	}
*/
	//scene coins
	//int numObjects=*((int*)(pcData+dataPos));
	//dataPos+=sizeof(int);
LOGI("vCoin0: %d",numGraphs);
	scene.num_coins=numGraphs;
	if(scene.vCoin.size()==0)
		scene.vCoin=vector<Coin*>(numGraphs);
	else if(scene.vCoin.size()<numGraphs)
		scene.vCoin.insert(scene.vCoin.end(),numGraphs-scene.vCoin.size(),(Coin*)0);
	else if(scene.vCoin.size()>numGraphs)
	{

	}


	if(numGraphs>0)
	{

		for(int j=0;j<numGraphs;j++)
		{
			LOGI("vCoin1: %d",j);
			int numTemp=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);//GraphDataID
			if(scene.vCoin[j]==(Coin*)0)
				scene.vCoin[j]=new Coin();
			LOGI("numTemp: %d",numTemp);
			scene.vCoin[j]->gp=vGDataforScene[numTemp];
			scene.vCoin[j]->reset();
			dataPos=loadTransform(dataPos, pcData, scene.vCoin[j]);
			//pMatrix->rotateM(vpScene[i]->vCoin[j]->pTransform->pTOmatrix->mMatrixQueue.back(),0,
			//		vpScene[i]->vCoin[j]->theta,0,1,0);
			scene.vCoin[j]->calculateMatrix();
		}
	}
	LOGI("vCoin2: %d",numGraphs);
	return dataPos;
}

int Game::readRoleMove1(Scene& scene, int numObjects, int dataPos, char *pcData){

	//scene enemys
	//int numObjects=*((int*)(pcData+dataPos));
	//LOGI("num_enemys: %d",numObjects);
	//dataPos+=sizeof(int);
	scene.num_enemys=numObjects;
	//LOGI("scene.num_enemys: %d",numObjects);
	if(scene.vEnemy.size()==0)
		scene.vEnemy=vector<ArmyRole*>(numObjects,(ArmyRole*)NULL);
	else if(scene.vEnemy.size()<numObjects)
		scene.vEnemy.insert(scene.vEnemy.end(),numObjects-scene.vEnemy.size(),(ArmyRole*)0);

	LOGI("vEnemy.size(): %d",scene.vEnemy.size());
	ArmyRole* pARTemp=0;
	if(numObjects>0)
	{

		for(int j=0;j<numObjects;j++)
		{
			int numTemp=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);//GraphDataID

			LOGI("enemy: %d,%s",j,vGDataforScene[numTemp]->fileName);

			if(scene.vEnemy[j]==(ArmyRole*)0)
				scene.vEnemy[j]=new ArmyRole(vGDataforScene[numTemp]->fileName);
			LOGI("enemy 1");
			pARTemp=scene.vEnemy[j];
			pARTemp->state=1;
			//LOGI("enemy 2");
			pARTemp->gp=vGDataforScene[numTemp];
			dataPos=loadTransform(dataPos, pcData, pARTemp);
			LOGI("enemy 3");
			pARTemp->calculateMatrix();

			pARTemp->setPhyScene( scene.pPScene );
			LOGI("enemy 4");
			//LOGI("scene.pPScene->h: %f" ,scene.pPScene->h);
			if(pARTemp->objectID==0) pARTemp->setMovements();

			pARTemp->setHPAll(120);
			pARTemp->setHP(120);

			pARTemp->setAim(pLR);


			pARTemp->nextDelay=100;
			pARTemp->timeBetweenDelay=0;



			//enemyRace->add(pAR);
			pARTemp->setVpAFG();
			pARTemp->act(ArmyRole::ID_RUN2);

			LOGI("enemy 5");

			/*
		pARTemp->vAR.clear();
		for(int k=0;k<j;k++)
		{
			pARTemp->vAR.push_back(scene.vEnemy[k]);
			scene.vEnemy[k]->vAR.push_back(pARTemp);
		}
			 */
		}

		for(int j=0;j<numObjects;j++)
		{
			if(scene.vEnemy[j]->vAR.size()!=0)
				scene.vEnemy[j]->vAR.clear();
			scene.vEnemy[j]->vAR=vector<ArmyRole*>(numObjects-1);
			for(int k=0;k<j;k++)
			{
				scene.vEnemy[k]->vAR[j-1]=scene.vEnemy[j];
				scene.vEnemy[j]->vAR[k]=scene.vEnemy[k];
			}
		}

	}

	return dataPos;
}

int Game::readRoleMove2(Scene& scene, int numObjects, int dataPos, char *pcData){
	//scene thorns
	//int numObjects=*((int*)(pcData+dataPos));
	//dataPos+=sizeof(int);
	//LOGI("num_thorns0: %d,%d",numObjects,dataPos);
	scene.num_thorns=numObjects;
	if(scene.vThorn.size()==0)
		scene.vThorn=vector<Thorn*>(numObjects);
	else if(scene.vThorn.size()<numObjects)
		scene.vThorn.insert(scene.vThorn.end(),numObjects-scene.vThorn.size(),(Thorn*)0);
	else if(scene.vThorn.size()>numObjects)
	{

	}
	//LOGI("num_thorns1: %d",numObjects);
	if(numObjects>0)
	{

		for(int j=0;j<numObjects;j++)
		{
			int numTemp=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);//GraphDataID
			if(scene.vThorn[j]==(Thorn*)0)
				scene.vThorn[j]=new Thorn();
			scene.vThorn[j]->gp=vGDataforScene[numTemp];
			scene.vThorn[j]->reset();
			dataPos=loadTransform(dataPos, pcData, scene.vThorn[j]);
			scene.vThorn[j]->calculateMatrix();
			scene.vThorn[j]->setRange();
		}
	}
	return dataPos;
}

int Game::readRoleMove3(Scene& scene, int numObjects, int dataPos, char *pcData){
	//scene.num_thornballs=0;
	//scene thornballs
	//int numObjects=*((int*)(pcData+dataPos));
	//dataPos+=sizeof(int);
	//LOGI("num_thornballs0: %d,%d",numObjects,dataPos);
	scene.num_thornballs=numObjects;
	if(scene.vThornball.size()==0)
		scene.vThornball=vector<Thornball*>(numObjects);
	else if(scene.vThornball.size()<numObjects)
		scene.vThornball.insert(scene.vThornball.end(),numObjects-scene.vThornball.size(),(Thornball*)0);
	else if(scene.vThornball.size()>numObjects)
	{

	}
	LOGI("num_thornballs1: %d",numObjects);
	if(numObjects>0)
	{
		for(int j=0;j<numObjects;j++)
		{
			int numTemp=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);//GraphDataID
			if(scene.vThornball[j]==(Thornball*)0)
				scene.vThornball[j]=new Thornball();
			scene.vThornball[j]->gp=vGDataforScene[numTemp];
			scene.vThornball[j]->reset();
			dataPos=loadTransform(dataPos, pcData, scene.vThornball[j]);
			scene.vThornball[j]->R=7.0f*scene.vThornball[j]->pTransform
					->pSOmatrix->mMatrixQueue.back()[0];
			scene.vThornball[j]->calculateMatrix();

		}
	}

	return dataPos;
}

int Game::readRoleMove4(Scene& scene, int numGraphs, int dataPos, char *pcData){
	LOGI("readRoleMove4 0");
	if(layoutReadinVersion>=16&&layoutReadinVersion<28)
	{
		numCrystalAll=0;
		numCrystalGet=0;
		vCrystal.clear();
		//LOGI("load Crystal");
		int numCrystal=*((int*)(pcData+dataPos));
		dataPos+=sizeof(int);
		numCrystalAll=numCrystal;
		pLine->num=numCrystal+1;
		pLine->pfPointers=new float[pLine->num*3];
		pLine->pfPointers[0]=vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12];
		pLine->pfPointers[1]=8.0f;
		pLine->pfPointers[2]=vpScene[0]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14];
		for(int i=0;i<numCrystal;i++)
		{
			int numTemp=*((int*)(pcData+dataPos));
			dataPos+=sizeof(int);//GraphDataID
			Crystal *pC=new Crystal();
			pC->gp=vGDataforScene[numTemp];
			//pC->reset();
			dataPos=loadTransform(dataPos, pcData, pC);
			//pMatrix->rotateM(pC->pTransform->pTOmatrix->mMatrixQueue.back(),0,
			//		pC->theta,0,1,0);
			pC->calculateMatrix();
			vCrystal.push_back(pC);
			pLine->pfPointers[i*3+3]
			    =pC->pTransform->pTOmatrix->mMatrixQueue.back()[12];
			pLine->pfPointers[i*3+1+3]
				=pC->pTransform->pTOmatrix->mMatrixQueue.back()[13];
			pLine->pfPointers[i*3+2+3]
				=pC->pTransform->pTOmatrix->mMatrixQueue.back()[14];
		}


	}
}

int Game::readRoleMove5(Scene& scene, int numGraphs, int dataPos, char *pcData){

}

int Game::readRoleMove6(Scene& scene, int numGraphs, int dataPos, char *pcData){

}

int Game::readRoleMove7(Scene& scene, int numGraphs, int dataPos, char *pcData){

}

void Game::updateLeadRole()
{
	//LOGI("updateLeadRole0");
	//getGL().pushMatrix();
	//pLR->pEP->body->setLinearVelocity(btVector3(0,0,0));

	//float posX=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12];
	//float posY=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13];
	//float posZ=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14];
	//sceneID=pLR->getSceneID();
//LOGI("updateLeadRole1 sceneID: %d",sceneID);
	//if(sceneID!=-1)
		//vpScene[sceneID]->renewPhyScene();
/*
	int xi=(pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12] - pTerrain->mapX0)/pTerrain->mapCellWidth;
	int yi=(pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14] - pTerrain->mapY0)/pTerrain->mapCellWidth;
	//sceneID=pTerrain->pMapData[yi*(pTerrain->mapWidth-1)+xi];
	if(pTerrain->vSceneIDs[yi*(pTerrain->mapWidth-1)+xi]!=0)
	{
		if(pLR->moveCode!=pLR->ID_JUMP1)
		{
			pLR->sceneIDLast=sceneID;
			pLR->sceneIDNow=sceneID;
		}
		pLR->setVEnemy(&(vpScene[sceneID]->vEnemy));
		//LOGI("scene id: %d",pMapData[yi*(mapWidth-1)+xi]);
		pLR->setPhyScene( vpScene[sceneID]->pPScene );
	}
*/
	//LOGI("updateL eadRole2");
	pLR->update();
	//LOGI("updateLeadRole3");
	//pLR->pMoveNow->getAnimation()->getType();

}

void Game::updateOneEnemy(ArmyRole* pAR)
{

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
		pAR->timeBetweenDelay=0;
		pAR->turnToEnemy();
	if(pAR->lengthToEnemy<5500&&pAR->lengthToEnemy>64)
	{// 靠近

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


	getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//getGL().pushMatrix();
	//pAR->pEP->body->setLinearVelocity(btVector3(0,0,0));
	pAR->update();
}

void Game::updateEnemys()
{

	//pAR=(ArmyRole*)enemyRace->vpg.back();

	for(int i=0; i<enemyRace->vpg.size(); i++)
	{
		//if(((ArmyRole*)(enemyRace->vpg[i]))->HP<=0)
		//{
		//	enemyRace->remove(enemyRace->vpg[i]);
		//	continue;
		//}
		if(enemyRace->vpg[i]->state==2)
			continue;
		updateOneEnemy( (ArmyRole*)(enemyRace->vpg[i]) );
	}

}

void Game::updateStaffs()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA , GL_ONE);

	//LOGI("updateStaffs 0");
	for (int h=0; h<pGRG->vpGR.size();h++)//1
	{
		for(int i=0; i<pGRG->vpGR[h]->vpg.size(); i++)
		{

			if(pGRG->vpGR[h]->vpg[i]->getState()==2) continue;
			getGL().chooseMatrix(GLSL::MMATRIX);
			getGL().setIdentity();
			//getGL().pushMatrix();
			/*
			if(pGRG->vpGR[h]->vpg[i]->roleType==Graph::ROLE_FIXED0)
			{
				getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);
				//continue;
				pGRG->vpGR[h]->vpg[i]->updatePhysic();
				//getGL().multMatrix(
				//		pGRG->vpGR[h]->vpg[i]->pTransform->pSOmatrix->mMatrixQueue.back());
				pGRG->vpGR[h]->vpg[i]->draw();

			}else
				*/
			if(pGRG->vpGR[h]->vpg[i]->roleType==Graph::ROLE_MOVE0)
			{

				getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG

				//pGRG->vpGR[h]->vpg[i]->updatePhysic();

				pGRG->vpGR[h]->vpg[i]->draw();
			}else //if(pGRG->vpGR[h]->vpg[i]->roleType==Graph::ROLE_FIXED0 ||
					//pGRG->vpGR[h]->vpg[i]->roleType==Graph::ROLE_FIXED1)
			{
				getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);

				pGRG->vpGR[h]->vpg[i]->draw();

			}


			//getGL().popMatrix();
		}

	}
/*
	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_ALPHA);

	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	getGL().translate(pos[12],.2,pos[14]);
	getGL().scale(1.2,1,1.2);


	//glBlendEquation(GL_FUNC_SUBTRACT);

	//glEnable(GL_ALPHA_TEST);
	//glAlphaFunc(GL_GREATER, 0.5f);

	pShadow->draw();

	//glDisable(GL_ALPHA_TEST);
	glDisable(GL_BLEND);
*/
}

void Game::reset()
{
	LOGI("reset");
	pLR->reset();
	for(int i=0;i<numVScene; i++)
	{
		vpScene[i]->reset();
	}
}

void Game::restart()
{
	reset();
}

//pthread_mutex_t mutex;

//pthread_mutex_init(&mutex, NULL);

void Game::CBTouchDown(float x, float z)
{
	/*
	//pLR->pEP->body->activate(true);
	if( pLR->pAFG->change_stay == 0 && pLR->moveCode!=pLR->ID_WALK1 )
	{
		pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		pLR->act(pLR->ID_WALK1);

		pLR->moveState= pLR->STATE_RUNNING;
	}
	*/
	pLR->CBTouchDown(x,z);

	TouchTheta1=getTouchTheta(x,-z);

	Vector3 vView=camera1->getView() - camera1->getPosition();
	float ThetaView=getTouchTheta(vView.x,vView.z);
	float *matrix1=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;

	pLR->addRotateY(TouchTheta1-ThetaView+ThetaBody);
}

void Game::CBTouchUp(float x, float z)
{
	pLR->CBTouchUp(x,z);
	//pLR->pEP->body->setLinearVelocity(btVector3(0,0,0));
	//pLR->moveState= pLR->STATE_STOP;
}

void Game::CBTouchMove(float x, float z)
{
	/*
	//pLR->pEP->body->activate(true);
	if(pLR->pAFG->change_stay == 0&&pLR->moveCode!=pLR->ID_RUN)
	{

		pLR->act(pLR->ID_RUN);

		pLR->moveState= pLR->STATE_RUNNING;
	}
	pLR->moveState= pLR->STATE_RUNNING;
	*/
	//return;

	pLR->CBTouchMove(x,z);


	TouchTheta2=getTouchTheta(x,-z);
	//if(TouchTheta2-TouchTheta1<2) return;
	Vector3 vView=camera1->getView() - camera1->getPosition();
	int ThetaView=getTouchTheta(vView.x,vView.z);
	float *matrix1=pLR->pTransform->pTOmatrix->mMatrixQueue.back();

	int ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;

	pLR->addRotateY(TouchTheta2+ThetaBody-ThetaView);

	TouchTheta1=TouchTheta2;
}

void Game::onCreate()
{

}

void Game::onSize(int w, int h)
{

}

void Game::onUpdate()
{
	update();
}

void Game::onTouchDown(float x, float y)
{
	firstTouchLast.x=x;
	firstTouchLast.y=y;
}

void Game::onTouchUp(float x, float y)
{

}

void Game::onTouchMove(float x, float y)
{
	//LOGI("onTouchMove");
	firstTouchNow.x=x;
	firstTouchNow.y=y;

    float len=sqrt((firstTouchNow.x-firstTouchLast.x)*(firstTouchNow.x-firstTouchLast.x)
                    )/500;

    len=firstTouchNow.x > firstTouchLast.x ? len: -len;

    camera1->rotatePos(len, 0, 1, 0);

    len=sqrt((firstTouchNow.y-firstTouchLast.y)*(firstTouchNow.y-firstTouchLast.y))*0.025f;

    len=firstTouchNow.y > firstTouchLast.y ? len: -len;

    camera1->extHeight+=len;


	firstTouchLast.x=x;
	firstTouchLast.y=y;
}

void Game::onButtonDown(int code_)
{
	commandCode=code_;
	buttonState=BUTTONSTATE_DOWN;
	if(pGame->pLR)
	{
		pGame->pLR->addCommand(code_);
		//pLR->act(code_);
		//pLR->pAFG->change_stay = 1;
	}
}

void Game::onButtonUp(int code_)
{
	buttonState=BUTTONSTATE_UP;
}

void Game::back()
{
	isRunning1=false;
	pLR->reset();
	vGDataforScene.clear();
	vFunctionIndex.clear();
	//pMjni->setControlVisable(0);
	pSetScene->state=SetScene::STATE_GETIN;
	pAppScene=pSetScene;

}

Game::~Game()
{

}
