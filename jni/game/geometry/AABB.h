#ifndef _AABB_H_
#define _AABB_H_

class AABB
{
public:
	AABB();
	AABB(float minX, float maxX,
		 float minY, float maxY,
		 float minZ, float maxZ );
private:
	//六个变量
	float minX;//x轴最小位置
	float maxX;//x轴最大位置
	float minY;//y轴最小位置
	float maxY;//y轴最大位置
	float minZ;//z轴最小位置
	float maxZ;//z轴最大位置
};

#endif //_AABB_H_
