#pragma once

class Frame
{
public:
	enum{
		TYPE_NORMAL, // not thing special
		TYPE_HIT1, // level 1, 轻, 反馈：后退
		TYPE_HIT2,// level 2, 重, 反馈：后倒地
		TYPE_HIT3,

		TYPE_SPEC1,
		TYPE_SPEC2
	};

	int type;
	int numofFrame;
	float hitLength2;

	float pos[3];
	float hp;

	unsigned ubo;

	void setInfo(int * info);
	void setInfo(int num, int type_);
	void setType( int type_);

	int getType();
};
