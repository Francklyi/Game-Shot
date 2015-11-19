#pragma once
#include "graphic/Graph.h"
#include "GLSL/material.h"

class MHP : public Graph
{
public:

	float hp;
	float hpAll;

	float *pv;
	int *pvi;

	GLuint ubov;
	GLuint ubovi;


	float* Ka;
	float* Kd;
	float* Ks;
	float Shininess;
	Material *pMaterial;

	static float ThetaBody;


	//float pos[3];

	MHP();
	~MHP();

	void setTheta();

	void setHP(float hp_);
	void setHPAll(float hpa_);

	void setData();
	virtual void draw();


	void setPos(float x, float y, float z);

	virtual void render();

};
