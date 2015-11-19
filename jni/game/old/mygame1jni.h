#ifndef _MYGAME1JNI_H_
#define _MYGAME1JNI_H_

#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string>
#include <math.h>
#include <vector>

#include <GLES/gl.h>
#include <EGL/egl.h>
#include <GLES/glext.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>


#include "Vector3.h"
#include "glu.h"
#include "getMtl.h"
#include "Entity.h"
#include "Camera.h"


using namespace std;
//#ifndef _Included_opengl_jni_Natives
#define _Included_opengl_jni_Natives

extern "C" {
#define  LOG_TAG    "game"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define SDCARD "./sdcard/"
#define LYLOAD "./sdcard/lyfile/"
static Vector3 mPFirst(0,0,0), mP(0,0,0), rPNow(0,0,0), rPLast(0,0,0);
class gletest{
	int k;

};

extern double glll;
extern struct Newmtl *np;
extern struct EntityData *ep;
////////////定义java端方法///////////////////////
JNIEXPORT jint JNICALL Java_com_ly_mygame1Jni_Natives_startLoadGame(JNIEnv* env,
		jobject thiz);

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_initScene(JNIEnv* env,
		jobject thiz, jint w, jint h);

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_moveCameraFirstP(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_moveCamera(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_rotateCamera(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_rotateCameraLast(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_exit(JNIEnv* env,
		jobject thiz);
}

#endif
