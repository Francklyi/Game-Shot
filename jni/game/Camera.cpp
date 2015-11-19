#include "Camera.h"
//#include <GLES/gl.h>
//#include "glu.h"
#include "log.h"

Camera::Camera()
:extHeight(0)
{
        Vector3 Position=Vector3(10,10,20);
	Vector3 View    =Vector3(0,0,0);
	Vector3 UpVector=Vector3(0,1,0);

	m_Position=Position;
	m_View    =View;
	m_UpVector=UpVector;
	m_pCamera=this;
}

Camera::Camera( float positionX ,float positionY, float positonZ,
	                    float viewX,     float viewY,     float viewZ,
	                    float upVectorX, float upVectorY, float upVectorZ)
{
	Vector3 Position=Vector3(positionX,positionY,positonZ);
	Vector3 View    =Vector3(viewX,viewY,viewZ);
	Vector3 UpVector=Vector3(upVectorX,upVectorY,upVectorZ);

	m_Position=Position;
	m_View    =View;
	m_UpVector=UpVector;
	m_pCamera=this;
}

Camera::~Camera()
{

}

void Camera::setCamera( float positionX ,float positionY, float positonZ,
	                    float viewX,     float viewY,     float viewZ,
	                    float upVectorX, float upVectorY, float upVectorZ)
{
	m_Position=Vector3(positionX,positionY,positonZ);
	m_View =Vector3(viewX,viewY,viewZ);
	m_UpVector=Vector3(upVectorX,upVectorY,upVectorZ);

	//m_Position=Position;
	//m_View    =View;
	//m_UpVector=UpVector;
}

void Camera::rotateView(float angle, float x, float y, float z){
	Vector3 newView;
	//计算方向向量
	Vector3 view = m_View - m_Position;

	//计算sin 和 cos 值
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	//计算旋转向量的x值
	newView.x = ( cosTheta+ (1- cosTheta) * x * x)    * view.x;
	newView.x +=((1-cosTheta) * x * y - z * sinTheta) * view.y;
	newView.x +=((1-cosTheta) * x * z + y * sinTheta) * view.z;
	//计算旋转向量的y值
	newView.y =  ((1-cosTheta) * x * y + z * sinTheta) * view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y *y   ) * view.y;
	newView.y += ((1-cosTheta) * y * z - x * sinTheta) * view.z;
	//计算旋转向量的z值
	newView.z =  ((1-cosTheta) * x * z - y * sinTheta) * view.x;
	newView.z += ((1-cosTheta) * y * z + x * sinTheta) * view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z  ) * view.z;

	m_View = m_Position + newView;
}

void Camera::rotatePos(float angle, float x, float y, float z){
	//if(x==m_UpVector.x && y==m_UpVector.y&& z==m_UpVector.z)
	//return;
	Vector3 newView;
	//计算方向向量
	Vector3 view = m_Position - m_View;

	//计算sin 和 cos 值
	float cosTheta = (float)cos(angle);
	float sinTheta = (float)sin(angle);

	//计算旋转向量的x值
	newView.x = ( cosTheta+ (1- cosTheta) * x * x)    * view.x;
	newView.x +=((1-cosTheta) * x * y - z * sinTheta) * view.y;
	newView.x +=((1-cosTheta) * x * z + y * sinTheta) * view.z;
	//计算旋转向量的y值
	newView.y =  ((1-cosTheta) * x * y + z * sinTheta) * view.x;
	newView.y += (cosTheta + (1 - cosTheta) * y *y   ) * view.y;
	newView.y += ((1-cosTheta) * y * z - x * sinTheta) * view.z;
	//计算旋转向量的z值
	newView.z =  ((1-cosTheta) * x * z - y * sinTheta) * view.x;
	newView.z += ((1-cosTheta) * y * z + x * sinTheta) * view.y;
	newView.z += (cosTheta + (1 - cosTheta) * z * z  ) * view.z;

	m_Position =m_View +newView;//


}

void Camera::setViewByMouse()
{

}
/*左右移动摄像机*/
void Camera::yawCamera(float speed)
{
	Vector3 yaw;
	Vector3 cross=m_View - m_Position;
	cross=cross.crossProduct(m_UpVector);

	//单位化
	yaw=cross.normalize();
	//更新摄像机位置
	m_Position.x += yaw.x * speed;
	m_Position.z += yaw.z * speed;
	//更新瞄准位置
	m_View.x += yaw.x * speed;
	m_View.z += yaw.z * speed;
}

//前后移动摄像机

void Camera::moveCamera(float speed)
{
	//计算方向向量
	Vector3 vec3 = m_View - m_Position;
	vec3 = vec3.normalize();

	//更新摄像机
	m_Position.x += vec3.x * speed;
	m_Position.z += vec3.z * speed;

	m_View.x += vec3.x * speed;
	m_View.z += vec3.z * speed;
}

Vector3 Camera::changeCoord(const Vector3& vec3)
{
	Vector3 yaw,newVec3;
	Vector3 dir=m_View - m_Position;
	dir=dir.normalize();
	Vector3 cross=dir.crossProduct(m_UpVector);
	yaw=cross.normalize();
	newVec3=yaw*vec3.x+m_UpVector*vec3.y+dir*vec3.z;
	           //Vector3(vec3.x*yaw.x+vec3.x*dir.x,
			 //       vec3.x*yaw.x+vec3.x*dir.x,
			 //       m_Position.z);
	return newVec3;
}

void Camera:: setLook()
{

	//LOGI("%f , %f , %f \n", m_Position.x, m_Position.y, m_Position.z);
	//LOGE("%f , %f , %f \n", m_View.x, m_View.y, m_View.z);
	//LOGI("%f , %f , %f \n", m_UpVector.x, m_UpVector.y, m_UpVector.z);
	//suLookAt( m_Position.x, m_Position.y, m_Position.z,
	//	       m_View.x, m_View.y, m_View.z ,
	//		   m_UpVector.x, m_UpVector.y, m_UpVector.z);
  getGL().setLookAt( m_Position.x, m_Position.y+extHeight, m_Position.z,
                   m_View.x, m_View.y, m_View.z ,
                   m_UpVector.x, m_UpVector.y, m_UpVector.z);
}

Camera *camera1 = new Camera();
