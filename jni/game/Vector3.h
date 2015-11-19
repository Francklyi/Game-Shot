#ifndef _VECTOR_H_
#define _VECTOR_H_

#include <math.h>

class Vector3{
public:
	float x , y , z;
	/*���캯��*/
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

	/*��Ա����*/
	float length();                              //������������
	Vector3 normalize();                         //��λ������
	float dotProduct(const Vector3& vec3);        //������
	Vector3 crossProduct(const Vector3& vec3);     //������

		/*���ز�����*/
	Vector3 operator + (const Vector3& v);
	Vector3 operator - (const Vector3& v);
	Vector3 operator * (float scale);
	Vector3 operator / (float scale);
	Vector3 operator - ();
};

#endif //_VECTOR_H_
