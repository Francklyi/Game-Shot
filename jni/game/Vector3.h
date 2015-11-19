#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>

class Vector3{
public:
	float x , y , z;
	/*构造函数*/
	Vector3(){ x=0.0; y=0.0; z=0.0;}
	Vector3(float xx ,float yy ,float zz){
		x=xx;
		y=yy;
		z=zz;
	}

	Vector3(const Vector3& vec3){
		x=vec3.x;
		y=vec3.y;
		z=vec3.z;
	}

	/*成员函数*/
	float length();                              //计算向量长度
	Vector3 normalize();                         //单位化向量
	float dotProduct(const Vector3& vec3);        //计算点积
	Vector3 crossProduct(const Vector3& vec3);     //计算叉积

		/*重载操作符*/
	Vector3 operator + (const Vector3& v);
	Vector3 operator - (const Vector3& v);
	Vector3 operator * (float scale);
	Vector3 operator / (float scale);
	Vector3 operator - ();
};

#endif //_VECTOR_H_
