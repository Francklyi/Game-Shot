#ifndef _GAME_H_
#define _GAME_H_
//include "role.h"

#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include "Camera.h"

#include "line.h"
#include <vector>
#include "graphic/Graph.h"
#include "graphic/GraphRaceGroup.h"
#include "fileDeal/loadFile.h"
#include <time.h>
#include <math.h>
#include "roles/LeadRole.h"
#include "MHP.h"
#include "skyBox.h"
#include "PhyPlane.h"
#include "PhyScene.h"
#include "Shadow.h"
#include "map.h"

#include "GraphShowUp.h"
#include "Scene.h"
#include "LeadHP.h"
#include "AppScene.h"
#include "terrain.h"

#include "ShowWord.h"
#include "showInfo.h"
#include <pthread.h>
#include <semaphore.h>
#include "roles/Crystal.h"
#include "BulletSystem.h"

using std::vector;

class Game : public AppScene
{
public:
	GraphRaceGroup *pGRG; // 图形种族集合

	MHP* pMHP;

	GraphRace* enemyRace;

	Shadow *pShadow;

	char *pcData;

	LeadRole *pLR;

	ArmyRole *pAR;

	SkyBox *pSkyBox;

	PhyScene *pPScene;

	Map *pMap;

	int layoutReadinVersion;

	double TouchTheta1;
	double TouchTheta2;

	int numVScene;

	vector<Scene*> vpScene;

	vector<GraphData*> vGData;

	vector<GraphData*> vGDataforScene;
	//index of function to read data
	vector<int> vFunctionIndex;

	int sceneID;

	Line *pLine;

	LeadHP *pLHP;

	Terrain *pTerrain;

	ShowWord *pShowWord;

	ShowInfo *pShowInfo;

	//Graph *pG_door;

	int state;

	enum {
		STATE_RUNNING, STATE_STOP, STATE_SUCCESS,STATE_FAIL
	};

	pthread_t pth_calculate;

	pthread_t pth_render;

	sem_t semCal;

	sem_t semRen;

	int buttonState;

	enum {
		 BUTTONSTATE_UP=0 ,BUTTONSTATE_DOWN
	};

	int commandCode;

	vector<int> vISceneNum;

	vector<Graph*> vGCrystal;

	vector<Crystal*> vCrystal;

	int numCrystalAll;

	int numCrystalGet;

	bool isAddCrystal;

	BulletSystem *pBS;
	//char *pcSceneDraw;

public:

	Game();

	~Game();

	void loadLayout(int num);

	void loadCommon();

	void loadScene(char *fileName);

	bool load(int layoutID);

	bool update();

	void renderScene();

	void onSize();

	void reset();

	void restart();

	bool store();

	bool stop();

	bool resume();

	double getTouchTheta(float x, float y);

	void setGraphPhysic(Graph* pg_);

	int onShapeRead(Graph* pg_, char *data, int dataPos);

	void setLeadRole();

	void loadOneEnemy(float *info);

	int loadLeadRole(int dataPos, char * pcData);

	//void loadScene();

	void updateLeadRole();

	void updateOneEnemy(ArmyRole* pAR);

	void updateEnemys();

	void updateStaffs();

	void CBTouchDown(float x, float z);

	void CBTouchUp(float x, float z);

	void CBTouchMove(float x, float z);


	int loadScenePhy(int dataPos, char * pcData);

	int loadTerrain(int dataPos, char * pcData);

	int loadVScene(int dataPos, char * pcData);

	int loadTransform(int dataPos, char *pcData, Graph *pg);

	int readOneGraph(int dataPos, char *pcData, Graph *pg);

	int readRoleMove0(Scene& scene, int numGraphs, int dataPos, char *pcData);

	int readRoleMove1(Scene& scene, int numGraphs, int dataPos, char *pcData);

	int readRoleMove2(Scene& scene, int numGraphs, int dataPos, char *pcData);

	int readRoleMove3(Scene& scene, int numGraphs, int dataPos, char *pcData);

	int readRoleMove4(Scene& scene, int numGraphs, int dataPos, char *pcData);

	int readRoleMove5(Scene& scene, int numGraphs, int dataPos, char *pcData);

	int readRoleMove6(Scene& scene, int numGraphs, int dataPos, char *pcData);

	int readRoleMove7(Scene& scene, int numGraphs, int dataPos, char *pcData);

	virtual void onCreate();

	virtual void onSize(int w, int h);

	virtual void onUpdate();

	virtual void onTouchDown(float x, float z);

	virtual void onTouchUp(float x, float z);

	virtual void onTouchMove(float x, float z);

	virtual void back();


	void updateUI();

	void startGame();

	void calculateThread();

	//void *newThread(void *ptr);

	void onButtonDown(int code_);

	void onButtonUp(int code_);

	void nextScene();
};

#endif  //_GAME_H_
