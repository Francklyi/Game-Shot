#include "BulletSystem.h"

BulletSystem::BulletSystem(){
	vBullets=vector<Bullet0*>(0);
}

void BulletSystem::addBullet(Bullet0* pB){
	vBullets.push_back(pB);
}

void BulletSystem::update(){
	for(int i=0;i<vBullets.size();i++){
		vBullets[i]->update();
	}
}
