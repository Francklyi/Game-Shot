#ifndef _JOINT_H_
#define _JOINT_H_
class Joint{
public:
	int sn;
	float posf[3];
	float *Matrix;
	float *Matrix_1;//only for root joint
};
#endif //_JOINT_H_
