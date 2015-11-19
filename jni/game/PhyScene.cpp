#include "PhyScene.h"
#include "log.h"
#include <math.h>

PhyScene::PhyScene()
:h(0)
,collisionTime(0)
,planeID(-1)
{

}

void PhyScene::setHeight(float h_)
{
	h=h_;
}

bool PhyScene::isInside(float *pos, float h_)
{
		return false;
}

bool PhyScene::hasTouch(float *nowPos, float *nextPos)
{
	return false;
}

void PhyScene::moveNoPhy(float x, float y, float z)
{

}

void PhyScene::addNewPos(float x, float y, float z)
{

}

void PhyScene::calculate(float *nowPos, float *nextPos, float h1)
{


}
