#pragma once
#include "graphic/Graph.h"

class MyButton : public Graph
{
public:
	float R;
	int Center2D[2];
	int state;
	enum {STATE_NORMAL, STATE_DOWN, STATE_UP, STATE_UPLESS };
	float frame;
	float frameSum;
	float scale_origen[3];

	int id;

	bool isActive;

public:
	MyButton();

	void update();

	void setCenter();

	bool isTouch(float x, float y);

	void changeState(int state_);

	bool onTouchDown(float x, float y);

	bool onTouchUp(float x, float y);

	void onTouchUpLess();

	void updateNoActive();

	void setActive(bool isActive_);
};
