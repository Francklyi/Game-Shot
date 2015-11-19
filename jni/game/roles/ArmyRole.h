#pragma once
#include "graphic/Graph.h"
#include "movement.h"
#include "Role.h"
#include "PhyCylinder.h"
#include <vector>
using std::vector;

class ArmyRole :public Role
{
public:

	enum{  ID_RUN=1001,ID_RUN2,
		ID_WALK1,ID_WALK2,ID_WALK3,
		ID_HIT1=2001, ID_HIT2, ID_HIT3, ID_HIT4, ID_HIT5
		, ID_HIT6, ID_HIT7, ID_HIT8,
		ID_BEHIT1=3001, ID_BEHIT2, ID_BEHIT3, ID_BEHIT4,
		ID_JUMP1=4001,ID_JUMP2,
		ID_FALLDOWN1, ID_FALLDOWN2,
		ID_GETUP1,ID_GETUP2
	};

	//static const int ID_RUN=1001;

	//Graph *pG;

	//int moveCode;

	//vector<Movement*> vMovement;

	//movement now
	//Movement* pMoveNow;

	static int objectSum;

	int objectID;

	float lengthToEnemy;

	enum{M_WALK, M_RUN, M_HIT1};

	void(*funcDealMovement)(void);

	PhyCylinder *pPhyC;

	vector<ArmyRole*> vAR;

	ArmyRole(char * fGraphFile);

	~ArmyRole();

	//void act(int code_);

	//void addMove(int code_,char *pfile,int type);

	void onUpdateStart();

	void update();

	void onUpdateEnd();

	void setChange(void(*func)(void)){ funcDealMovement = func;}

	void dealMovements0(int ID);

	void dealMovements1(int ID);

	void dealMovementRun1();

	void dealMovementJump1();

	void dealMKick1();


	void setMovements();

	void addMovement(int moveID,char *pfile, int infoLen, int *info);
	//set the hit data
	void hit();


	//set be hitted data
	virtual void behitted(int type, float hp);

	void getEnviromentInfo();

	void nextMove();

	void getCloseToEnemy();

	void turnToEnemy();

	virtual void makeTransform();

	virtual void interactionEnemy(Role*);

	virtual void interactionSameKind(Role*);

	virtual void reset();
};
