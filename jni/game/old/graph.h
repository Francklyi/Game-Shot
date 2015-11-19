#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <math.h>
#include <GLES/gl.h>
#include "log.h"

#define one 1.0f
typedef unsigned char byte;

//CUBE
static GLfloat vertices[24] = {
		-one, -one, -one,
		one, -one, -one,
		one,  one, -one,
		-one,  one, -one,
		-one, -one,  one,
		one, -one,  one,
		one,  one,  one,
		-one,  one,  one,
};


static GLfloat colors[] = {
		0,    0,    0,  one,
		one,    0,    0,  one,
		one,  one,    0,  one,
		0,  one,    0,  one,
		0,    0,  one,  one,
		one,    0,  one,  one,
		one,  one,  one,  one,
		0,  one,  one,  one,
};

static byte indices[] = {
		0, 4, 5,    0, 5, 1,
		1, 5, 6,    1, 6, 2,
		2, 6, 7,    2, 7, 3,
		3, 7, 4,    3, 4, 0,
		4, 7, 6,    4, 6, 5,
		3, 0, 1,    3, 1, 2
};

void Cube_draw()
{
	glFrontFace(GL_CW);

	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0 , colors);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
}

#undef one
#define one 15.0f

//CUBE_L

static GLfloat lvertices[24] = {
		-one,   0,   -one,
		 one,    0,  -one,
		 one,   one, -one,
		-one,  one,  -one,
		-one,   0,    one,
		 one,    0,   one,
		 one,   one,  one,
		-one,  one,   one,
};

#undef one
#define one 100.0f
static GLfloat lcolors[] = {
		one,  0,    one,  one/2,
		one,  one,    one,  one/2,
		one,  0,    one,  one/2,
		one,  one,    0,  one/2,
		0,    one,        0,  one/2,
		0,    one,        one,  one/2,
		0,    0,        one,  one/2,
		one,    0,        0,  one/2,
};

static byte lindices[] = {
		0, 4, 5,    0, 5, 1,   //bottom
		1, 5, 6,    1, 6, 2,    //right
	//	2, 6, 7,    2, 7, 3,    //top
		3, 7, 4,    3, 4, 0,   //left
		//4, 7, 6,    4, 6, 5,  //front
		3, 0, 1,    3, 1, 2    //back
};

void Cube_draw_L()
{
	glFrontFace(GL_CW);

	glVertexPointer(3, GL_FLOAT, 0, lvertices);
	glColorPointer(4, GL_FLOAT, 0 , lcolors);

	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, lindices);
}

int getSphereData(float *&ps,float *&pn, unsigned int *&pi, float r)
{
	const float PI = 3.141592f;
	int row=20; //行
	int col=20;//列
	int p_num=row*col+2;
	//ps=new float[p_num*3];
	//pn=new float[p_num*3];
	//pi=new unsigned int[3*2*row*col];
	ps=(float *)malloc(p_num*3*sizeof(float));
	pn=(float *)malloc(p_num*3*sizeof(float));
	pi=(unsigned int *)malloc(3*2*row*col*sizeof(unsigned int));
	int psi=0;
	int pni=0;
	int pii=0;
	float thera_r=PI/(row+1);
	float thera_c=2*PI/col;

	float cxs[col];
	float czs[col];

	for(int i=0;i<col;i++)
	{
		cxs[i]=sin(i*thera_c);
		czs[i]=cos(i*thera_c);
	}

	ps[psi++]=0;
	ps[psi++]=r;
	ps[psi++]=0;

	pn[pni++]=0;
	pn[pni++]=1;
	pn[pni++]=0;

	for(int i=1;i<row+1;i++)
	{
		float y0=sin(PI/2-i*thera_r);
		float y=r*y0;
		float r1=r*cos(PI/2-i*thera_r);
		for(int j=0;j<col;j++)
		{
			ps[psi++]=r1*cxs[j];
			ps[psi++]=y;
			ps[psi++]=r1*czs[j];

			pn[pni++]=cxs[j];
			pn[pni++]=y0;
			pn[pni++]=czs[j];

		}
	}

	ps[psi++]=0;
	ps[psi++]=-r;
	ps[psi++]=0;

	pn[pni++]=0;
	pn[pni++]=-1;
	pn[pni++]=0;


	for(int i=1;i<col;i++)
	{
		pi[pii++]=0;
		pi[pii++]=i;
		pi[pii++]=i+1;
	}
	pi[pii++]=0;
	pi[pii++]=col;
	pi[pii++]=1;

	for(int i=1;i<row;i++)
	{
		int n=i*col+1;
		for(int j=0;j<col-1;j++)
		{
			pi[pii++]=n-col+j;
			pi[pii++]=n+j;
			pi[pii++]=n+j+1;

			pi[pii++]=n-col+j;
			pi[pii++]=n-col+j+1;
			pi[pii++]=n+j+1;
		}
		pi[pii++]=n-1;
		pi[pii++]=n+col-1;
		pi[pii++]=n;

		pi[pii++]=n-1;
		pi[pii++]=n;
		pi[pii++]=n-col;
	}

	for(int i=0;i<col-1;i++)
	{
		pi[pii++]=p_num-1-col+i;
		pi[pii++]=p_num-1;
		pi[pii++]=p_num-col+i;
	}
	pi[pii++]=p_num-2;
	pi[pii++]=p_num-1;
	pi[pii++]=p_num-1-col;

	return 3*2*row*col;
}

void draw_sphere(float *ps,float *pn, unsigned int * pi, int pNum)
{
	    glDisableClientState(GL_COLOR_ARRAY);
		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		//glColorPointer(3, GL_FLOAT, 0 , pn);
		glVertexPointer(3, GL_FLOAT, 0, ps);
		glNormalPointer(GL_FLOAT, 0, pn);
		glDrawElements(GL_TRIANGLES, pNum, GL_UNSIGNED_INT, pi);
		glEnableClientState(GL_COLOR_ARRAY);
		//glPopMatrix();
}

void draw_sphere(float x_, float y_, float z_, float r)
{
	//glPushMatrix ();
	//glTranslatef (x_, y_, z_);

	const float PI = 3.141592f;
	int row=20; //行
	int col=20;//列
	int p_num=row*col+2;
	float ps[p_num*3];
	float pn[p_num*3];
	unsigned int pi[3*2*row*col];
	int psi=0;
	int pni=0;
	int pii=0;
	float thera_r=PI/(row+1);
	float thera_c=2*PI/col;

	float cxs[col];
	float czs[col];

	for(int i=0;i<col;i++)
	{
		cxs[i]=sin(i*thera_c);
		czs[i]=cos(i*thera_c);
	}

	ps[psi++]=0;
	ps[psi++]=r;
	ps[psi++]=0;

	pn[pni++]=0;
	pn[pni++]=1;
	pn[pni++]=0;

	for(int i=1;i<row+1;i++)
	{
		float y0=sin(PI/2-i*thera_r);
		float y=r*y0;
		float r1=r*cos(PI/2-i*thera_r);
		for(int j=0;j<col;j++)
		{
			ps[psi++]=r1*cxs[j];
			ps[psi++]=y;
			ps[psi++]=r1*czs[j];

			pn[pni++]=cxs[j];
			pn[pni++]=y0;
			pn[pni++]=czs[j];

		}
	}

	ps[psi++]=0;
	ps[psi++]=-r;
	ps[psi++]=0;

	pn[pni++]=0;
	pn[pni++]=-1;
	pn[pni++]=0;


	for(int i=1;i<col;i++)
	{
		pi[pii++]=0;
		pi[pii++]=i;
		pi[pii++]=i+1;
	}
	pi[pii++]=0;
	pi[pii++]=col;
	pi[pii++]=1;

	for(int i=1;i<row;i++)
	{
		int n=i*col+1;
		for(int j=0;j<col-1;j++)
		{
			pi[pii++]=n-col+j;
			pi[pii++]=n+j;
			pi[pii++]=n+j+1;

			pi[pii++]=n-col+j;
			pi[pii++]=n-col+j+1;
			pi[pii++]=n+j+1;
		}
		pi[pii++]=n-1;
		pi[pii++]=n+col-1;
		pi[pii++]=n;

		pi[pii++]=n-1;
		pi[pii++]=n;
		pi[pii++]=n-col;
	}

	for(int i=0;i<col-1;i++)
	{
		pi[pii++]=p_num-1-col+i;
		pi[pii++]=p_num-1;
		pi[pii++]=p_num-col+i;
	}
	pi[pii++]=p_num-2;
	pi[pii++]=p_num-1;
	pi[pii++]=p_num-1-col;

	glDisableClientState(GL_COLOR_ARRAY);
	glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
	//glColorPointer(3, GL_FLOAT, 0 , pn);
	glVertexPointer(3, GL_FLOAT, 0, ps);
	glNormalPointer(GL_FLOAT, 0, pn);
	glDrawElements(GL_TRIANGLES, 3*2*row*col, GL_UNSIGNED_INT, pi);

	//glPopMatrix();
}

void drawAxis()
{
float vertics[12]={
		0,   0,   0,
		10,  0,   0,
		0,   10,  0,
		0,   0,   10
};

float colors[16]={
		1,   1,   1, 1,
		1,  0,   0,  1,
		0,   1,  0,  1,
		0,   0,   1, 1
};

unsigned int index[6]={
		0  ,  1,
		0  ,  2,
		0  ,  3
};

glColorPointer(4, GL_FLOAT, 0 , colors);
glVertexPointer(3, GL_FLOAT, 0, vertics);
glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, index);

}

#undef one
#define one 15.0f

void QuatToMatrix( btQuaternion* quat, float m[16] )
{
 float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

 //计算相关的系数
 x2 = quat->x() + quat->x();
 y2 = quat->y() + quat->y();
 z2 = quat->z() + quat->z();

 xx = quat->x() * x2;
 xy = quat->x() * y2;
 xz = quat->x() * z2;
 yy = quat->y() * y2;
 yz = quat->y() * z2;
 zz = quat->y() * z2;
 wx = quat->w() * x2;
 wy = quat->w() * y2;
 wz = quat->w() * z2;

 //将其填入矩阵位置
 m[0] = 1.0 - (yy +zz);
 m[1] = xy - wz;
 m[2] = xz + wy;
 m[3] = 0.0;
 m[4] = xy + wz;
 m[5] = 1.0-(xx+zz);
 m[6] = yz + wx;
 m[7] = 0.0;
 m[8] = xz + wy;
 m[9] = yz + wx;
 m[10] = 1.0 - (xx + yy);
 m[11] = 0.0;
 m[12] = 0.0;
 m[13] = 0.0;
 m[14] = 0.0;
 m[15] = 1.0;

}
#endif //_GRAPH_H_
