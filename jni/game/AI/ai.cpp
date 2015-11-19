#include "ai.h"
#include <btBulletDynamicsCommon.h>

AI::AI()
{


}

AI::~AI()
{

}

void AI::setMap(Map *map_)
{
  map = map_;
}

void AI::setScene(Scene *scene_)
{
  scene = scene_;
}

void AI::setLeadRole(LeadRole *leadRole_)
{
  leadRole = leadRole_;
}

void AI::addEntitylist(Entitylist *entitylist_)
{
  entitylist = entitylist_;
}

void AI::update()
{
//判断主角和scene.begin 、scene.end 的距离

  //判断主角和其它东西的距离
}
