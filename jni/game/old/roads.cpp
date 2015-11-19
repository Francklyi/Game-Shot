#include"roads.h"

Roads::Roads():
roads(0)
{

}

Roads::~Roads()
{

}

bool Roads::load()
{
   n=10;
   ids = (int*)malloc(n*sizeof(int));
   for(int i=0; i<n; i++) ids[i] = 0;
}

bool Roads::update()
{
  roads[ii]->update();
}

int Roads::addRoadPart(RoadPart *roadPart)
{
  roads.push_back(roadPart);
  return roads.size()-1;
}

void Roads::romoveRoadPart(int)
{

}
