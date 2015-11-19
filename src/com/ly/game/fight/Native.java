package com.ly.game.fight;

import javax.microedition.khronos.egl.EGLDisplay;
import javax.microedition.khronos.egl.EGLSurface;

import android.content.res.AssetManager;

public class Native {
	
	static{
		//System.loadLibrary("iconv");
		//System.loadLibrary("xml2");
		System.loadLibrary("devil");
		//System.loadLibrary("bullet");
		System.loadLibrary("game");
	}

	public static native int init();
	
	public static native int exit();
	
	public static native int stop();
	
	public static native int resume();
	
	public static native int restartGame();
	
	public static native int initScene(int w,int h);
	
	public static native int testNative();
	
	public static native int update(int type);
	
	public static native void setGravity(float x,float y,float z);
	
	public static native void moveCameraFirstP(float x, float y);

	public static native void moveCamera(float x, float y);

	public static native void rotateCamera( float x, float y);

	public static native void rotateCameraLast( float x, float y);
	
	public static native void setLeadRoleAction(int code);
	
	public static native int onSurfaceCreated();
	
	public static native int onSurfaceChanged(int w , int h);
	
	public static native void move(float x,float y);
	
	public static native void setDirection(float x, float y);
	
	public static native void CBTouchDown(float x, float y);
	
	public static native void CBTouchUp(float x, float y);
	
	public static native void CBTouchMove(float x, float y);
	
	public static native int getLeadState();
	
	/*
	 * add lead velocity
	 */
	public static native void addLeadV(float x, float y, float z);
	
	/*
	 * set lead velocity
	 */
	public static native void setLeadV(float x, float y, float z);
	
	public static native void sendPath(String string);
	
	public static native void addBall();
	
	public static native void setGameMode(int gameMode_);
	
	public static native void readFromAssets(AssetManager assetManager);
	
	public static native void onTouchDown(float x, float y);
	
	public static native void onTouchMove(float x, float y);
	
	public static native void onTouchUp(float x, float y);
	
	public static native void setEGL(EGLSurface mEglSurface,EGLDisplay mEglDisplay);

	public static native int getSceneType();
	
	public static native void onButtonDown(int code);
	
	public static native void onButtonUp(int code);
	
	public static native void changeGameState(int state);
	
	public static native void setLevelRecord(int level_);
	
	public static native void next();
	
}
