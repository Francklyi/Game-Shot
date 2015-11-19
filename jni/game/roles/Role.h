#pragma once
#include "graphic/Graph.h"
#include "movement.h"
#include "Attack.h"
#include "MHP.h"
#include "Shadow.h"

#include <vector>
using std::vector;

class Role : public Graph
{
public:

	enum {
		STATE_RUNNING,
		STATE_STANDING,
		STATE_DEAD
	};

	int posState;

	enum{
		POSSTATE_GROUND,
		POSSTATE_SKY
	};

	static int roleSum;

	int roleID;

	//int state;

	int nextDelay;

	int timeBetweenDelay;

	//alarm lenght
	float lengthAlarm;
	// can fight length
	float lengthFight;

	// body length
	float bodyR;

	//Graph *pG;

	Role *aimRole;

	vector<Role *> vAims;

	float HPAll;

	float HP; // hit point

	MHP *pMHP;

	vector<Attack> attack1;

	float pos[3];


	Role();

	~Role();

	void setHPAll(float HPAll_);

	void setHP(float HP_);

	void addAim(Role *aimRole_);

	void removeAim(Role *aimRole_);

	void setAim(Role *aimRole_);


	void getRolePos();


	virtual void makeTransform();

	virtual void behitted(int type, float hp)=0;

	virtual void interactionEnemy(Role*)=0;

	virtual void interactionSameKind(Role*)=0;

	virtual void updateAfterTransform();

	virtual void updateAfterDraw();

	virtual void reset();
};

