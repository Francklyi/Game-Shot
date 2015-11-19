#ifndef _GDATA_H_
#define _GDATA_H_

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <vector>
#include "Vector3.h"
#include "joint.h"
#include "GLSL/GLSL.h"
#include "GLSL/material.h"
//#include "Entity.h"

using std::vector;

class Mtls{
public:
	//char* name;
	char *Lmap_Ka;//��ʾ�洢map_Ka�ĳ��ȵ�ָ�룬��ָ�����һ���ֽھ���map_Ka���׸��ַ���λ�á�
	char *Lmap_Kd;
	float *Ns, *Ni, *d, *Tr,
		*Tf , *Ka, *Kd, *Ks, *Ke,
		*v, *vn, *vt;
	int *illum,
		nv, nvt, nvn,
		w , h;
	GLuint mTextureId;//��ɫ����id
	GLuint mNormalTexId;//��������id
	unsigned active_texture_id;//���ڰ���ɫ����ʹ�õ�������0��ʼ
	char *Lbp;
	//float ** flp;
	//struct VTNbuffer *VTNp;

	int viL;//number of vi
	unsigned short *vip;
	GLuint uboi[1];// for vi
};


class Animation{
public:
	int direction; //1��ʾ����-1��ʾ����
	int frame;       //��ǰ֡λ��
	int *frameNum;
	float *frameTime;
	int *jointNum;
	Joint *joints;
	char *fileName;
	//float *jointMatrixs;
	char change_stay;//��Ͳ���
};

class GraphData{
public:
	char *fileData;
	int pointerNum;
	float *vp, *vnp, *vtp , *vp_;
	GLuint ubo[1];
	int jointNump;
	float *weightsp;
	int numofMtls;
	Mtls *np;
	Animation *animations;
};

class Graph
{

public:
	GraphData *gp;
	static GLSL *glsl;
	Vector3 position;
	char *fileName;
	static int TextureId;
	static bool useTexture;
	//char *fileNamea;
	Graph(){ gp=0; TextureId=0;}

	Graph(char *fileName1){ Graph(); fileName=fileName1; }

	~Graph();

	void setFileName(char *fileName1){ fileName=fileName1; }

	//void setAniFile(char * fileName1){ fileNamea=fileName1; }

	GraphData * getEntityDataHandler(){ return gp; }

	void load();

	void drawMovement();

	void drawNoMovement();

	virtual void draw();

        void setAction(Animation*animations_)
        {
          gp->animations=animations_;
               // moving = true;
        }

	void setPosition(Vector3 position1){ position=position1;}

	Vector3 getPosition() { return position;}

	void loadModel(GraphData *gp,char * fileNamei);

	void getMtls(char **cp,GraphData *gp);

	void getOneMtl(char **cp,Mtls *Mp);

	GLuint setTexture(char **cp);

	GLuint setTextureB(char *map_Kd);

	void setMaterial(Mtls *Mp);
	//void loadAnimation(GraphData *gp,char * fileName);

	int getNewmtlNum(char **cp){ return (int)*((*cp)++); }

	static void setTextureId(int id){ TextureId = id; }

	static void setUseTexture(bool useTexture_){ useTexture = useTexture_; }
};
#endif //_GDATA_H_
