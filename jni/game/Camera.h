#ifndef _CAMERA_H_
#define _CAMERA_H_
//#include "mygame1jni.h"
#include "GLSL/GLSL.h"
#include "Vector3.h"

class Camera{
public:
	/*构造函数*/
	Camera();
	Camera( float positionX ,float positionY, float positonZ,
		                    float viewX,     float viewY,     float viewZ,
		                    float upVectorX, float upVectorY, float upVectorZ);
	~Camera();

	/*获得摄像机状态的方法*/
	Vector3 getPosition(){ return m_Position; }
	Vector3 getView()    { return m_View;     }
	Vector3 getUpVector(){ return m_UpVector; }
	float   getSpeed()   { return m_Speed;    }

	/*设置速度*/
	void setSpeed(float speed){
		m_Speed = speed;
	}

	/*设置摄像机的位置，观察点和向上向量*/
	void setCamera( float positionX ,float positionY, float positonZ,
		            float viewX, float viewY, float viewZ,
		            float upVectorX, float upVectorY, float upVectorZ);

	/*旋转摄像机方向*/
	void rotateView(float angle, float X, float Y, float Z);

	/*旋转摄像机位置*/
	void rotatePos(float angle, float X, float Y, float Z);

	/*根据鼠标设置摄像机观察方向*/
	void setViewByMouse();
	/*左右移动摄像机*/
	void yawCamera(float speed);

	/**前后移动摄像机*/
	void moveCamera(float speed);

	/*放置摄像机*/
	void setLook();

	/*得到摄像机指针*/
	Camera* getCamera(void){ return m_pCamera; }

	/*坐标变换*/
	Vector3 changeCoord(const Vector3& vec3);

	/*设置朝向*/
	void setView(Vector3 vec3){m_View=vec3; };

	/*设置高度*/
	void setHeight(float height){m_Position.y=height;}

	Vector3 getVeiwPostiotn(){return m_View;}
//private:
	/*摄像机属性*/
	Camera *m_pCamera;   /** <当前全局摄像机指针*/
	Vector3   m_Position;       /** <位置*/
	Vector3   m_View;           /** <朝向 */
	Vector3   m_UpVector;       /** <向上向量*/
	Vector3   m_PointVector;
	float     m_Speed;          /** <速度 */

	float extHeight;
};

//全局照相机1指针
extern Camera *camera1;
#endif //_CAMERA_H_
