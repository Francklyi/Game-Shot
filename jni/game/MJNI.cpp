#include "MJNI.h"
#include "log.h"

MJNI *pMjni=new MJNI();

MJNI::MJNI()
:mEnv(0)
{

}

void MJNI::init(JNIEnv *mEnv_)
{
	mEnv=mEnv_;
	mEnv->GetJavaVM(&mJavaVM);

	if ( !mJavaVM) {
		return;
	}

	mJavaVM->AttachCurrentThread (&mEnv, 0); // (void **)
}

void MJNI::captureCoin()
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = mEnv->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = mEnv->GetMethodID(jNativesCls
			, "captureCoin"
			, "()V");

	if (mid) {
		mEnv->CallVoidMethod(obj , mid);
	}
}

void MJNI::success(int level)
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "gameSuccess"
			, "(I)V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj , mid, level);
	}
}

void MJNI::setProgressBar(int visable)
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "setProgressBar"
			, "(I)V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj , mid,visable);
	}
}

void MJNI::setControlVisable(int visable)
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "setControlVisable"
			, "(I)V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj , mid,visable);
	}
}


void MJNI::beforeLayoutLoad(int level_)
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "beforeLayoutLoad"
			, "(I)V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj , mid , level_);
	}
}

void MJNI::afterLayoutLoad(int num_,int level_)
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "afterLayoutLoad"
			, "(II)V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj , mid ,num_,level_);
	}
}

void MJNI::fail()
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "gameFail"
			, "()V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj , mid);
	}
}

void MJNI::restart()
{

}

void MJNI::toGameScene(int sceneNum)
{
	beforeLayoutLoad(sceneNum);
	pGame->load(sceneNum);//id
}

void MJNI::addCrystalNum()
{
	jclass jNativesCls;

	if ( !jNativesCls ) {
		jNativesCls = (mEnv)->FindClass("com/ly/game/fight/GameActivity");

	}

	if ( jNativesCls == 0 ) {
		return;
	}

	jobject obj = mEnv->AllocObject(jNativesCls);

	jmethodID mid = (mEnv)->GetMethodID(jNativesCls
			, "addCrystalNum"
			, "()V");

	if (mid) {
		(mEnv)->CallVoidMethod(obj , mid);
	}
}
