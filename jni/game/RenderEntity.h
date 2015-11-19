#pragma once
#include "Object.h"
#include <GLES2/gl2.h>
#include "graphic/GraphData.h"

class RenderData
{
public:
	int handle;
	GLuint ubo;
	int size;
	GLsizei stride;
	GLvoid* pointer;
};

class RenderEntity : public Object
{
public:
	GraphData *gp;

	//int num_vertex;
	//int num_vi;
	//float *pv;
	//float *pvn;
	//float *pvt;
	//unsigned short *pvi;

	//unsigned int texID;

	//GLuint uboi;
	//int viL;
	float Mmatrix[16];

	float MVMatrix[16];

	float MVPMatrix[16];

	float NormalMatrix[9];

	int change_stay;
	GLuint ubov;
	int sizev;
	GLsizei stridev;
	GLvoid* pointerv;

	//RenderData RD_v;
	//RenderData RD_vn;
	//RenderData RD_vt;

public:
	RenderEntity();

	//void setData(float *pv_,float *pvn_,float *pvt_,unsigned short *pvi_);

	//void setData_v(int handle, GLuint ubo,int size,
	//	    GLsizei stride, GLvoid* pointer);

	//void setData_vn(int handle, GLuint ubo,int size,
	//	    GLsizei stride, GLvoid* pointer);

	//void setData_vt(int handle, GLuint ubo,int size,
	//	    GLsizei stride, GLvoid* pointer);

};
