#include <jni.h>
//#include <btBulletDynamicsCommon.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <pthread.h>

//#include <GLES/gl.h>
//#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <EGL/egl.h>
//#include<unistd.h>
//#include "physic.h"
#include "gameMain.h"
#include "Camera.h"
#include "log.h"

#include "line.h"
#include <vector>
#include "graphic/Graph.h"
#include "graphic/GraphRaceGroup.h"
#include "fileDeal/loadFile.h"
#include <time.h>
#include <math.h>
#include "roles/LeadRole.h"
#include "roles/coin.h"
#include "roles/ArmyRole.h"
#include "MHP.h"
#include "Screen2d.h"
#include "skyBox.h"

//#include <sys/time.h>
#include<unistd.h>

#include "MRandom.h"

#include "PhyScene.h"

#include "game.h"
#include "commond.h"
#include "SysTime.h"

#include "MJNI.h"

using std::vector;

const char *pagPath;
const char *lyPath;
const char *texPath;
const char *layoutPath;
const char *aniPath;

enum{ GAMEMODE_SCORES=0 , GAMEMODE_TIMER};

enum{ GAMEMODE_SET=0 , GAMEMODE_MAIN};

int gameMode=GAMEMODE_SET;

enum{ GAMESCENE0=0 , GAMESCENE1, GAMESCENE2 };

int gameScene=GAMESCENE0;

char pSceneName0[]="fight1.lyout";//"dball_new.lyout";

char pSceneName1[]="dball_new0.lyout";

char pSceneName2[]="dball_new1.lyout";

char *pSceneName=pSceneName0;

bool isRunning=true;

GraphRaceGroup *pGRG; // 图形种族集合

MHP* pMHP;

GraphRace* enemyRace;

Graph gdata1;

Graph *pShadow;

int ballLeft=0;

char *pcData;

time_t T0;
time_t TNow;

LeadRole *pLR=0;

ArmyRole *pAR=0;

Coin *pCoin=0;

SkyBox *pSkyBox=0;
//extern Game gameObject;

int W = 0, H = 0;

//onTouch surfaceView X and Y
float onTouchX;
float onTouchY;

// leadRole move direction
float x = 0, y = 0, z = 0;
// leadRole move direction show
float xp = 0, zp = 0;

//Game *game;

static Vector3 mPFirst(0, 0, 0), mP(0, 0, 0), rPNow(0, 0, 0), rPLast(0, 0, 0);
static Vector3 fingerPointer(0,0,0);

JNIEnv *mEnv;
jobject mJObject;
float t1=0;

//#define M_PI 3.1415926
double TouchX1, TouchY1;
double TouchX2, TouchY2;
double TouchTheta1, TouchTheta2;

int viewport[4];

int layoutReadinVersion=0;

Game *pGame;

SetScene *pSetScene;

AppScene *pAppScene;

//struct timeval{time_t tv_sec; /*秒s*/suseconds_t tv_usec; /*微秒us*/};

struct timeval t_start,t_end;


EGLSurface eglSurface;

EGLDisplay eglDisplay;

int gameState;

int gameStateLast;

void gluProject(float objX, float objY, float objZ,
		float* model, int modelOffset, float* project, int projectOffset,
		int* view, int viewOffset, float* win, int winOffset) {
	float scratch[32];
	int M_OFFSET = 0; // 0..15
	int V_OFFSET = 16; // 16..19
	int V2_OFFSET = 20; // 20..23
	getMatrix().multiplyMM(scratch, M_OFFSET, project, projectOffset,
			model, modelOffset);

	scratch[V_OFFSET + 0] = objX;
	scratch[V_OFFSET + 1] = objY;
	scratch[V_OFFSET + 2] = objZ;
	scratch[V_OFFSET + 3] = 1.0f;

	getMatrix().multiplyMV(scratch, V2_OFFSET,
			scratch, M_OFFSET, scratch, V_OFFSET);

	float w = scratch[V2_OFFSET + 3];
	if (w == 0.0f) {
		return;
	}

	float rw = 1.0f / w;

	win[winOffset] =
			view[viewOffset] + view[viewOffset + 2]
			                        * (scratch[V2_OFFSET + 0] * rw + 1.0f)
			                        * 0.5f;
	win[winOffset + 1] =
			view[viewOffset + 1] + view[viewOffset + 3]
			                            * (scratch[V2_OFFSET + 1] * rw + 1.0f) * 0.5f;
	win[winOffset + 2] = (scratch[V2_OFFSET + 2] * rw + 1.0f) * 0.5f;
}

void gluUnProject(float winX, float winY, float winZ,
		float* model, int modelOffset, float* project, int projectOffset,
		int* view, int viewOffset, float* obj, int objOffset) {
	float scratch[32];
	int PM_OFFSET = 0; // 0..15
	int INVPM_OFFSET = 16; // 16..31
	int V_OFFSET = 0; // 0..3 Reuses PM_OFFSET space
	getMatrix().multiplyMM(scratch, PM_OFFSET, project, projectOffset,
			model, modelOffset);

	if (!getMatrix().invertM(scratch, INVPM_OFFSET, scratch, PM_OFFSET)) {
		return;
	}

	scratch[V_OFFSET + 0] =
			2.0f * (winX - view[viewOffset + 0]) / view[viewOffset + 2]
			                                            - 1.0f;
	scratch[V_OFFSET + 1] =
			2.0f * (winY - view[viewOffset + 1]) / view[viewOffset + 3]
			                                            - 1.0f;
	scratch[V_OFFSET + 2] = 2.0f * winZ - 1.0f;
	scratch[V_OFFSET + 3] = 1.0f;

	getMatrix().multiplyMV(obj, objOffset, scratch, INVPM_OFFSET,
			scratch, V_OFFSET);
}

void screenPto3dP(int x, int y, float *obj4)
{
    glGetIntegerv(GL_VIEWPORT, viewport);
    //LOGI("WH: %d, %d",viewport[2],viewport[3]);

    float modelview[16];
    float projection[16];
	//GLfloat  winX, winY, winZ;
	//GLdouble posX, posY, posZ;

	for(int i=0;i<16;i++) modelview[i]=(float)getGL().VMatrix[i];
	for(int i=0;i<16;i++) projection[i]=(float)getGL().PMatrix[i];

	gluUnProject(x,viewport[3]-y,0.0, modelview,0, projection, 0,viewport,0, obj4, 0);

}

void m3dpToScreenp(float *pf3dp, float *pfscreenp)
{
    glGetIntegerv(GL_VIEWPORT, viewport);
    //LOGI("WH: %d, %d",viewport[2],viewport[3]);

    float modelview[16];
    float projection[16];
	//GLfloat  winX, winY, winZ;
	//GLdouble posX, posY, posZ;

	for(int i=0;i<16;i++) modelview[i]=(float)getGL().VMatrix[i];
	for(int i=0;i<16;i++) projection[i]=(float)getGL().PMatrix[i];

	gluProject(pf3dp[0],pf3dp[1],pf3dp[2], modelview,0, projection, 0,viewport,0,pfscreenp,0 );
	//m_log1f("pfv42 0:  %f",pfv42[0]);m_log1f("pfv42 1:  %f",pfv42[1]);m_log1f("pfv42 2:  %f",pfv42[2]);
}

double getTouchTheta(float x, float y)
{
	//if(x==0.0) LOGI("x: %f",x);//x+=0.01;

	if(y==0.0) {y+=0.02;}//LOGI("y: %f",y);
	else
	if(y==-0.0) {y=0.01;}

	double a=x/(sqrt(x*x + y*y)+0.0001f);
	//if(abs(a)==1.0f)
		//LOGI("a: %f",a);
	//a= abs(a)==1.0f?a+0.01f:a;

	a=asin(a);
	if(-y<0) a=M_PI-a;
	return a*180/M_PI;
}

void setGraphPhysic(Graph* pg_)
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
	getMatrix().setIdentityM(pmatrix,0);

	getMatrix().multiplyMM(pmatrix, 0,
			pg_->pMShape->pTransform->pTOmatrix->mMatrixQueue.back(), 0,
			pg_->pMShape->pTransform->pROmatrix->mMatrixQueue.back(), 0);

	getMatrix().multiplyMM(pmatrix, 0, pg_->pTransform->pTOmatrix->mMatrixQueue.back(), 0, pmatrix, 0);

	pg_->pEP->posOrig=btVector3(pmatrix[12],pmatrix[13],pmatrix[14]);

	pg_->pEP->matrix=pmatrix;

	pg_->pEP->setPhysic();

	pg_->pEP->body->setUserPointer(&(pg_->roleType) );

	if(pg_->roleType==Graph::ROLE_MOVE0)
	{
		//pg_->pEP->remove();
		//ballLeft++;
		//pLR=gdata2;
	}
	*/
}

int onShapeRead(Graph* pg_, char *data, int dataPos)
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

void setLeadRole()
{
	char fileName5[]="model1.ly\0";//"walkManNew1-1.ly\0";//my_scene2_2.ly\0";//"my_scene1_7.ly";
	//gdata1.setFileName(fileName5);//fileName1);"square.ly";//

	//gdata1.setAniFile(fileName5_1);
	//gdata1.setNTexture(NTextureFile);
	//gdata1.load();

	pLR=new LeadRole(fileName5);

	pLR->setMovements();

	//pLR->act(2);
}

void loadOneEnemy(float *info)
{
	char pARstr[]="model1.ly\0";
	pAR=new ArmyRole(pARstr);
	pAR->pTransform->copy(pLR->pTransform);
	pAR->setPos(info[2],info[3],info[4]);

	pAR->setShape();
	pAR->pMShape->copy(pLR->pMShape);
	setGraphPhysic(pAR);

	pAR->setHPAll(info[1]);
	pAR->setHP(info[1]);

	pAR->setAim(pLR);


	pAR->nextDelay=100;
	pAR->timeBetweenDelay=0;



	enemyRace->add(pAR);
	pAR->setVpAFG();
	pAR->act(GraphRace::ID_RUN2);
}

void loadLeadRole(int dataPos, char * pcData)
{
	LOGI("set LeadRole 0");
	//set LeadRole
	int LfileName = 0;
	int hasLR=pcData[dataPos];
	dataPos++;
	if (hasLR==1)//has LeadRole
	{
		LfileName=pcData[dataPos];
		dataPos++;
		char *strFN=(char*)malloc(LfileName+1);
		for (int j=0; j<LfileName; j++)
		{
			strFN[j]=pcData[dataPos];
			dataPos++;
		}
		strFN[LfileName]=0;
		LOGI("Lead: %s",strFN);
		pLR=new LeadRole(strFN);
		char pARstr[]="model1.ly\0";
		pAR=new ArmyRole(pARstr);

		for (int j=0; j<16; j++)
		{
			float f_=*(float*)(pcData+dataPos);
			dataPos+=4;
			pLR->pTransform->pTOmatrix->mMatrixQueue[0][j]=f_;
			pAR->pTransform->pTOmatrix->mMatrixQueue[0][j]=f_;
		}
		//LOGI("a: 8,10: %f, %f",pLR->pTransform->pTOmatrix->mMatrixQueue[0][8],
		//		pLR->pTransform->pTOmatrix->mMatrixQueue[0][10]);
		for (int j=0; j<16; j++)
		{
			float f_=*(float*)(pcData+dataPos);
			dataPos+=4;
			pLR->pTransform->pSOmatrix->mMatrixQueue[0][j]=f_;
			pAR->pTransform->pSOmatrix->mMatrixQueue[0][j]=f_;
		}
		for (int j=0; j<16; j++)
		{
			float f_=*(float*)(pcData+dataPos);
			dataPos+=4;
			pLR->pTransform->pROmatrix->mMatrixQueue[0][j]=f_;
			pAR->pTransform->pROmatrix->mMatrixQueue[0][j]=f_;
		}
		if(layoutReadinVersion>=10)
		{
			// read type of the  role
			pLR->roleType=*(int*)(pcData+dataPos);
			pAR->roleType=pLR->roleType;
			dataPos+=4;

		}

		dataPos=onShapeRead(pLR,pcData,dataPos);
		LOGI("set LeadRole 1.5");
		setGraphPhysic(pLR);
		LOGI("setGraphPhysic done");

		pAR->setShape();
		pAR->pMShape->copy(pLR->pMShape);
		setGraphPhysic(pAR);

		LOGI("set LeadRole 2");
	}

	if(pLR)
	{
		pLR->setMovements();

		pLR->act(pLR->ID_RUN);
	}



	pLR->setHPAll(100);
	pLR->setHP(100);
	pAR->setHPAll(100);
	pAR->setHP(100);

	pLR->addAim(pAR);
	pLR->setAim(pAR);

	float lPosX=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12];
	float lPosY=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13];//pLR->floorHeight;
	float lPosZ=pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14];

	//getGL().setLookAt(camera1->m_Position.x,camera1->m_Position.y, camera1->m_Position.z,
		//lPosX+(camera1->m_Position.x-camera1->m_View.x)*2,
		//20,//lPosY+30,//(camera1->m_Position.y-camera1->m_View.y)*
		//lPosZ+(camera1->m_Position.z-camera1->m_View.z)*2,
	//	lPosX+15, 10, lPosZ, 0,1, 0);

	camera1->setCamera(lPosX+20,
			lPosY+20,
			lPosZ+30,
			lPosX, 10, lPosZ, 0,1, 0);


	pAR->setAim(pLR);

	pAR->nextDelay=100;
	pAR->timeBetweenDelay=0;

	getMatrix().translateM(pAR->pTransform->pTOmatrix->mMatrixQueue.back(),0,
			30,0,15);

	enemyRace=new GraphRace();
	enemyRace->add(pAR);
	enemyRace->setMovements();
	pAR->setVpAFG();
	pAR->act(GraphRace::ID_RUN2);

	int n=3;
	float enemyInfos[3][5]=
	{
			{1,50,  30, 0, 40},
			{1,100,  40, 0, 30},
			{1,150,  30, 0, 20},
	};

	for(int i=0; i<n; i++)
	{
		loadOneEnemy(enemyInfos[i]);
	}

	pLR->setEnemyRace(enemyRace);

	pMHP=new MHP();
	pMHP->setData();
}

PhyPlane *pPlane1;
PhyPlane *pPlane2;

PhyScene *pPScene;

void loadScene()
{
	pPlane1=new PhyPlane();
	pPlane1->setPointers(-172.2,58.2,20,60.3);

	pPlane2=new PhyPlane();
	pPlane2->setPointers(-167.6,63.7,-167.6,-63.3);

	pPScene=new PhyScene();


	float pfs[]={
			13.4, 56.9,
			-166.8, 57.3,
			-166.8, -56.6,
			13.7 , -56.6
	};

	//pPScene->setPlane(pfs);



	pSkyBox=new SkyBox();

	int numofcores=sysconf(_SC_NPROCESSORS_ONLN);
	LOGI("numOfCores: %d",numofcores);

	pGRG = new GraphRaceGroup();

	char filgpath[] = "fight7.lyout";//combineChars(layoutPath, pSceneName);
	LOGI("Graph2:%s", filgpath);
	pcData = getLoadFile().getDataOfLyoutFile(filgpath);//getCharsofFile(filgpath);
	pGRG->clear();
	int dataPos = 0;
	int NumGraphRace = 0;
	int Numentity = 0;
	int entityNum = 0;
	int LfileName = 0;

	if(pcData[0]=='L') // if is new standard
	{
		dataPos++;
		layoutReadinVersion=*(int*)(pcData+dataPos);
		dataPos+=sizeof(int);
	}

	NumGraphRace = pcData[dataPos];
	dataPos++;
	for (int h = 0; h < NumGraphRace; h++)
	{
		Numentity = pcData[dataPos];
		dataPos++;
		LfileName = pcData[dataPos];
		dataPos++;
		char *strFN = (char*) malloc(LfileName + 1);
		for (int j = 0; j < LfileName; j++)
		{
			strFN[j] = pcData[dataPos];
			dataPos++;
		}
		strFN[LfileName] = 0;

		for (int i = 0; i < Numentity; i++)
		{
			Graph *gdata2 = new Graph();
			//MessageBox(_T(chars));
			gdata2->setFileName(strFN);

			//gdata2->pPlane1=pPlane1;
			//gdata2->pPlane2=pPlane2;
			gdata2->pPScene=pPScene;

			LOGI("pGRG->add 1");
			// gdata2->setNTexture(NTextureFile);

			pGRG->add(gdata2);
			LOGI("pGRG->add 2");
			//gdata2->load();
			for (int j = 0; j < 16; j++)
			{
				float f_ = *(float*) (pcData + dataPos);
				dataPos += 4;
				gdata2->pTransform->pTOmatrix->mMatrixQueue[0][j] = f_;
			}
			for (int j = 0; j < 16; j++)
			{
				float f_ = *(float*) (pcData + dataPos);
				dataPos += 4;
				gdata2->pTransform->pSOmatrix->mMatrixQueue[0][j] = f_;
			}
			for (int j = 0; j < 16; j++)
			{
				float f_ = *(float*) (pcData + dataPos);
				dataPos += 4;
				gdata2->pTransform->pROmatrix->mMatrixQueue[0][j] = f_;
			}

			if(layoutReadinVersion>=10)
			{

				// read type of the  role
				gdata2->roleType=*(int*)(pcData+dataPos);
				LOGI("layoutReadinVersion>=10: %d",gdata2->roleType);
				dataPos+=4;
			}

			if(gdata2->roleType==Graph::ROLE_LEAD)
			{
				//pLR=gdata2;
			}
			//else
				// add to the GraphRaceGroup

			LOGI("onShapeRead0");
			dataPos=onShapeRead(gdata2,pcData,dataPos);
			LOGI("onShapeRead1");
			if(gdata2->pMShape) setGraphPhysic(gdata2);
			//if(!(gdata2->pEP)) continue;

			if(gdata2->roleType==Graph::ROLE_FIXED0)
			{
				//gdata2->pEP->body->setFriction(0.5);
				//gdata2->pEP->setRestitution(4.0);
			}else
				if(gdata2->roleType==Graph::ROLE_MOVE0)
				{
					//gdata2->pEP->body->setLinearVelocity(btVector3(80,0,50));
					//gdata2->pEP->setRestitution(2.0);
					//gdata2->pEP->body->setFriction(1.2);
				}
		}
		free(strFN);
		//dataPos+=64;
	}

	float *pfca=(float *)(pcData+dataPos);
	camera1->setCamera(pfca[0],pfca[1],pfca[2], pfca[3],pfca[4],pfca[5], pfca[6],pfca[7],pfca[8]);
	//camera1->setCamera(0.5,50,-100, 0,0,0, 0, 1, 0);

	dataPos+=9*sizeof(float);
	LOGI("setCamera 1");

	loadLeadRole(dataPos, pcData);
}

void *thread1(void *ptr)
{
	LOGI("fthread1 0");
	//pGame=new Game();
	//pGame->load();
	LOGI("fthread1 1");
    return 0;
}

void onSurfaceCreated()
{
	getGL().setPGPNow(getGL().pGLSL_BASE);

	pSetScene=new SetScene();
	pAppScene=pSetScene;

	pAppScene->onCreate();

	pGame=new Game();
	//pGame->load();


	pthread_t a_thread;
	void *thread_result;
	//int res = pthread_create(&a_thread, NULL, thread1, (void *)NULL);
	//res = pthread_join(a_thread, &thread_result);

	return;
/*
	char pcshadow[]="shadow1.ly\0";
	//pShadow=new Graph(pcshadow);
	//pShadow->load();

	pShadow=new Graph();
	pShadow->setFileName(pcshadow);
	pShadow->load();

	//pLR->pEP->remove();

	//pLR->setTerrain(pT);

	pCoin=new Coin();
*/
	//physic->step(1);
	//camera1->m_Position=Vector3(10,20,50);
}

void onSurfaceChanged(int w, int h)
{
  //  glDisable(GL_DITHER);
  W = w;
  H = h;

  LOGE("w,h: %d, %d", w, h);
  //glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);
  //LOGI("w1");
  //glShadeModel(GL_SMOOTH);
  //LOGI("w2");
  glEnable(GL_CULL_FACE);
  glFrontFace(GL_CCW);
  glCullFace(GL_BACK);

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  //glShadeModel(GL_SMOOTH);
  glEnable (GL_DEPTH_TEST);

  glViewport(0, 0, w, h);

  float ratio = (float) w / h;
  float left = -ratio;
  float right = ratio;
  float bottom = -1.0f;
  float top = 1.0f;
  float near = -100.0f;
  float far = 1000.0f;

  float wSize=100.0f;
  getGL().perspective(45.0f, (float) w / (float) h, 1.0f, 5000.0f);
  //getGL().ortho( left*wSize, right*wSize, bottom*wSize, top*wSize, near, far);
  int ww = 30;
  // getGL().ortho(ww*ratio,-ww*ratio,ww,-ww,1,100);
  //  gluLookAt(0, one/2, one+10 ,   0.0, one/2-5, -one,   0.0, 1.0, 0.0);

  //pNum=getSphereData(ps,pn, pi, ball_r);
  //renderToTex();
  pScreen2d->init();

  pRenderManager->initFboHDR(w, h);

  T0=time(NULL);

  gettimeofday(&t_start, NULL);



  pGame->onSize();

	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

  pRenderManager->initFullScreenSquare();

  pAppScene->onSize(w,h);
  //return;

  LOGI("onSurfaceChanged : end");
}

void onTouch(float x, float y)
{

}

void setRunState(bool isRunning_)
{
	isRunning=isRunning_;
}

void updateLeadRole()
{
	getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//getGL().pushMatrix();
	//pLR->pEP->body->setLinearVelocity(btVector3(0,0,0));
	pLR->update();
	pLR->pMoveNow->getAnimation()->getType();

	/*
	if(pLR->moveState== pLR->STATE_RUNNING)
	{
		float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();

		if((fingerPointer.x-pos[12])*(fingerPointer.x-pos[12])
				+(fingerPointer.z-pos[14])*(fingerPointer.z-pos[14])<4)
		{
			pLR->moveState= pLR->STATE_STOP;
			pLR->pAFG->frame ==35;
		}

	}
*/
}

void updateOneEnemy(ArmyRole* pAR)
{

	pAR->getEnviromentInfo();


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


	getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//getGL().pushMatrix();
	//pAR->pEP->body->setLinearVelocity(btVector3(0,0,0));
	pAR->update();
}

void updateEnemys()
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

void touchOne()
{
	setRunState(false);

	//restart();

	jclass jNativesCls;
	JavaVM *g_VM;

	(mEnv)->GetJavaVM(&g_VM);

	if ( !g_VM) {
		return;
	}

	(g_VM)->AttachCurrentThread (&mEnv, NULL); // (void **)

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "touchOne"
			, "()V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj
				, mid);
	}

}

void captureCoin()
{
	jclass jNativesCls;
	JavaVM *g_VM;

	(mEnv)->GetJavaVM(&g_VM);

	if ( !g_VM) {
		return;
	}

	(g_VM)->AttachCurrentThread (&mEnv, NULL); // (void **)

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "captureCoin"
			, "()V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj
				, mid);
	}
}

int coinTime=20;

void updateStaffs()
{
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA , GL_ONE);


	for (int h=0; h<pGRG->vpGR.size();h++)//1
	{
		for(int i=0; i<pGRG->vpGR[h]->vpg.size(); i++)
		{
			//if(pGRG->vpGR[h]->vpg[i]->pEP)
			//if(pGRG->vpGR[h]->vpg[i]->pEP->getIsRemoved())
			{
				//LOGI("!body1");
			//	continue;
			}

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
				//gdata2->pEP->body->setLinearVelocity(btVector3(50,0,40));
				//gdata2->pEP->body->setFriction(1.2);
				getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG
				//btVector3 velocity_=pGRG->vpGR[h]->vpg[i]->pEP->body->getLinearVelocity();
				//velocity_=pGRG->vpGR[h]->vpg[i]->pEP->colVelocity(velocity_);
				//velocity_=btVector3(velocity_.getX(),0,velocity_.getZ());
				//pGRG->vpGR[h]->vpg[i]->pEP->body->setLinearVelocity(velocity_);

				pGRG->vpGR[h]->vpg[i]->updatePhysic();



				pGRG->vpGR[h]->vpg[i]->draw();
			}else //if(pGRG->vpGR[h]->vpg[i]->roleType==Graph::ROLE_FIXED0 ||
					//pGRG->vpGR[h]->vpg[i]->roleType==Graph::ROLE_FIXED1)
			{
				getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);
				//continue;
				getGL().multMatrix(
						pGRG->vpGR[h]->vpg[i]->pTransform->pTOmatrix->mMatrixQueue.back());
				getGL().multMatrix(
						pGRG->vpGR[h]->vpg[i]->pTransform->pROmatrix->mMatrixQueue.back());
				getGL().multMatrix(
						pGRG->vpGR[h]->vpg[i]->pTransform->pSOmatrix->mMatrixQueue.back());
				pGRG->vpGR[h]->vpg[i]->draw();

			}


			//getGL().popMatrix();
		}

	}

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

}

int fps=1;


void drawFrame()
{
	//pSysTime->renewTime();
	//fps++;
	//sleep(0.5);
	//LOGI("drawFrame 0");
	//if(!isRunning) return;
	pAppScene->onUpdate();
	//LOGI("drawFrame 1");
	//pAppScene->onUpdate();

	//pAppScene->onUpdate();
	//TNow=time(NULL);

	//gettimeofday(&t_end, NULL);

	//LOGI("TNow: %ld", (t_end.tv_sec- t_start.tv_sec)*1000000 + t_end.tv_usec- t_start.tv_usec);//*100- t_start.tv_usec

	//t_start.tv_sec=t_end.tv_sec;
	//t_start.tv_usec=t_end.tv_usec;
	//T0=TNow;
	return;
}

JNIEXPORT jint JNICALL
Java_com_ly_game_fight_Native_onSurfaceChanged(JNIEnv* env, jobject thiz, jint w,
    jint h)
{
  onSurfaceChanged(w, h);
  return 1;
}

JNIEXPORT jint JNICALL
Java_com_ly_game_fight_Native_onSurfaceCreated(JNIEnv* env, jobject thiz)
{
	mEnv= env;
	mJObject=thiz;

	initEngine();//!!! init Engine()

	pMjni->init(env);
	onSurfaceCreated();
	return 1;
}

JNIEXPORT jint JNICALL
Java_com_ly_game_fight_Native_update(JNIEnv * env, jobject obj, jint type)
{

	if(type==0) //stop
	{
		pAppScene->isRunning=false;
	}else
	{
		pAppScene->isRunning=true;
	}
		drawFrame();
  return 1;
}

JNIEXPORT jint JNICALL
Java_com_ly_game_fight_Native_exit(JNIEnv * env, jobject obj)
{

	//if(pGRG) free(pGRG);
	//pGRG=0;
	//LOGI("free(pGRG)");
	//if(pLR) free(pLR);
	//pLR=0;
	//LOGI("free(pLR)");
	//if(pT) free(pT);
	//pT=0;
	//LOGI("free(pT)");
	//if (glsl)free(glsl);
	//glsl = 0;
	////LOGI("free(glsl)");
	//exit(0);
	pAppScene->back();
	return 1;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchDown(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y)
{
	pAppScene->onTouchDown(x,y);
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchMove(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y)
{
	pAppScene->onTouchMove(x,y);
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchUp(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y)
{
	pAppScene->onTouchUp(x,y);
}
/*
JNIEXPORT jint JNICALL
Java_com_ly_game_fight_Native_setGravity(JNIEnv * env, jobject obj, jfloat x_,
    jfloat y_, jfloat z_)
{
  x = x_;
  y = y_;
  z = z_;
  //physic->dynamicsWorld->setGravity(btVector3(0,-5,0));
  //physic->dynamicsWorld->setGravity(btVector3(y*5,-z*5,x*5));
}
*/

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_moveCameraFirstP(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y)
{
        mPFirst.x=x;
        mPFirst.z=y;

//        float pfs[4];

        /*
        float objpos[3];
        float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
        objpos[0]=pos[12];
        objpos[1]=pos[13];
        objpos[2]=pos[14];

        m3dpToScreenp(objpos, pfs);

        */
        //LOGI("xy: %f, %f",x,y);
        //LOGI("thums_pos: %f, %f, %f",pfs[0],viewport[3]-pfs[1],pfs[2]);
/*
        screenPto3dP(x, y, pfs);

    	float hRadio=(camera1->m_Position.y)/pfs[1];

    	pfs[0]=(camera1->m_Position.x-pfs[0]*hRadio)/(1-hRadio),
    	pfs[1]=0;
    	pfs[2]=(camera1->m_Position.z-pfs[2]*hRadio)/(1-hRadio);

    	fingerPointer=Vector3(pfs[0], pfs[1], pfs[2]);

    	float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();

    	pLR->pEP->body->activate(true);
    	if(pLR->moveState== pLR->STATE_STOP)
    	{
    		pLR->act(pLR->ID_RUN);

    		pLR->moveState= pLR->STATE_RUNNING;
    	}
    	//
    	TouchTheta1=getTouchTheta(pfs[0]-pos[12],pos[14]- pfs[2]);

    	Vector3 vView=camera1->getView() - camera1->getPosition();
    	float ThetaView=getTouchTheta(vView.x,vView.z);
    	float *matrix1=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
    	float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;


    	float *pf=(float*)malloc(4*sizeof(float));
    	pf[0]=2.0;//rotateTouchTheta2-ThetaView+ThetaBody
    	pf[1]=TouchTheta1+ThetaBody;
    	pf[2]=0;
    	pf[3]=1;
    	pf[4]=0;

    	//LOGI("Theta: %f , %f, %f, %f", TouchTheta1,ThetaView,ThetaBody,pf[1]);

    	pLR->setChange(pf);
    	//getMatrix().rotateM(pLR->pTransform->pTOmatrix->mMatrixQueue.back(),0, TouchTheta2-TouchTheta1,0,1,0);

    	//pLR->pAFG->change_stay = 1;
    	//pLR->pAFG->frame=1;

    	//velocity_ls1=velocity_ls2.cross(velocity_ls1);
    	//btVector3 velocity_ls2 =pLR->pEP->getVelocity();
    	//velocity_ls2 = velocity_ls2.normalize()*0.2;
    			//+ velocity_ls1.normalize()*x*0.1*z

    	//pLR->pEP->body->setLinearVelocity(velocity_ls2);



*/
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_moveCamera(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y)
{
        //sX+=x/1000;
        //sZ+=y/1000;
        //sZ=z;
/*
    float pfs[4];

    screenPto3dP(x, y, pfs);

	float hRadio=(camera1->m_Position.y)/pfs[1];

	pfs[0]=(camera1->m_Position.x-pfs[0]*hRadio)/(1-hRadio);
	pfs[1]=0;
	pfs[2]=(camera1->m_Position.z-pfs[2]*hRadio)/(1-hRadio);

	fingerPointer=Vector3(pfs[0], pfs[1], pfs[2]);

	float * pos=pLR->pTransform->pTOmatrix->mMatrixQueue.back();

	pLR->pEP->body->activate(true);
	if(pLR->moveState== pLR->STATE_STOP)
	{
		pLR->act(pLR->ID_RUN);

		pLR->moveState= pLR->STATE_RUNNING;
	}
	//
	TouchTheta1=getTouchTheta(pfs[0]-pos[12],pos[14]- pfs[2]);

	Vector3 vView=camera1->getView() - camera1->getPosition();
	float ThetaView=getTouchTheta(vView.x,vView.z);
	float *matrix1=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;


	float *pf=(float*)malloc(4*sizeof(float));
	pf[0]=2.0;//rotateTouchTheta2-ThetaView+ThetaBody
	pf[1]=TouchTheta1+ThetaBody;
	pf[2]=0;
	pf[3]=1;
	pf[4]=0;

	//LOGI("Theta: %f , %f, %f, %f", TouchTheta1,ThetaView,ThetaBody,pf[1]);

	pLR->setChange(pf);

	//btVector3 velocity_ls2 =pLR->pEP->getVelocity();
	//velocity_ls2 = velocity_ls2.normalize()*0.2;
			//+ velocity_ls1.normalize()*x*0.1*z

	//pLR->pEP->body->setLinearVelocity(velocity_ls2);

*/
        float xx=(x-mPFirst.x)/W;
        float yy=(y-mPFirst.z)/H;
        Vector3 vec3l = camera1->getPosition();
        Vector3 vec3V = camera1->getView()-vec3l;
        camera1->yawCamera(xx);
        camera1->moveCamera(-yy);

}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_rotateCameraLast(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y){
        rPLast.x=x;
        rPLast.y=y;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_rotateCamera(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y)
{
        rPNow.x=x;
        rPNow.y=y;
        /*
        Vector3 yaw;
        Vector3 cross=camera1->getView() - camera1->getPosition();
        //yaw.x=rPLast.x-rPNow.x;
        //yaw.y=rPNow.y-rPLast.y;
        //yaw.z=0;
        yaw=rPLast-rPNow;
        yaw.z=0;
        yaw=camera1->changeCoord(yaw);
        cross=cross.crossProduct(yaw);
        yaw=cross.normalize();
        yaw=yaw;
        */
        float theta=sqrt((rPNow.x-rPLast.x)*(rPNow.x-rPLast.x)
                        +(rPNow.y-rPLast.y)*(rPNow.y-rPLast.y))/500;
        theta=rPNow.x > rPLast.x ? theta: -theta;
        //LOGI("theta:%f\n", theta);
        //camera1->rotateView(theta, yaw.x, yaw.y, yaw.z);
        camera1->rotatePos(theta, 0, 1, 0);
        rPLast.x=x;
        rPLast.y=y;
        //LOGI("x:%f  %f\n", cross.x,yaw.x);
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setLeadRoleAction(JNIEnv* env,
		jobject thiz, jint code_)
{
	//LOGE("setLeadRoleAction");
	//LOGI("code1:%d", code_);
	if(pGame->pLR)
	{
		pGame->pLR->addCommand(code_);
		//pLR->act(code_);
		//pLR->pAFG->change_stay = 1;
	}
	//gdata1.pAFG->change_stay = 1;
	//game->setLeadRoleAction((int)code_);
	//game->move(camera1->m_View.x, camera1->m_View.y, camera1->m_View.z);
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_move(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z)
{
  LOGE("move");
//game->setLeadRoleAction((int)code_);
xp=0,zp=0;
//game->move(x*0.5, 0, z*0.5);
//xp=x,zp=z;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_addLeadV(JNIEnv* env,
jobject thiz, jfloat x, jfloat y, jfloat z)
{
	/*
    if(pLR)
    {
    	pLR->act(3);
    	pLR->pAFG->change_stay = 1;
    }
    */
  //gdata1.rotate(90, 0, 1, 0);
//game->addLeadV(x, y, z);
//xp=x,zp=z;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_CBTouchDown(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z)
{
	pGame->CBTouchDown( x, z);
	return;

	//pLR->pEP->body->activate(true);
	if( pLR->pAFG->change_stay == 0 && pLR->moveCode!=pLR->ID_WALK1 )
	{
		pLR->pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		pLR->act(pLR->ID_WALK1);

		pLR->moveState= pLR->STATE_RUNNING;
	}

	//return;
	//
	TouchTheta1=getTouchTheta(x,-z);

	Vector3 vView=camera1->getView() - camera1->getPosition();
	float ThetaView=getTouchTheta(vView.x,vView.z);
	float *matrix1=pLR->pTransform->pTOmatrix->mMatrixQueue.back();
	float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;

/*
	float *pf=(float*)malloc(4*sizeof(float));
	pf[0]=2.0;//rotateTouchTheta2-
	pf[1]=TouchTheta1-ThetaView+ThetaBody;
	pf[2]=0;
	pf[3]=1;
	pf[4]=0;

	//LOGI("Theta: %f , %f, %f, %f", TouchTheta1,ThetaView,ThetaBody,pf[1]);

	pLR->setChange(pf);
	*/
	pLR->addRotateY(TouchTheta1-ThetaView+ThetaBody);
	//getMatrix().rotateM(pLR->pTransform->pTOmatrix->mMatrixQueue.back(),0, TouchTheta2-TouchTheta1,0,1,0);

	//pLR->pAFG->change_stay = 1;
	//pLR->pAFG->frame=1;

	//velocity_ls1=velocity_ls2.cross(velocity_ls1);
	//btVector3 velocity_ls2 =pLR->pEP->getVelocity();
	//velocity_ls2 = velocity_ls2.normalize()*0.2;
			//+ velocity_ls1.normalize()*x*0.1*z

	//pLR->pEP->body->setLinearVelocity(velocity_ls2);
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_CBTouchUp(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z)
{
	pGame->CBTouchUp( x, z);
	return;

	//pLR->pEP->body->setLinearVelocity(btVector3(0,0,0));
	pLR->moveState= pLR->STATE_STOP;
	//if(pLR->moveCode==pLR->ID_RUN)
	//{

	//}
		//pLR->pAFG->frame=35;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_CBTouchMove(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z)
{
	pGame->CBTouchMove( x, z);
	return;

	//pLR->pEP->body->activate(true);
	if(pLR->pAFG->change_stay == 0&&pLR->moveCode!=pLR->ID_RUN)
	{

		pLR->act(pLR->ID_RUN);

		pLR->moveState= pLR->STATE_RUNNING;
	}
	pLR->moveState= pLR->STATE_RUNNING;
	//pLR->act(3);
	TouchTheta2=getTouchTheta(x,-z);
	//if(TouchTheta2-TouchTheta1<2) return;
	Vector3 vView=camera1->getView() - camera1->getPosition();
	int ThetaView=getTouchTheta(vView.x,vView.z);
	float *matrix1=pLR->pTransform->pTOmatrix->mMatrixQueue.back();

	int ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;
/*
	float *pf=(float*)malloc(4*sizeof(float));
	pf[0]=2.0;//rotateTouchTheta2--ThetaView
	pf[1]=TouchTheta2+ThetaBody-ThetaView;
	pf[2]=0.0f;
	pf[3]=1.0f;
	pf[4]=0.0f;
*/
	//LOGI("Theta: %f ", pf[1]);
	//LOGI("Theta: %f , %f, %f, %f", matrix1[8],matrix1[10],ThetaBody,pf[1]);
	//if(pf[1]<0.0f) pf[1]+=360.0f;
	//if(abs(pf[1])<1) pf[1]=1;
	//pLR->setChange(pf);

	pLR->addRotateY(TouchTheta2+ThetaBody-ThetaView);
	TouchTheta1=TouchTheta2;

	//btVector3 velocity_ls2 =pLR->pEP->getVelocity();
	//velocity_ls2 = velocity_ls2.normalize()*0.2;
			//+ velocity_ls1.normalize()*x*0.1*z

	//pLR->pEP->body->setLinearVelocity(velocity_ls2);
	//pLR->setChange(pf);
	//
	//pLR->action(TouchTheta2-TouchTheta1);
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().pushMatrix();
	//getGL().rotate(TouchTheta2-TouchTheta1,0,1,0);
	//getMatrix().rotateM(pLR->pTransform->pTOmatrix->mMatrixQueue.back(),0, TouchTheta2-TouchTheta1,0,1,0);

	//pLR->rotate(TouchTheta2-TouchTheta1,0,1,0);
	//pLR->pAFG->change_stay = 1;
	//pLR->pAFG->frame=1;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setDirection(JNIEnv* env,
		jobject thiz, jfloat x, jfloat z)
{

	// LOGE("move");
	double t=x/sqrt(x*x + z*z);
	double a=asin(t);
	if(-z<0) a=M_PI-a;
	a=a*180/M_PI;
	// LOGE("move: %f",a);
	pLR->rotate(a,0,1,0);
	pLR->pAFG->change_stay = 1;
	pLR->pAFG->frame=1;//0
	//Log.d("lx,ly",String.valueOf(a));
	//game->setLeadRoleAction((int)code_);
	xp=x*0.5f,zp=z*0.5f;

	//game->leadRole->setPos(btVector3(LeadRole::pos->x()+xp*0.01f
	//   ,LeadRole::pos->y()
	//   ,LeadRole::pos->z()+zp*0.01f));
}

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_stop(JNIEnv * env, jobject obj)
{
	setRunState(false);
   //game->stop();
}

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_resume(JNIEnv * env, jobject obj)
{
	setRunState(true);
   //game->resume();
}

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_restartGame(JNIEnv * env, jobject obj)
{
	//((Game*)pAppScene)->restart();
	pAppScene->back();

	sleep(1);
	pGame->load(pGame->gameLevelNow-1);

	return 0;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchXY(JNIEnv* env,
jobject thiz, jfloat x, jfloat z)
{
onTouchX = x;
onTouchY = z;

onTouch( x, z);

}


JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setGameMode(JNIEnv* env,
                jobject thiz, jint gameMode_)
{
	gameMode=gameMode_;
	AppScene::gameMode=gameMode_;
	pSetScene->sceneNum=
			gameMode_==SetScene::GAMEMOVE_LEVEL?pSetScene->vButtonAll.size():9;
	pSetScene->state=SetScene::STATE_GETIN;
	pAppScene=pSetScene;

/*
	switch(gameMode_)
	{
	case GAMEMODE_SET:

		break;
	//case GAMEMODE_MAIN:
		//pAppScene=pGame;

	//	break;
	}
*/
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setGameScene(JNIEnv* env,
                jobject thiz, jint gameScene_)
{
	gameScene=gameScene_;
	switch(gameScene)
	{
	case GAMESCENE0:
		pSceneName=pSceneName0;
		break;
	case GAMESCENE1:
		pSceneName=pSceneName1;
		break;
	case GAMESCENE2:
		pSceneName=pSceneName2;
		break;
	}
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_sendPath(JNIEnv* env,
                jobject thiz, jstring string)
{
	pagPath= env->GetStringUTFChars(string, 0);
	char lyfile[]="files/lyfile/";
	getLoadFile().lyPath=getLoadFile().combineChars(pagPath,lyfile);
	char tex[]="files/gamepics/";
	getLoadFile().texPath=getLoadFile().combineChars(pagPath,tex);
	char layout[]="files/layout/";
	getLoadFile().layoutPath=getLoadFile().combineChars(pagPath,layout);
	char ani[]="files/ani/";
	getLoadFile().aniPath=getLoadFile().combineChars(pagPath,ani);
	char ani2[]="files/ani2/";
	getLoadFile().ani2Path=getLoadFile().combineChars(pagPath,ani2);
	//LOGI("chs: %s",pagPath);
	//env->ReleaseStringUTFChars(string,chs);
}

void Java_com_ly_game_fight_Native_readFromAssets
(JNIEnv* env,jclass thiz,jobject assetManager)
{
    // use asset manager to open asset by filename
	getLoadFile().mAssetManager = AAssetManager_fromJava(env,assetManager);
    assert(NULL != getLoadFile().mAssetManager);
}

JNIEXPORT void Java_com_ly_game_fight_Native_setEGL
(JNIEnv* env,jclass thiz,jobject eglSurface_, jobject eglDisplay_)
{
	eglSurface=(EGLSurface)eglSurface_;
	eglDisplay=(EGLDisplay)eglDisplay_;
}

int SysSceneType=SCENETYPE_LEVELS;

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_getSceneType(JNIEnv * env, jobject obj)
{
	if(pAppScene==pSetScene)
		return SCENETYPE_LEVELS;
	else
	if(pAppScene==pGame)
		return SCENETYPE_GAME;

	return SysSceneType;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onButtonDown
(JNIEnv* env,jobject thiz, jint code)
{
	pGame->onButtonDown(code);
	return;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onButtonUp
(JNIEnv* env, jobject thiz, jint code)
{
	pGame->onButtonUp(code);
	return;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setLevelRecord(JNIEnv* env,
                jobject thiz, jint level_)
{
	AppScene::gameLevelRecord=level_;
}

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_next(JNIEnv* env, jobject thiz)
{


	pAppScene->back();


	//pAppScene=pSetScene;
	//pSetScene->state=SetScene::STATE_BACKGROUND;
	//pGame->pLR->reset();
	//pGame->vGDataforScene.clear();

	//pMjni->beforeLayoutLoad(pGame->gameLevelNow);

	sleep(1);

	pGame->load(pGame->gameLevelNow);

	return;

//	pthread_t pth_loadNext;
//	int status = pthread_create(&pth_loadNext, NULL, newThread1, NULL);

//	return;

}
