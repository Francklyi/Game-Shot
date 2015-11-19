#include "RenderScene.h"

RenderScene::RenderScene()
{
	vpREGroup=vector<RenderEntityGroup*>(0);
}

void RenderScene::addREntity(RenderEntity* pRE)
{
	/*
	//vpREntity
	for(int i=0;i<vpREGroup.size();i++)
	{
		if(pRE->texID==vpREGroup[i]->vpREntity[0]->texID)
		{
			vpREGroup[i]->addREntity(pRE);
			return;
		}
	}
	RenderEntityGroup *pREG=new RenderEntityGroup();
	pREG->addREntity(pRE);
	vpREGroup.push_back(pREG);
	*/
}

void RenderScene::render()
{

}

void RenderScene::organize()
{

}

RenderScene *pRenderScene=new RenderScene();
