#pragma once

class KeyFrame
{
public:
	enum{
		TYPE_HIT,
		TYPE_SPEC1,
		TYPE_SPEC2
	};
	int type;
	int numofFrame;
	float hitLength;

public:
	KeyFrame();

	void setKeyFrameNum(int num);
};
