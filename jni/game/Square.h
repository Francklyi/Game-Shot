#pragma once

class Square
{
public:
	float vertics[13];
	float texCoord[8];
	unsigned short index[6];
	float pColor[3];

	float interval[3];

	Square();
	void setPos();
	void draw();
};
