#pragma once
#include "graphic/Graph.h"
#include "movement.h"
#include "Role.h"
#include "ArmyRole.h"
#include "graphic/GraphRace.h"
#include "line.h"

#include <vector>
using std::vector;
#include <deque>
using std::deque;

class LeadRole :public Role
{
public:

	enum{ ID_RUN=1001,ID_RUN2,
		ID_WALK1,ID_WALK2,ID_WALK3,
		ID_HIT1=2001, ID_HIT2, ID_HIT3, ID_HIT4, ID_HIT5
		, ID_HIT6, ID_HIT7, ID_HIT8,
		ID_BEHIT1=3001, ID_BEHIT2, ID_BEHIT3, ID_BEHIT4,
		ID_JUMP1=4001,ID_JUMP_SHORT,
		ID_FALLDOWN1=5001, ID_FALLDOWN2
	};

	//static const int ID_RUN=1001;

	//Graph *pG;

	int command;

	vector<int> vCommands;

	vector<ArmyRole*> *pvEnemy;

	GraphRace *pEnemyR;


	float jumpForwardZ;

	//vector<Movement*> vMovement;

	//movement now
	//Movement* pMoveNow;

	float lengthToEnemy;

	enum{M_WALK, M_RUN, M_HIT1};

	void(*funcDealMovement)(void);

	Line *pLine;

	float lastPos[3];

	int sceneIDNow;

	int sceneIDLast;

	float *pfTemp;

	int coinNum;

	Graph *pG_hitme;

	int sceneID;

	int sceneBelowID;

	int numID;

	int terrainCellNum;

	int xi, yi;

	deque<int> vCommand;

	float fallVel;

	float pfMoveSky[3];

	LeadRole(char * fGraphFile);

	~LeadRole();

	//void act(int code_);

	//void addMove(int code_,char *pfile,int type);

	void onUpdateStart();

	void update();

	void onUpdateEnd();

	virtual void reset();

	//void setChange(void(*func)(void)){ funcDealMovement = func;}

	void dealMovements0(int ID);

	void dealMovements1(int ID);

	void dealMovementRun1();

	void dealMovementJump1();

	void dealMovementJumpShort();

	void dealMKick1();

	void dealFallDown();

	void setMovements();

	//set the hit data
	void hit();


	//set be hitted data
	virtual void behitted(int type, float hp);

	void getEnviromentInfo();

	void nextMove();

	void getCloseToEnemy();

	void turnToEnemy();

	void addCommand(int cmd);

	void clearCommands();

	void dealCommands(int cmd);

	virtual void makeTransform();

	virtual void interactionEnemy(Role*);

	virtual void interactionSameKind(Role*);

	void setEnemyRace(GraphRace *pEnemyR_);

	void CBTouchDown(float x, float z);

	void CBTouchUp(float x, float z);

	void CBTouchMove(float x, float z);

	void setVEnemy(vector<ArmyRole*> *pvEnemy_);

	void setSceneID(int sceneID);

	int getSceneID();

};
