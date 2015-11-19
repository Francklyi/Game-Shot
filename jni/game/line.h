#ifndef _LINE_H_
#define _LINE_H_
#include "Vector3.h"

class Line{
public:
  Line();
  ~Line();

  void setPointers(Vector3 p1_, Vector3 p2_);
  float getLength();
  void draw();
  void draw(Vector3 *p1_, Vector3 *p2_);

  void drawMults();

public:
	Vector3 p1;
	Vector3 p2;
	float *pfPointers;
	int num;
	float pf16[16];
	float pColor[3];
};

#endif //_LINE2D_H_
