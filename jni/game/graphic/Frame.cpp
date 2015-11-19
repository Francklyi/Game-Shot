#include "Frame.h"

void Frame::setInfo(int num, int type_)
{
	numofFrame=num;
	type=type_;
}

void Frame::setType( int type_)
{
	type=type_;
}

int Frame::getType()
{
	return type;
}

void Frame::setInfo(int * info)
{
	numofFrame=info[0];
	type=info[1];
	pos[0]=info[2];
	pos[1]=info[3];
	pos[2]=info[4];
	hitLength2=info[5];
	hp=info[6];
}
