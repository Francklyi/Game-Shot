#include "terrain.h"
#include "fileDeal/loadFile.h"
#include "log.h"

Terrain::Terrain()
:mapWidth(0)
,mapHeight(0)
,pMapData(0)
,mapCellWidth(0)
,mapX0(0)
,mapY0(0)
,cellNumLR(0)
,pIScenes(0)
,pISceneIDs(0)
{
	vSceneIDs=vector<vector<int>*>(0);
}

Terrain::~Terrain()
{

}

