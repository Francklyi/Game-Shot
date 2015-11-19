#ifndef _GRAPH_H_
#define _GRAPH_H_

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>

#include "log.h"

#define SCALING 2
#define one SCALING
typedef unsigned char byte;

int num_bo=3;
GLuint *ubo;

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
	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, vertices);
	glColorPointer(4, GL_FLOAT, 0 , colors);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, indices);
}

#undef one
#define one 20.0f

//CUBE_L

static GLfloat lvertices[24] = {
		-one,   -one/2,   -one,
		 one,    -one/2,  -one,
		 one,   one, -one,
		-one,  one,  -one,
		-one,   -one/2,    one,
		 one,    -one/2,   one,
		 one,   one,  one,
		-one,  one,   one,
};

#undef one
#define one 1.0f
static GLfloat lcolors[] = {
		one,  0,    one,  one,
		one,  one,    one,  one,
		one,  0,    one,  one,
		one,  one,    0,  one,
		0,    one,        0,  one,
		0,    one,        one,  one,
		0,    0,        one,  one,
		one,    0,        0,  one,
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
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_NORMAL_ARRAY);
	//glEnableClientState(GL_COLOR_ARRAY);
	glVertexPointer(3, GL_FLOAT, 0, lvertices);
	glColorPointer(4, GL_FLOAT, 0 , lcolors);
	glDrawElements(GL_TRIANGLES, 24, GL_UNSIGNED_BYTE, lindices);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_NORMAL_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
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

	ubo=(GLuint*)malloc(num_bo*sizeof(GLuint));
	glGenBuffers(num_bo,ubo);
	LOGI("ubo:%d",ubo[0]);

	glBindBuffer(GL_ARRAY_BUFFER, ubo[0]);
	glBufferData(GL_ARRAY_BUFFER, p_num*3*sizeof(float), ps, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ubo[1]);
	glBufferData(GL_ARRAY_BUFFER, p_num*3*sizeof(float), pn, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ubo[2]);
	glBufferData(GL_ARRAY_BUFFER, p_num*3*sizeof(float), pn, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*2*row*col*sizeof(unsigned int), pi, GL_STATIC_DRAW);

    LOGI("getSphereData");
	return 3*2*row*col;
}

int getSphereData1(float r, GLuint* ubo)
{
	const float PI = 3.141592f;
	int row=10; //行
	int col=10;//列
	int p_num=row*col+2;
	//ps=new float[p_num*3];
	//pn=new float[p_num*3];
	//pi=new unsigned int[3*2*row*col];
	float *ps=(float *)malloc(p_num*3*sizeof(float));
	float *pn=(float *)malloc(p_num*3*sizeof(float));
	unsigned int *pi=(unsigned int *)malloc(3*2*row*col*sizeof(unsigned int));
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

	int num_bo = 3;
	//ubo=(GLuint*)malloc(num_bo*sizeof(GLuint));
	glGenBuffers(num_bo,ubo);

	glBindBuffer(GL_ARRAY_BUFFER, ubo[0]);
	glBufferData(GL_ARRAY_BUFFER, p_num*3*sizeof(float), ps, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, ubo[1]);
	glBufferData(GL_ARRAY_BUFFER, p_num*3*sizeof(float), pn, GL_STATIC_DRAW);

	//glBindBuffer(GL_ARRAY_BUFFER, ubo[2]);
	//glBufferData(GL_ARRAY_BUFFER, p_num*3*sizeof(float), pn, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 3*2*row*col*sizeof(unsigned int), pi, GL_STATIC_DRAW);

    LOGI("getSphereData12");
	return 3*2*row*col;
}


void draw_sphere(int pNum)
{
	float mat_amb[]={0.2f,0.4f,0.8f,0.8f};
	float mat_diff[]={1.0f,1.0f,1.0f,1.0f};
	float mat_spe[]={0.0f,1.0f,1.0f,1.0f};
	float mat_shin[]={0.0f,1.0f,0.0f,1.0f};
	//glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT, mat_amb);
	//glMaterialfv(GL_FRONT,GL_DIFFUSE, mat_diff);
	//glMaterialfv(GL_FRONT,GL_SPECULAR, mat_spe);
	//glMaterialfv(GL_FRONT,GL_SHININESS, mat_shin);
	    //glDisableClientState(GL_COLOR_ARRAY);
		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
	    glBindBuffer(GL_ARRAY_BUFFER, ubo[0]);
		//glBufferData(GL_ARRAY_BUFFER, (pNum/2+6)*sizeof(float), ps, GL_STATIC_DRAW);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, ubo[1]);
		//glBufferData(GL_ARRAY_BUFFER, (pNum/2+6)*sizeof(float), pn, GL_STATIC_DRAW);
		glNormalPointer(GL_FLOAT, 0, 0);

		//glEnaBUFFER_OFFSETbleClientState(GL_COLOR_ARRAY);
		//glBindBuffer(GL_ARRAY_BUFFER, ubo[2]);
		//glColorPointer(3, GL_FLOAT, 0 , 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo[3]);

	    glDrawElements(GL_TRIANGLES, pNum, GL_UNSIGNED_INT, 0);

	    glDisableClientState(GL_VERTEX_ARRAY);
	    glDisableClientState(GL_NORMAL_ARRAY);
	    glDisableClientState(GL_COLOR_ARRAY);
}

void draw_sphere1(float *ps,float *pn, unsigned int * pi, int pNum)
{
	    //glDisableClientState(GL_COLOR_ARRAY);
		//glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
		glEnableClientState(GL_VERTEX_ARRAY);
	    glBindBuffer(GL_ARRAY_BUFFER, ubo[0]);
		//glBufferData(GL_ARRAY_BUFFER, (pNum/2+6)*sizeof(float), ps, GL_STATIC_DRAW);
		glVertexPointer(3, GL_FLOAT, 0, 0);

		glEnableClientState(GL_NORMAL_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, ubo[1]);
		//glBufferData(GL_ARRAY_BUFFER, (pNum/2+6)*sizeof(float), pn, GL_STATIC_DRAW);
		glNormalPointer(GL_FLOAT, 0, 0);

		glEnableClientState(GL_COLOR_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, ubo[2]);
		glColorPointer(3, GL_FLOAT, 0 , 0);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo[3]);

	    glDrawElements(GL_TRIANGLES, pNum, GL_UNSIGNED_INT, 0);

	    glDisableClientState(GL_VERTEX_ARRAY);
	    glDisableClientState(GL_NORMAL_ARRAY);
	    glDisableClientState(GL_COLOR_ARRAY);
}


void draw_sphere(float x_, float y_, float z_, float r)
{
	//glPushMatrix ();
	//glTranslatef (x_, y_, z_);

	const float PI = 3.141592f;
	int row=10; //行
	int col=10;//列
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

	//glEnableClientState(GL_COLOR_ARRAY);
	//glDisableClientState(GL_COLOR_ARRAY);
	glColor4f(1.0f, 0.0f, 1.0f, 1.0f);
	glColorPointer(3, GL_FLOAT, 0 , pn);
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
		0,   0,   0, 1,
		1,  0,   0,  1,
		0,   1,  0,  1,
		0,   0,   1, 1
};

unsigned int index[6]={
		0  ,  1,
		0  ,  2,
		0  ,  3
};

glVertexPointer(3, GL_FLOAT, 0, vertics);
glColorPointer(4, GL_FLOAT, 0 , colors);
glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, index);

}

#undef one
#define one 20.0f

#endif //_GRAPH_H_
