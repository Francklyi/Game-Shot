#ifndef _TERRAIN_H_
#define _TERRAIN_H_
//#include "loadFile.h"
#include <vector>

using std::vector;

class Terrain
{
public:
	Terrain();
	~Terrain();
public:

	int mapWidth;
	int mapHeight;
	char *pMapData;
	vector<vector<int>*> vSceneIDs;
	int *pIScenes;
	char *pISceneIDs;
	float mapCellWidth;
	float mapX0;
	float mapY0;
	int cellNumLR;
	enum{ SQUARE_LOAD, //can get in
		SQUARE_TABLE,//get in only when it is higher
		SQUARE_FORBIDDEN //forbidden getting in
		};
};

#endif //_TERRAIN_H_
