#pragma once
#include "AppScene.h"
#include "graphic/Graph.h"
#include "Vector3.h"
#include "Square.h"
#include "OptionGraph.h"
#include "ShowWord.h"
#include <vector>
using std::vector;

class SceneTutorial: public AppScene
{
public:

	float ratio ,left ,right ,bottom ,top ,near ,far, wSize;

	float PMatrix[16];
	float sceneMatrix1[16];

	Graph *pg_background;

	vector<Square*> vSquare;

	vector<GraphData*> vGData;

	Graph* pgBackground;

	vector<OptionGraph*> vgPlanes;

	vector<ShowWord *> vpShowWord;
public:

	SceneTutorial();

	virtual void onCreate();

	virtual void onSize(int w, int h);

	virtual void onUpdate();

	virtual void onTouchDown(float x, float z);

	virtual void onTouchUp(float x, float z);

	virtual void onTouchMove(float x, float z);
};
