#ifndef _ROADPART_H_
#define _ROADPART_H_

#include <GLES2/gl2.h>
#include <EGL/egl.h>
#include <vector>

#include "Vector3.h"
#include "Entity.h"
#include "GLSL/GLSL.h"
#include "GLSL/material.h"

class RoadPart
{
public:
  char *fileName;
  int id;
  Graph *graph;
public:
  RoadPart();

  RoadPart(char *fileName_ );

  ~RoadPart();

  void setFile(char *);

  bool load();

  bool update();

  void draw();

  //void regester(Roads *);
};

#endif //_ROADPART_H_
