#include "AABB.h"

AABB::AABB(float minX, float maxX,
		   float minY, float maxY,
		   float minZ, float maxZ )
{
	this->minX=minX;
	this->maxX=maxX;
	this->minY=minY;
	this->maxY=maxY;
	this->minZ=minZ;
	this->maxZ=maxZ;
}
