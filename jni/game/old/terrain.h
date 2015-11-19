#ifndef _TERRAIN_H_
#define _TERRAIN_H_
//#include "loadFile.h"

class Terrain
{
public:
	Terrain(char *fileName1);
	~Terrain();
	void init();
	float* getXY(float xl , float yl ,float xn , float yn);
	float  getAveHeight(float x, float y);

	bool loadRawFile(char *fileName1);
	float getHeight(float x, float y);
	int getSquaretype(float x, float y);

	float *getNearLine(float x, float y); // get the nearest line
public:
	char *fileName;
	unsigned int width;
	unsigned int height;
	float *heightArray;
	float cellWidth;
	float X0;
	float Y0;
	enum{ SQUARE_LOAD, //can get in
		SQUARE_TABLE,//get in only when it is higher
		SQUARE_FORBIDDEN //forbidden getting in
		};
};

#endif //_TERRAIN_H_
