#pragma once

#include "graphic/Graph.h"

class SkyBox : public Graph{
public:
	float yRot ;

	float matrixTemp[16];

	SkyBox();

	void update();

	void setYRot(float rot){ yRot=rot;}
	float getYRot();
	void setYRotSpeed(float speed);

	virtual void updateAfterDraw();
private:
	float yRotSpeed;
};

