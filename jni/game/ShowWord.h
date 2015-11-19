#pragma once
#include "GLSL/GLSL.h"
#include "Entity.h"

class ShowWord : public Entity
{
public:
	GLuint texID;
//#define EWIDTH 0.05859375;
	//0.0625;

	float pColor[3];

	float interval[3];

	float HPPercentage;

	static float *vertics;

	static unsigned short *index;

	static float *texCoord1;

	float *texCoordTemp;

	static GLuint mTextureId;

	unsigned active_texture_id;

	int showNum;

	static int objectSum;

public:
	ShowWord();

	void setShowNum(int n);

	void draw(int n);

	void draw(int n10,int n);

	void draw(int n100,int n10,int n);
	//void showNum(int *pNum, int n);

	void setColor(float r,float g,float b);

	void drawMeterial();

	void draw();
};
