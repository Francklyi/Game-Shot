#include "Role.h"
#include "log.h"

int Role::roleSum=0;

Role::Role()
:Graph()
{
	roleID=roleSum++;
	vAims=vector<Role*>(0);
	pos[0]=0;
	pos[1]=0;
	pos[2]=0;
}

Role::~Role()
{

}

void Role::addAim(Role *aimRole_)
{
/*
	for(int i=0;i<vAims.size();i++)
	{
		if(vAims[i]->roleID==aimRole_->roleID)
		{
			return;
		}
	}
*/
	vAims.push_back(aimRole);

}

void Role::reset()
{
	Graph::reset();
	HP=HPAll;
}

void Role::setAim(Role *aimRole_)
{
	aimRole=aimRole_;
}

void Role::setHP(float HP_)
{
	HP=HP_;
}

void Role::setHPAll(float HPAll_)
{
	HPAll = HPAll_;
}

void Role::updateAfterTransform()
{

}

void Role::updateAfterDraw()
{

}

void Role::makeTransform()
{
	Graph::makeTransform();
}
