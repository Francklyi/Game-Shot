#include "EntityFactory.h"

#include "log.h"

bool init(int numELs_)
{

}

int EntityFactory::registerRole(AEA *aeap)
{
        for(vector<Entitylist *>::iterator iter=ELps.begin()
                        ; iter!= ELps.end();++iter)
        {
                if(aeap->classid==(*iter)->id)
                        {
                        (*iter)->registerRole(aeap);
                        LOGI("roleId:%d",aeap->classid);
                        return ELps.size();
                        }
        }
        Entitylist *ELp=new Entitylist(aeap->classid,aeap->getPath());
        ELp->setOrder(ELps.size());
        ELp->registerRole(aeap);
        ELps.push_back(ELp);
        numELs=ELps.size();
        return ELps.size();
}

Entitylist *EntityFactory::getRoleClass(AEA *aeap)
{
  for(vector<Entitylist *>::iterator iter=ELps.begin()
                    ; iter!= ELps.end();++iter)
    {
       if(aeap->classid==(*iter)->id)
           {
              //LOGI("roleId:%d",role->roleId);
              return (*iter);
           }
    }
  return NULL;
}

int EntityFactory::deleteRole(int id,int order)
{
        return 0;
}

void EntityFactory::loadXml()
{
  for(int i=0;i<ELps.size();++i)
  {
      ELps[i]->loadXml();
  }
}

bool EntityFactory::load()
{
        LOGI("RoleFactory::load ,%d",ELps.size());
        for(int i=0;i<ELps.size();++i)
        {
            ELps[i]->load();
        }
}

bool EntityFactory::update()
{
        //LOGI("RoleFactory::update");
        for(int i=0;i<ELps.size();++i)
        {
            ELps[i]->update();
        }
}

EntityFactory::~EntityFactory()
{
        LOGI("~RoleFactory0");
        for(int i=0;i<ELps.size();++i)
        {
                delete ELps[i];
                ELps[i]=0;
        }
        ELps.clear();
}
