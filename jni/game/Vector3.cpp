#include "Vector3.h"

float Vector3::length(){
	return sqrt(x*x+y*y+z*z);
}

Vector3 Vector3::normalize(){
	float l=length();
	if(l==0) return *this;
	Vector3 vec3(0, 0, 0);
	if(l!=0) vec3=Vector3(x/l, y/l, z/l);
	return vec3;
}

float Vector3::dotProduct(const Vector3& vec3){
	return x*vec3.x+y*vec3.y+z*vec3.z;
}

Vector3 Vector3::crossProduct(const Vector3& vec3){
	Vector3 vec(
		         y * vec3.z - z * vec3.y,
				-( x * vec3.z - z * vec3.x ),
				 x * vec3.y - y * vec3.z
		       );
	return vec;
}

/*ÖØÔØ²Ù×÷·û*/
Vector3 Vector3::operator + (const Vector3& v)
{
	Vector3 vec3(x + v.x , y+ v.y, z + v.z);
	return vec3;
}
Vector3 Vector3::operator - (const Vector3& v)
{
	Vector3 vec3(x - v.x , y- v.y, z - v.z);
	return vec3;
}
Vector3 Vector3::operator * (float scale)
{
	Vector3 vec3(x * scale , y * scale, z * scale);
	return vec3;
}
Vector3 Vector3::operator / (float scale)
{
	Vector3 vec3(x / scale , y / scale, z / scale);
	return vec3;
}
Vector3 Vector3::operator - ()
{
	Vector3 vec3(-x , -y, -z);
	return vec3;
}
