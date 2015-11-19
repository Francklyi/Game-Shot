package com.ly.game.fight;

import javax.microedition.khronos.egl.EGLConfig;
import javax.microedition.khronos.opengles.GL10;

import android.opengl.GLSurfaceView.Renderer;
import android.util.Log;

public class MyRender implements Renderer{

	static float gx, gy, gz;
	static double fps;
	static long time0;
	static long timeNow;
	static long timeLength;
	static boolean isFirstDraw=true;
	
	public static boolean isRunning=true;
	
	public void setRunState(boolean isRunning_)
	{
		isRunning=isRunning_;
	}
	
	public void onDrawFrame(GL10 gl) {

//gl.GL_O
		//Log.d("draw frame","start");
		try {
				fps++;
				//Message msg= new Message();
				timeLength=(System.currentTimeMillis()-time0);
				//timeLength=timeLength>0?timeLength:1;
				
				if(timeLength>200)
				{
					GameActivity.fps=(fps/( (double)(timeLength+1) )*1000.0f );//+10L
					GameActivity.renewFPS();
					fps=0;
					timeLength=0;
					time0=System.currentTimeMillis();
				}
			
			
				//Thread.sleep(20);
			} catch (Exception e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
		
			if(!isRunning)
			{
				Native.update(0);
				return;
			}
			//if(isRunning)
				Native.update(1);
			


	}

	public void onSurfaceChanged(GL10 gl, int width, int height) {
		if(Native.onSurfaceChanged(width, height)!=0);
		//Log.d("changed","changed");

		Log.d("onSurfaceChanged", "1");
	}

	public void onSurfaceCreated(GL10 gl, EGLConfig config) {
		Log.d("onSurfaceCreated", "1");
		Native.sendPath(GameActivity.pagPathString+"/");
		
		isFirstDraw=false;
		fps=1;
		time0=System.currentTimeMillis();
		
		Native.readFromAssets(GameActivity.assetManager);
		if (Native.onSurfaceCreated()!=0);

		Native.setLevelRecord(GameActivity.game_level_record);
		
		GameActivity.startGame();
	}

}


/*
public int[] getConfigSpec() {
	// TODO Auto-generated method stub
    int[] configSpec = {
            EGL10.EGL_RED_SIZE,      8,
            EGL10.EGL_GREEN_SIZE,    8,
            EGL10.EGL_BLUE_SIZE,     8,
            EGL10.EGL_ALPHA_SIZE,    8,
            EGL10.EGL_DEPTH_SIZE,   16,
            EGL10.EGL_RENDERABLE_TYPE,4,
            EGL10.EGL_NONE
    };
    return configSpec;
}


public void surfaceCreated(GL10 gl) {
	// TODO Auto-generated method stub
	Log.d("onSurfaceCreated", "1");
	//Native.sendPath(GameActivity.pagPathString+"/");
	Native.readFromAssets(GameActivity.assetManager);
	if (Native.onSurfaceCreated()!=0);

}


public void sizeChanged(GL10 gl, int width, int height) {
	// TODO Auto-generated method stub
	if(Native.onSurfaceChanged(width, height)!=0);
	//Log.d("changed","changed");
	Log.d("onSurfaceChanged", "1");
}


public void drawFrame(GL10 gl) {
	// TODO Auto-generated method stub
	if (isFirstDraw) {
		isFirstDraw=false;
		fps=1;
		time0=System.currentTimeMillis();
	}
	else {
		fps++;
		Message msg= new Message();
		timeLength=(System.currentTimeMillis()-time0);
		//timeLength=timeLength>0?timeLength:1;
		if(timeLength>2000)
		{
			fps=0;
			timeLength=0;
			time0=System.currentTimeMillis();
		}
		msg.arg1=(int)(fps/( (double)(timeLength+1) )*1000 );//+10L
		GameActivity.handler1.sendMessage(msg);
	}
	
	try {
		//Thread.sleep(40);
	} catch (Exception e) {
		// TODO Auto-generated catch block
		e.printStackTrace();
	}
	//if(isRunning)
		Native.update();
	
	//Thread.sleep(100);
}
*/
