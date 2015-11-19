#ifndef _ENTITYFACTORY_H_
#define _ENTITYFACTORY_H_
#include "Entitylist.h"
class EntityFactory
{
public:
  EntityFactory():ELps(0),numELs(0){}

  ~EntityFactory();

  bool init(int numELs_);

  void addEntitylist(Entitylist *entitylist_)
  {
    ELps.push_back(entitylist_);
  }

  int registerRole(AEA *aeap);//类的id

  int deleteRole(int id,int order);//order:在id类中的第order个对象

  void initRCPtrs(int n){ ELps=vector<Entitylist *>(n); }

  void loadXml();

  bool load();

  bool update();

  Entitylist *getRoleClass(AEA *aeap);

  vector<Entitylist *> ELps;

  int numELs;

};

#endif //_ENTITYFACTORY_H_
