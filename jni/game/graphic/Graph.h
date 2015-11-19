#ifndef _GDATA_H_
#define _GDATA_H_
#include "Vector3.h"
#include "joint.h"
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "GLSL/GLSL.h"
#include "GLSL/material.h"
#include "GraphData.h"
#include "Entity.h"
#include "parser/ParserLy.h"
#include "fourNumber.h"
#include "graphic/MShape.h"
#include "terrain.h"
//#include "EntityPhysic.h"
#include "roles/movement.h"
#include "graphic/AnimForGraph.h"
#include "Object.h"

#include "PhyPlane.h"
#include "PhyScene.h"
#include "RenderEntity.h"
//#include "RenderScene.h"
#include "Movepath.h"

#include <iostream>
using namespace std;
using std::vector;
using std::string;

class Graph:public Entity
{
public:
	enum {
		ROLE_FIXED0=0, ROLE_FIXED1, ROLE_FIXED2,
		ROLE_LEAD,
		ROLE_SCENE0, ROLE_SCENE1,

		ROLE_MOVE0, ROLE_MOVE1,
		ROLE_MOVE2, ROLE_MOVE3,
		ROLE_MOVE4, ROLE_MOVE5,
		ROLE_MOVE6, ROLE_MOVE7
	};

	GraphData *gp;

	//static GLuint uboLast;

	//static GLuint uboiLast;

	Object * pGraphRace;
	//vector<Movement*> vMovement;

	//static GLSL *glsl;
	Vector3 position;

	char *fileName;

	string fileStr;

	char *fileNamea;

	int order; //���ڱ�Ǹ�ģ���� GraphRace�е�˳��

	//static int TextureId;

	//static int frameNum;

	float floats1[4];

	//static FourNumber fourNumber1;

	static bool useTexture;

	bool isChoosed;

	int state; // 0: ����ʾ�� 1:���ɼ� 2:ɾ��

	/**the type of the role**/
	int roleType;

	ParserLy *pParserLy;

	MShape *pMShape;

	//EntityPhysic *pEP;

	void(*changeFunc)(void);

	Vector3 centre;

	GLuint NTextureId;

	int moveState;

	enum {	STATE_RUNNING, STATE_STOP };

	bool lockMovement;

	float *changeArgu;
	//char *fileNamea;
	int delayAll;

	int delayStep;

	float floorHeight;

	Terrain *pT;

	AnimForGraph *pAFG;

	vector<AnimForGraph *> vpAFGs;

	Movement* pMoveNow;

	int moveCode;

	float pf32[32];

	bool hasChange;

	float changeRotateY;

	Frame *pFrameNow;

	float nowPos[3];

	float nextPos[3];

	float vel[3];

	float bodyHeight;

	float pfTemp4[4];

	//static PhyPlane *pPlane1;

	//static PhyPlane *pPlane2;

	PhyScene *pPScene;

	RenderEntity *pREntity;

	int Id_REG;

	int moveType;

	enum MoveType{
		FIXED,MOVE_PERSISTENT,MOVE_TRIGGER
	};

	int glslFunctype;

	Movepath *pMovepath;

	GLuint uboAni;

	float *pfAniVertex;

	float *pfMoveStepNow;

	int followType;

	enum{
		FOLLOWTYPE_NO , FOLLOWTYPE_YES
	};

	void setGlslFuncType(int functype_);

	void setPhyScene(PhyScene *pPScene_);

	Graph();

	Graph(char *fileName1);

	~Graph();

	void setFileName(char *fileName1){
		fileName=fileName1;
		fileStr=string(fileName1);
	}

	void setRoleType(int type_);

	int getRoleType();

	//void setAniFile(char * fileName1){ fileNamea=fileName1; }

	GraphData * getEntityDataHandler(){ return gp; }

	void load();

	virtual void reset();

	void drawMovement();

	void drawNoMovement();

	virtual void draw();

	void update();

	virtual void render();

	void setAction(Movement* pMoveNow);

	void updatePhysic();

	void setShape();

	void setPosition(Vector3 position1){ position=position1;}

	Vector3 getPosition() { return position;}

	void loadModel(GraphData *gp,char * fileNamei);

	void setMaterial(Mtl *Mp);

	void setAniFile(char * fileName1){ fileNamea=fileName1; }

	void loadAnimation(GraphData *gp,char * fileName);

	//static void setTextureId(int id){ TextureId = id; }

	static void setUseTexture(bool useTexture_){ useTexture = useTexture_; }

	void setChange(void(*func)(void)){ changeFunc = func;}

	void setOrder(int order_){
		order=order_;
	}

	int getState(){ return state;}

	void setFuncType(int funcType_);

	GLuint setNTexture(char *NTextureFile);

	GLuint setTexture(BYTE *pixels,int w, int h,unsigned int format);

	void action(float theta);

	float * getActionVp();

	float getCentreHeight();

	void addRotateY(float theta);

	void setRotateY(float theta);

	void setChange(float *pf);

	void changeMatrix();

	void setTerrain(Terrain *pT_){ pT = pT_; }

	void setFloorH(float fH){ floorHeight= fH; }

	void setVpAFG();

	void act(int code_);

	void addMove(int code_,char *pfile,int type);

	bool isPointerScreen();

	void setGraphRace(Object * pGraphRace_);

	void removeFromRace();

	virtual void makeTransform();

	virtual void updateAfterTransform();

	virtual void updateAfterDraw();

	void calVelocity();

	void setMoveType(int type);

	void updateTransform();

	void addMovepoint(Movepoint *pMovepoint);

	//virtual void move(float x, float y, float z);

	void updateMove();

	void initMovepath();

};
#endif //_GDATA_H_
