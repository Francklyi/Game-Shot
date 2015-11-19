package com.ly.game.fight;

import android.content.Context;
import android.graphics.Canvas;
import android.graphics.Color;
import android.graphics.Paint;
import android.view.MotionEvent;
import android.widget.Button;

public class MyButton extends Button{

	Paint paint;
	
	public MyButton(Context context) {
		super(context);
		// TODO Auto-generated constructor stub
		paint=new Paint();
		paint.setColor(Color.argb(0, 0, 0, 0));
		paint.setAntiAlias(true);
	}

	@Override
	protected void onDraw(Canvas canvas) {
		// TODO Auto-generated method stub
		super.onDraw(canvas);

		canvas.drawCircle(this.getWidth()/2, this.getHeight()/2, this.getHeight()/2, paint);
		//canvas.drawRect(5, 5, this.getWidth()-5, this.getHeight()-5, paint);	
	}
	
	public boolean onTouchEvent(MotionEvent event) {
		if(event.getAction()==MotionEvent.ACTION_DOWN
				||event.getAction()==MotionEvent.ACTION_HOVER_MOVE){
		//this.setBackgroundColor(Color.argb(255, 175,238,238));
		paint.setColor(Color.argb(190, 0, 191, 255));
		this.invalidate();
		}else
			if (event.getAction()==MotionEvent.ACTION_UP) {
				//this.setBackgroundColor(Color.BLACK);
				paint.setColor(Color.argb(0, 0, 0, 0));
				this.invalidate();
			}
		return super.onTouchEvent(event);
	    }
}
