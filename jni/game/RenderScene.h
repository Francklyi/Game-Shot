#pragma once
#include "RenderEntity.h"
#include "RenderEntityGroup.h"
#include <vector>
using std::vector;

class RenderScene
{
public:
	vector<RenderEntityGroup*> vpREGroup;


public:
	RenderScene();

	void addREntity(RenderEntity* pRE);

	void render();

	void organize();

};

extern RenderScene *pRenderScene;
