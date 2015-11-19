#ifndef _MOVEMENT_H_
#define _MOVEMENT_H_

//#include "graphic/GraphData.h"
#include "graphic/animation.h"

class Movement
{
public:
	int id;

	char *fileName;

	int code;

	int attack;

	//GraphData *pGD;

	Animation *pA;

public:
	float HP;

	Movement():id(0),code(0),attack(0),fileName(0),pA(0)
	{

	}
	Movement( int id, int code_, int attack_, char *fileName_, int type );
	~Movement();

	bool setFileName(char *fileName_){ fileName=fileName_; }

	Animation *loadAnimation(char * fileName);

	bool load();

	int getCode(){ return code; }

	//void setGraphData(GraphData *pGD_);

	//void setAniBuffers();

	Animation * getAnimation();

	//void getFramePointer(float *pf3, int frame,int num);

	float getHP();
};
#endif //_MOVEMENT_H_
