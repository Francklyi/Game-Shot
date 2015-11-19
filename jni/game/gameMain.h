#ifndef _GMAE_MAIN_H_
#define _GAME_MAIN_H_
#include <jni.h>
#include <EGL/egl.h>
#include "commond.h"
extern EGLSurface eglSurface;
extern EGLDisplay eglDisplay;

extern "C"{

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_onSurfaceChanged(JNIEnv* env,
                                                                    jobject thiz, jint w, jint h);

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_onSurfaceCreated(JNIEnv* env, jobject thiz);

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_update(JNIEnv * env, jobject obj, jint type);

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_exit(JNIEnv * env, jobject obj);

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_stop(JNIEnv * env, jobject obj);

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_resume(JNIEnv * env, jobject obj);

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_restartGame(JNIEnv * env, jobject obj);

//JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_setGravity(JNIEnv * env, jobject obj ,
//		jfloat x_, jfloat y_, jfloat z_);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_moveCameraFirstP(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_moveCamera(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_rotateCamera(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_rotateCameraLast(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setLeadRoleAction(JNIEnv* env,
		jobject thiz, jint code_);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_move(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_addLeadV(JNIEnv* env,
    jobject thiz, jfloat x, jfloat y, jfloat z);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setDirection(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchXY(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_CBTouchDown(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_CBTouchUp(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_CBTouchMove(JNIEnv* env,
                jobject thiz, jfloat x, jfloat z);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_sendPath(JNIEnv* env,
                jobject thiz, jstring string);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setGameMode(JNIEnv* env,
                jobject thiz, jint gameMode_);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setGameScene(JNIEnv* env,
                jobject thiz, jint gameScene_);

JNIEXPORT void Java_com_ly_game_fight_Native_readFromAssets
(JNIEnv* env,jclass thiz,jobject assetManager);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchDown(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchMove(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onTouchUp(JNIEnv* env,
                jobject thiz, jfloat x, jfloat y);

JNIEXPORT void Java_com_ly_game_fight_Native_setEGL
(JNIEnv* env,jclass thiz,jobject eglSurface_, jobject eglDisplay_);

JNIEXPORT jint JNICALL Java_com_ly_game_fight_Native_getSceneType(JNIEnv * env, jobject obj);


JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onButtonDown(JNIEnv* env,
		jobject thiz, jint code);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_onButtonUp(JNIEnv* env,
		jobject thiz, jint code);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_changeGameState(JNIEnv* env,
		jobject thiz);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_setLevelRecord(JNIEnv* env,
                jobject thiz, jint level_);

JNIEXPORT void JNICALL Java_com_ly_game_fight_Native_next(JNIEnv* env,
                jobject thiz);

}
#endif //_GAME_MAIN_H_
