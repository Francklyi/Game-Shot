#include "animation.h"

Animation::Animation():type(TYPE_GROUND)
{
	vKF=vector<KeyFrame*>(0);
}

Animation::~Animation()
{

}

void Animation::setPV(float *pV_)
{
	pV=pV_;
}

void Animation::setType( int type_)
{
	type=type_;
}

int Animation::getType()
{
	return type;
}

void Animation::setFloorHeight(int floorHeight_)
{
	floorHeight = floorHeight_;
}

int Animation::getFrameFlag()
{

}

int Animation::act()
{

}

int Animation::getFrameType()
{
	return pFrames[frame].getType();
}

void Animation::setKeyFrame(int *keyFrame_, int len)
{

}

void Animation::addKeyFrame(KeyFrame *pKF)
{
	vKF.push_back(pKF);
}

int Animation::getKeyFrame(int frame)
{
	//if(vKF.size()==0) return -1;

	//if(frame<vKF[vKF.size()/2])

	//int temp=0;
	for(int i=0;i<vKF.size(); i++)
	{
		if(frame<=vKF[i]->numofFrame)
			return i;
	}
}
