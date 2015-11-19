package com.ly.game.fight;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.graphics.Paint.Style;
import android.graphics.RectF;
import android.util.AttributeSet;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnTouchListener;

public class ControlBall extends View implements OnTouchListener{
	
	float mW;
	float mH;
	static float x , y;
	float R=100;
	Paint paint = new Paint();
	RectF rect=new RectF(mW-R, mH-R,mW+R, mH+R);
	
	public ControlBall(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		setOnTouchListener(this);
	}
	
	public ControlBall(Context context, AttributeSet attrs) {
		super(context, attrs);
		// TODO Auto-generated constructor stub
	}
	
	public ControlBall(Context context, AttributeSet attrs, int defStyle) {
		super(context, attrs, defStyle);
		// TODO Auto-generated constructor stub
	}

	public void setWH(int w,int h)
	{
		mW=( (float)w )*0.5f;
		mH=( (float)h )*0.5f;
		R=mW-10;
		x=mW;
		y=mH;
	}
	
	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);
		//paint.setColor(Color.argb(200, 150, 100, 200));
		paint.setColor(Color.argb(100, 50, 255, 200));
		paint.setAntiAlias(true);
		//canvas.drawRect(0, 0, this.getWidth(), this.getHeight(), paint);
		canvas.drawCircle(mW, mH, mW/8, paint);
		
		canvas.drawCircle(x, y, mW/4, paint);
		//float halfl=200;
		
		rect.set(mW-R, mH-R,mW+R, mH+R);
		paint.setStyle(Style.FILL);
		//paint.setColor(Color.argb(200, 150, 60, 150));
		paint.setColor(Color.argb(50, 0, 0, 0));
		paint.setStrokeWidth(5);
		canvas.drawArc(rect, 0.0f, 360.0f, false, paint);
		
		//paint.setStrokeWidth(5);
		
		//canvas.drawLine(mW, mH, x, y, paint);
		
	}


	public boolean onTouch(View v, MotionEvent event) {
		// TODO Auto-generated method stub
		x=event.getX();
		y=event.getY();
		
		float lx=x-mW;
		float ly=y-mH;
		
		float l1=(float) Math.sqrt(lx*lx + ly*ly);
		//float l2=R;//halfl*halfl
		if(l1>R)
		{
			lx*=R/l1;
			ly*=R/l1;
			x=lx+mW;
			y=ly+mH;
		}
		//Log.d("lx,ly",String.valueOf(lx)+","+String.valueOf(-ly));
		/*
		double t=lx/l1; double a=Math.asin(t);
		if(-ly<0) a=Math.PI-a; a=a*180/Math.PI;
		*/
		switch(event.getAction())
		{
		case MotionEvent.ACTION_DOWN:
			Native.CBTouchDown(lx, ly);//(-ly, lx);
			this.invalidate();
			return true;
		case MotionEvent.ACTION_UP:
			Native.CBTouchUp(lx, ly);
			x=mW;
			y=mH;
			this.invalidate();
			return false;
		case MotionEvent.ACTION_MOVE:
			Native.CBTouchMove(lx, ly);
			this.invalidate();
			return false;
		}
		/*
		if(event.getAction()==MotionEvent.ACTION_DOWN)
		{
			Native.setDirection(lx, ly);
		}
		if(event.getAction()==MotionEvent.ACTION_UP)
		{
			
			Native.move(lx, ly);
			x=mW;
			y=mH;
		}else {
			//Native.setDirection(lx, ly);
		}
		*/
		//this.invalidate();
		
		return false;
	}
	
}
