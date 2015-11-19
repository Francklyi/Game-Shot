#pragma once

#include <vector>
#include "Vector3.h"
#include "joint.h"
#include "GLSL/GLSL.h"
#include "GLSL/material.h"
#include "Mtl.h"
#include "Animation.h"
#include "roles/movement.h"
#include <string>
using std::string;

class GraphData {
public:
	enum{USE_ONE_UBO,USE_UBOS,USE_ARRAY};
	int useDataModel;
	char *fileData;
	int pointerNum;
	GLuint ubo[1];  //v、vn、vt间隔排列的数据的buffer的id
	//GLuint ubos[3]; //分别为v、vn、vt各自的单排的buffer对象的id
	//GLuint ubo_ani[1];  //used for vertex animation
	float *vp, *vnp, *vtp, *vp_;

	int jointNump;
	float *weightsp;
	int numofMtl;

	Mtl **pmtls;//pointer to mtl*
	Animation *animations;

	vector<Movement*> vMovements;

	//vector<Animation *> vAnims;

	enum{ANIM_WALK1=0, ANIM_RUN1};

	char *fileName;

	string strFile;

	float *batch_v;

	unsigned short *batch_vi;

	GLuint batch_ubo[1];

	GLuint batch_uboi[1];

	GLuint uboAniV[1];

	int id_REG;//id of RenderEntityGroup

	int dataType;//0: 间隔排列 1:顺序排列

	enum {
		DATATYPE_0=0,//1:间隔排列
		DATATYPE_1   //0:顺序排列
	};

	static vector<GraphData*> vGDAll;
private:
	GraphData();

	void setFileName(char *fileName1);

	//load ".ly" file set by fileName do not need init
	void load();
public:
	static GraphData* getInstance(char *fileName1);

	~GraphData();

	void init(float *vp_,float *vnp_,float *vtp_, unsigned short *vip_
			, int viLength, GLuint textureID, int batchNum);

	void draw();

	void setAttribPointer();

	void setVp(float *vp_){ vp=vp_; }

	void setVnp(float *vnp_){ vnp=vnp_; }

	void setVtp(float *vtp_){ vtp=vtp_; }

	void setUbo(GLuint ubo_) { ubo[0]=ubo_; }

	void setMtl(Mtl *pMtlsin);

	void addMovement(Movement* pM);

	Animation *loadAnimation(char * fileName);

	//void setGraphData(GraphData *pGD_);

	void setAniBuffers(Animation *pA_);

	void setAniBuffers2(Movement* pM);

	Animation * getAnimation();

	void getFramePointer(float *pf3, Animation *pA, int frame,int num);

	bool writeCache(Movement* pM);

	void setBatchs(int num);

	void updateMove(Animation *pA_,int frame);

	static GraphData* isLoad(char *fileName);

};
