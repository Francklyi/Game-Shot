#ifndef _AI_H_
#define _AI_H_
#include "map.h"
#include "scene.h"
#include "leadRole.h"
#include "Entitylist.h"

class AI
{
public:
  Entitylist *entitylist;
  Map *map;
  Scene *scene;
  LeadRole *leadRole;

  AI();
  ~AI();

  void setMap(Map *map_);
  void setScene(Scene *scene_);
  void setLeadRole(LeadRole *leadRole_);

  void addEntitylist(Entitylist *entitylist_);

  void update();

};

#endif //_AI_H_
