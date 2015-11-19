#pragma once
#include <jni.h>
#include "commond.h"

class MJNI
{
public:
	JNIEnv *mEnv;
	jobject mJObject;
	JavaVM *mJavaVM;

	MJNI();

	void init(JNIEnv *mEnv_);

	void captureCoin();

	void success(int level);

	void fail();

	void restart();

	void setProgressBar(int visable);

	void setControlVisable(int visable);

	void beforeLayoutLoad(int level_);

	void afterLayoutLoad(int num_,int level_);

	void toGameScene(int sceneNum);

	void addCrystalNum();

};

extern MJNI *pMjni;
