#ifndef _BULLET_SYSTEM_
#define _BULLET_SYSTEM_
#include "roles/Bullet0.h"
#include <vector>

using std::vector;

class BulletSystem{
public:
	vector<Bullet0*> vBullets;

	BulletSystem();
	void addBullet(Bullet0*);

	void update();

};

#endif //_BULLET_SYSTEM_
