#pragma once
#include "AppScene.h"
#include "graphic/Graph.h"
#include "Vector3.h"
#include "Square.h"
#include "MyButton.h"
#include "MyButton.h"
#include "ShowWord.h"
#include <vector>
using std::vector;

class SetScene: public AppScene
{
public:

	float ratio ,left ,right ,bottom ,top ,near ,far, wSize;

	float PMatrix[16];
	float sceneMatrix1[16];

	Graph *pg_background;

	vector<Square*> vSquare;

	vector<GraphData*> vGData;

	Graph* pgBackground;

	vector<MyButton*> vMyButton;

	vector<MyButton*> vButtonAll;

	vector<ShowWord *> vpShowWord;

	vector<vector<MyButton*>* > vvButtonAll;

	int state;

	enum{
		STATE_BACKGROUND,
		STATE_GETIN,
		STATE_GETOUT,
		STATE_FIX,
		STATE_TRANSLATE
	};

	int frameAllGetIn;
	int frameGetIn;
	int frameAllGetOut;
	int frameGetOut;

	float *pfVectorInOut1;
	float *pfVectorInOut2;

	Vector3 inOutVec1;
	Vector3 inOutVec2;

	int IdVScene;

	float invX, invY, invZ;

	float viewRadio;

	int screenOffY;
	int screenOffYLast;

	int screenHeight;

	int touchState;

	enum {
		TOUCH_DOWN, TOUCH_MOVE, TOUCH_UP
	};

	int touchDownX,touchDownY;

	int sceneNum;

public:

	SetScene();

	virtual void onCreate();

	virtual void onSize(int w, int h);

	virtual void onUpdate();

	void updateGetIn();

	void updateGetOut();

	void updateFix();

	void updateBackground();

	virtual void onTouchDown(float x, float z);

	virtual void onTouchUp(float x, float z);

	virtual void onTouchMove(float x, float z);

	virtual void back();

	void moveVScene(int id, float x, float y, float z);


};
