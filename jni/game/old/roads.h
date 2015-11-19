#ifndef _ROADS_H_
#define _ROADS_H_
#include <vector>
#include "roadpart.h"

using std::vector;

class Roads
{
private:

  vector<RoadPart *> roads;
  int n;
  int ii;
  int *ids;

public:
  Roads();
  ~Roads();

  bool load();

  bool update();

  int addRoadPart(RoadPart *roadPart);

  void romoveRoadPart(int);
};

#endif //_ROADS_H_
