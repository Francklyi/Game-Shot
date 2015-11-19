#pragma once
#include "RenderEntity.h"
#include <vector>
using std::vector;
#include "graphic/GraphData.h"
#include "graphic/Graph.h"
#include "Object.h"
#define MAX_OBJS_BATCH 10

class RenderEntityGroup : public Object
{
public:
	GraphData *gp;

	vector<RenderEntity*> vpREntity;

	int num;

	float MVMatrixs[8000];

	float MVPMatrixs[8000];

	float NormalMatrixs[5000];// most 500 object for one type of graph

	float *pvdata;
	unsigned short *pvi;

	float num_re;//number of renderEntitys
	int num_v;
	unsigned short num_vi;

	unsigned int texID;

	float *pfTemp;

	static int drawCallTime;
public:
	RenderEntityGroup();

	void setGraphData(GraphData *gp);

	void initREs(int num);

	void addREntity(Graph *pGraph);

	void clear();

	void organize();

	void renderObjects();

	void render();

	void updateTransform(RenderEntity *pRE);

};
