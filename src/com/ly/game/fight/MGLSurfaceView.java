package com.ly.game.fight;


import android.content.Context;
import android.opengl.GLSurfaceView;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class MGLSurfaceView extends GLSurfaceView implements OnTouchListener{
	
	public MGLSurfaceView(Context context) {
		super(context);
		setEGLContextClientVersion(2);
		setOnTouchListener(this);
		
	}

	@Override
	public boolean onTouch(View view, MotionEvent event) {
		// TODO Auto-generated method stub
		switch (event.getAction()) {
		case MotionEvent.ACTION_DOWN:
			Native.onTouchDown(event.getX(), event.getY());
			
			return true;
		case MotionEvent.ACTION_MOVE:
			Native.onTouchMove(event.getX(), event.getY());
			break;
		case MotionEvent.ACTION_UP:
			Native.onTouchUp(event.getX(), event.getY());
			break;
		default:
			break;
		}
		
		return false;
	}

}
