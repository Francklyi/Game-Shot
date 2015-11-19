package com.ly.game.fight;

import java.io.File;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.sql.Time;

import android.R.integer;
import android.annotation.SuppressLint;
import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.content.DialogInterface;
import android.content.DialogInterface.OnCancelListener;
import android.content.Intent;
import android.content.SharedPreferences;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.graphics.Color;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnSeekCompleteListener;
import android.media.SoundPool;
import android.media.SoundPool.OnLoadCompleteListener;
import android.opengl.GLSurfaceView;
import android.os.Bundle;
import android.os.Environment;
import android.os.Handler;
import android.util.Log;
import android.util.SparseIntArray;
import android.view.Gravity;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.MotionEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.view.View.OnTouchListener;
import android.view.ViewGroup.LayoutParams;
import android.view.Window;
import android.view.WindowManager;
import android.view.animation.Animation;
import android.view.animation.Animation.AnimationListener;
import android.view.animation.TranslateAnimation;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ProgressBar;
import android.widget.RelativeLayout;
import android.widget.TextView;

import com.google.android.gms.ads.AdRequest;
import com.google.android.gms.ads.AdSize;
import com.google.android.gms.ads.AdView;

@SuppressLint("InlinedApi")
public class GameActivity extends Activity 
implements OnTouchListener,OnClickListener{

	static MGLSurfaceView mglSurfaceView;
	static MyRender myRender;
	//SensorManager sensorManager;
	static TextView textView;
	//static TextView timeView;
	static TextView timeTextView;
	static TextView countTextView;
	Button button;
	static LinearLayout adLayout;
	//static GameOverLayout gameOverLayout;
	//GameOverActivity gameOverActivity;
	
	private static AdView mAdView;
	
	static long TIME=0,time=0;

	static int windowWidth=0, windowHeight=0;
	
	View controlView;
	int controlType=1;
	
	static long gameTIME=0,gameTime=0;
	Time time1=new Time(1);
	static int addBallTime=0;
	
	static Context context;
	
	/**
	 * set layout
	 */
	Button button_mTimer;
	Button button_mScores;
	
	Button button_demo;
	
	Button button_set;
	Button button_about;
	static String pagPathString;
	boolean isCreated=false;
	static RelativeLayout gameLayout;
	static View setLayout;
	static View gameoverLayout;
	/**
	 * game over layout
	 */
	Button button_restart;
	Button button_back;
	Button button_share;
	
	static View gameSuccessLayout;
	/**
	 * game fail layout
	 */
	Button button_next;
	Button button_back1;
	Button button_share1;
	
	static TextView scoresTextView;
	static TextView recordTextView;
	
	static double fps;
	
	static int gameMode;
	
	static final int GAMEMODE_LEVEL=0;
	static final int GAMEMODE_DEMO=1;
	
	static int numofCrystals;
	static int numofCrystalsAll;
	static int numofCoins;
	
	static SharedPreferences sharedPreferences;
	
	public static int game_level_record = 0;
	public static int game_level_choose = 0;
	public static long timer_record = 0;
	
	static SoundPool soundPool;
	
	static SparseIntArray soundPoolMap;
	
	static boolean playSound=true;
	
	static final int SE_CAPTURE=1;// sound effect capture coin
	static final int SE_BAD=2;
	
	
	static MediaPlayer mediaPlayer;
	
	static boolean isGameRunning;
	
	static ProgressBar progressBar;
	
	Button buttonSound;
	
	static Button button_jumph;
	static Button hit2Button;
	static Button hit3Button;
	static Button button_jumps;
	
	final int ID_HIT1=2001;
	final int ID_HIT2=2002;
	final int ID_HIT3=2003;
	final int ID_HIT4=2004;
	
	final int ID_JUMP1=4001;
	final int ID_JUMP_SHORT=4002;
	
	final int levelNumAll=10;
	
	static ControlBall controlBall;
	
	static int controlVisable=1;
	
	static AssetManager assetManager;
	
	int timeLeft=0;
	
	static int timeArray[]={
			80, 100, 150, 
			150, 150, 150,
			240, 150, 160,
			200, 160, 120,
			300, 300, 160, 160,
			130, 130, 160, 160,
			130, 130, 160, 160
	};
	
	static boolean isCountTime=false;
	
	static boolean playingMusic=true;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		// TODO Auto-generated method stub
		getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
        		WindowManager.LayoutParams.FLAG_FULLSCREEN);
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        WindowManager windowManager = (WindowManager)getSystemService(Context.WINDOW_SERVICE);
		windowWidth=windowManager.getDefaultDisplay().getWidth();
		windowHeight=windowManager.getDefaultDisplay().getHeight();
		super.onCreate(savedInstanceState);
		
		//setContentView(R.layout.game);
		gameMode=GAMEMODE_LEVEL;
		context=this;
		
		isGameRunning=false;
		numofCoins=0;
		numofCrystalsAll=0;
		numofCrystals=0;

		assetManager=getAssets();		
		
		copyfile();
		
		setPreferece();
		

		new Thread(loadSoundRunnable).start();
		
        setGLView();
        
        myRender.setRunState(true);
        
        setViews();
        
        //setSetLayout();
        
        setGameSuccessLayout();
        
        setGameOverLayout();
        //isGameRunning=false;
        setAdView();
        
        //sensorManager=(SensorManager)getSystemService(SENSOR_SERVICE);  

	}
	 
	public void onWindowFocusChanged(boolean hasFocus) {
	        super.onWindowFocusChanged(hasFocus);
	    /*   int version = Integer.valueOf(android.os.Build.VERSION.SDK_INT);
	       //Log.d("version",""+version);
	    if (hasFocus) {
	    	this.getWindow().getDecorView().setSystemUiVisibility(
	                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
	                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
	                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
	                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
	                | View.SYSTEM_UI_FLAG_FULLSCREEN
	                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
	                );}
	    */
	}
	
	static void startGame()
	{   
		playMusic();
		numofCoins=0;
		numofCrystals=0;
		myRender.setRunState(true);
		addBallTime=45;
		adLayout.setVisibility(View.INVISIBLE);
		//adLayout.removeView(mAdView);
		//gameTIME=System.currentTimeMillis()+timeArray[3]*1000;

		//isGameRunning=true;
		gameMode=GAMEMODE_LEVEL;
		Native.setGameMode(gameMode);
		
		Native.resume();
		//handler.postDelayed(runnable, 100);
		//handler.postDelayed(addBallRunnable, 2000);

		
        switch (gameMode) {
		case GAMEMODE_LEVEL:
			
			break;

		case GAMEMODE_DEMO:
			//handler.postDelayed(runnable, 100);
			break;
		}
        
	}
	private static final String AD_UNIT_ID = "ca-app-pub-6614426361022703/5840656677";
			//"ca-app-pub-6614426361022703/5840656677";
	public void setAdView() {
        mAdView = new AdView(this);
        mAdView.setAdUnitId(AD_UNIT_ID);
        mAdView.setAdSize(AdSize.BANNER);
        mAdView.setAdListener(new ToastAdListener(this));
        
        adLayout = new LinearLayout(this);
        adLayout.setGravity(Gravity.CENTER_HORIZONTAL|Gravity.TOP);
        adLayout.setOrientation(LinearLayout.VERTICAL);
        
        adLayout.addView(mAdView);

        addContentView(adLayout, new LayoutParams(
				LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        adLayout.setVisibility(View.INVISIBLE);

        handler.post(new Runnable() {

        	@Override
        	public void run() {
        		// TODO Auto-generated method stub
        		mAdView.loadAd(
        				new AdRequest.Builder()
        				//.addTestDevice(AdRequest.DEVICE_ID_EMULATOR)
        				//.addTestDevice("AA57B0FD4A3FDFCA8720058DA01453C9")
        				.build()
        				);
        	}
        });
	}
	
	public void setGLView() {
		mglSurfaceView=new MGLSurfaceView(this);
		//mglSurfaceView.setEGLContextClientVersion(2);
		//mglSurfaceView.setOnTouchListener(this);
		
		myRender=new MyRender();
		mglSurfaceView.setRenderer(myRender);
		
        addContentView(mglSurfaceView,new LayoutParams(
				LayoutParams.MATCH_PARENT,LayoutParams.MATCH_PARENT));
		
	}
	
	void setSetLayout()
	{
        LayoutInflater inflater = LayoutInflater.from(this);  
        setLayout = inflater.inflate(R.layout.activity_main, null); 
        //setContentView(R.layout.activity_main);
        addContentView(setLayout,new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
    	//button_mTimer=(Button)findViewById(R.id.button_mtimer);
    	button_mScores=(Button)findViewById(R.id.button_selectlevel);
    	//button_set=(Button)findViewById(R.id.button_set);
    	//button_about=(Button)findViewById(R.id.button_about);
    	
    	//button_mTimer.setBackgroundResource(R.drawable.button);
    	button_mScores.setBackgroundResource(R.drawable.button);
    	//button_set.setBackgroundResource(R.drawable.button);
    	//button_about.setBackgroundResource(R.drawable.button);
    	
    	//button_mTimer.setOnClickListener(this);
    	button_mScores.setOnClickListener(this);
    	//button_set.setOnClickListener(this);
    	//button_about.setOnClickListener(this);
    	
    	//button_set.setVisibility(View.INVISIBLE);button_demo
    	//button_about.setVisibility(View.INVISIBLE);
    	button_demo=(Button)findViewById(R.id.button_demo);
    	//button_set=(Button)findViewById(R.id.button_set);
    	//button_about=(Button)findViewById(R.id.button_about);
    	
    	//button_mTimer.setBackgroundResource(R.drawable.button);
    	button_demo.setBackgroundResource(R.drawable.button);
    	//button_set.setBackgroundResource(R.drawable.button);
    	//button_about.setBackgroundResource(R.drawable.button);
    	
    	//button_mTimer.setOnClickListener(this);
    	button_demo.setOnClickListener(this);
    	
	}

	void setGameOverLayout()
	{
        LayoutInflater inflater = LayoutInflater.from(this);  
        gameoverLayout = inflater.inflate(R.layout.gameover, null); 
        gameoverLayout.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				return true;
			}
		});
        //setContentView(R.layout.activity_main);
        addContentView(gameoverLayout,
        		new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        
        //scoresTextView=(TextView)findViewById(R.id.textView_scores);
        //recordTextView=(TextView)findViewById(R.id.textView_newrecord);
        
		button_restart=(Button)findViewById(R.id.button_restart);
		button_restart.setBackgroundResource(R.drawable.button);
		button_restart.setOnClickListener(this);
		
		button_back=(Button)findViewById(R.id.button_back);
		button_back.setBackgroundResource(R.drawable.button);
		button_back.setOnClickListener(this);
		
		button_share=(Button)findViewById(R.id.button_share);
		button_share.setBackgroundResource(R.drawable.button);
		button_share.setOnClickListener(this);
	
		gameoverLayout.setVisibility(View.INVISIBLE);
	}
	
	void setGameSuccessLayout()
	{
		
        LayoutInflater inflater = LayoutInflater.from(this);  
        gameSuccessLayout = inflater.inflate(R.layout.gamesuccess, null); 
        gameSuccessLayout.setOnTouchListener(new OnTouchListener() {
			
			@Override
			public boolean onTouch(View v, MotionEvent event) {
				// TODO Auto-generated method stub
				return true;
			}
		});
        //setContentView(R.layout.activity_main);
        addContentView(gameSuccessLayout,
        		new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        
        //scoresTextView=(TextView)findViewById(R.id.textView_scores);
        //recordTextView=(TextView)findViewById(R.id.textView_newrecord);
        
		button_next=(Button)findViewById(R.id.button_next);
		button_next.setBackgroundResource(R.drawable.button);
		button_next.setOnClickListener(this);
		
		button_back1=(Button)findViewById(R.id.button_back1);
		button_back1.setBackgroundResource(R.drawable.button);
		button_back1.setOnClickListener(this);
		
		button_share1=(Button)findViewById(R.id.button_share1);
		button_share1.setBackgroundResource(R.drawable.button);
		button_share1.setOnClickListener(this);
	
		gameSuccessLayout.setVisibility(View.INVISIBLE);
	}
	
	static RelativeLayout layout_controlBall;
	
	public void setViews() {
		        
        RelativeLayout.LayoutParams layoutParams=
				new RelativeLayout.LayoutParams(
						RelativeLayout.LayoutParams.WRAP_CONTENT,
						RelativeLayout.LayoutParams.WRAP_CONTENT);          

		LinearLayout ll = new LinearLayout(this);
        
        ll.setGravity(Gravity.LEFT|Gravity.TOP);
        ll.setOrientation(LinearLayout.VERTICAL);
        
        textView=new TextView(this);
        textView.setTextColor(Color.WHITE);
        textView.setTextSize(24);
        ll.addView(textView, 
				new LayoutParams(LayoutParams.WRAP_CONTENT, LayoutParams.WRAP_CONTENT));
        
        addContentView(ll, layoutParams);
        
        LayoutInflater inflater = LayoutInflater.from(this);  
        gameLayout = (RelativeLayout) inflater.inflate(R.layout.game, null); 
        //setContentView(R.layout.activity_main);
        addContentView(gameLayout,new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT)); 
        
        timeTextView=(TextView)findViewById(R.id.textView_time);
        timeTextView.setText("5:30");
        countTextView=(TextView)findViewById(R.id.textView_count);
        countTextView.setText(""+numofCrystals+"/"+numofCrystalsAll);


        layout_controlBall = new RelativeLayout(this);
        //layout_controlBall.setLayoutParams(layoutParams);
        //layout_controlBall.setGravity(Gravity.LEFT|Gravity.BOTTOM);
        //layout_controlBall.setBottom(10);            
        //layout_controlBall.setTop(300); 
        
        int w=windowWidth/4, h=windowWidth/4;
		controlBall=new ControlBall(this);
		controlBall.setWH(w, h);
		
		layoutParams.alignWithParent=true;
		//layoutParams.leftMargin=10;
		//layoutParams.bottomMargin=10;
		layoutParams.setMargins(5, windowHeight-h-5, 0, 5);
		layout_controlBall.addView(controlBall, new LayoutParams(w, h));//
		gameLayout.addView(layout_controlBall, layoutParams);
        
       // llaLayoutParams.setMargins(10 ,10, 20, 20);
        
        buttonSound=(Button)findViewById(R.id.button_yinxiao);//new Button(this);
        //buttonSound.setLayoutParams(llaLayoutParams);
        if (playingMusic) {
    		buttonSound.setBackgroundResource(R.drawable.yinxiao1);
    	}
    	else {
    		buttonSound.setBackgroundResource(R.drawable.yinxiao2);
		}
		
		buttonSound.setOnClickListener(new OnClickListener() {
			@Override
			public void onClick(View v) {
				// TODO Auto-generated method stub
		    	if (!playingMusic) {
		    		playingMusic=true;
		    		playSound=true;
		    		playMusic();
		    		buttonSound.setBackgroundResource(R.drawable.yinxiao1);
		    	}
		    	else {
		    		playingMusic=false;
		    		playSound=false;
					 stopPlayMusic();
		    		buttonSound.setBackgroundResource(R.drawable.yinxiao2);
				}
			}
		});
        
        //ll = new LinearLayout(this);
       // ll.setGravity(Gravity.RIGHT|Gravity.BOTTOM);
        //ll.setOrientation(LinearLayout.VERTICAL);
        //buttonSound.setPadding(10, 20, 20, 10);
        //ll.addView(buttonSound, llaLayoutParams);
        //addContentView(ll, layoutParams);
        
       // llaLayoutParams=
       // 		new LinearLayout.LayoutParams(
        //				LinearLayout.LayoutParams.WRAP_CONTENT, 
        //				LinearLayout.LayoutParams.WRAP_CONTENT);    
        
        button_jumph=(Button)findViewById(R.id.button_jumph);//new Button(this);
        //button_jumph.setId(ID_HIT1);
        //button_jumph.setText("Hit1");
        button_jumph.setOnTouchListener(this);


        button_jumps=(Button)findViewById(R.id.button_jumps);//new Button(this);
        //button_jumps.setId(ID_HIT4);
        //button_jumps.setText("Hit4");
        button_jumps.setOnTouchListener(this);
        //button_jumps.setOnClickListener(onClickListener1);
        //button_jumps.setBackgroundResource(R.drawable.button_b1);
        //ll.addView(button_jumps, llaLayoutParams);
        
        hit2Button=(Button)findViewById(R.id.button_shot1);
        hit2Button.setOnTouchListener(this);
        
		progressBar=(ProgressBar)findViewById(R.id.progressBar1);
		setProgressBar(0);
        
        controlBall.setOnTouchListener(controlBall);
        setViewsVisable(0);
	}
	
	public static void setViewsVisable(int v) {
		if(v==0)//invisable
		{
			layout_controlBall.setVisibility(View.INVISIBLE);
			button_jumph.setVisibility(View.INVISIBLE);

			button_jumps.setVisibility(View.INVISIBLE);
			hit2Button.setVisibility(View.INVISIBLE);
			timeTextView.setVisibility(View.INVISIBLE);
			countTextView.setVisibility(View.INVISIBLE);
		}else if(v==1)
		{
			layout_controlBall.setVisibility(View.VISIBLE);
			layout_controlBall.bringToFront();
			button_jumph.setVisibility(View.VISIBLE);

			button_jumps.setVisibility(View.VISIBLE);
			hit2Button.setVisibility(View.VISIBLE);
			timeTextView.setVisibility(View.VISIBLE);
			countTextView.setVisibility(View.VISIBLE);
		}
		
	}
	
	static Handler handler=new Handler();
	
	
	static int ProgressBarVisable=0;
	
	static Runnable runnable_progressBar=new Runnable() {

		@Override
		public void run() {
			//	Log.d("pvisable1 ",""+ProgressBarVisable);
			// TODO Auto-generated method stub
			if(ProgressBarVisable==0)
			{
				Log.d("pvisable",""+ProgressBarVisable);
				progressBar.setVisibility(View.INVISIBLE);
				
			}
			else if(ProgressBarVisable==1)
			{
				Log.d("pvisable",""+ProgressBarVisable);
				progressBar.setVisibility(View.VISIBLE);
				progressBar.bringToFront();
			}
		}
	};
	
	public void setProgressBar(int visable) {
		ProgressBarVisable=visable;
		//Log.d("pvisable",""+ProgressBarVisable);
		handler.post(runnable_progressBar);
	}
	
	public void setPreferece()//playingMusic
	{
		//Log.e("level1",""+0);
		sharedPreferences=getSharedPreferences("user_record", Activity.MODE_PRIVATE);
		SharedPreferences.Editor editor = sharedPreferences.edit();
		
		game_level_record=sharedPreferences.getInt("level", 1);
		int gameLevelMin=10;
		if(game_level_record<gameLevelMin)
		 {
			editor.putInt("level", gameLevelMin);
			game_level_record=gameLevelMin;
			editor.commit();
		}else if(game_level_record>levelNumAll){
			editor.putInt("level", levelNumAll);
			game_level_record=levelNumAll;
			editor.commit();
		}
		//Log.e("level",""+game_level_record);
		playingMusic=sharedPreferences.getBoolean("music", true);
		playingMusic=false;
	}
	
	public static Handler handler1=new Handler()
	{
		public void handleMessage(android.os.Message msg) 
		{
			textView.setText((""+fps+"00000").substring(0, 5));
		};
	};
    
	static Runnable runnable_fps=new Runnable() {
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			
		}
	};
	
	public static void renewFPS() {
		handler1.sendEmptyMessage(0);
		//handler.post(runnable_fps);
	}
	
    static Runnable runnable=new Runnable(){
    public void run() {

    	//gameTime=System.currentTimeMillis();

    	//gameTime=gameTIME-gameTime;
    	if (!isCountTime) {
			return;
		}
    	gameTime--;
    	
    	if(gameTime<0)
    	{
    		gameFail();
    	}
    	else {
    		//if((float)gameTime/1000.0f==0.0f) 
    		timeTextView.setText(""+gameTime/60+" : "+gameTime%60);
    		//timeView.setText(""+gameTime/1000+" : "+gameTime%1000);//((gameTime-gameTIME)/1000)
    		//if(myRender.isRunning)
    		handler.postDelayed(this, 1000);	
		}

    }
    };
	
    static Runnable addBallRunnable=new Runnable() {
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			if(myRender.isRunning)
			{
				//Native.addBall();
				
			}
    		//handler.postDelayed(this, 5000);	
		}
	};
	
	static Runnable showScoreRunnable=new Runnable() {
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			//playSoundE(SE_CAPTURE);
			if(playSound)
			soundPool.play(soundPoolMap.get(SE_CAPTURE), 0.1f, 0.1f, 0, 0, 1);
			//numofCoins++;
			//timeView.setText(""+numofCoins);
		}
	};
	
	Runnable successRunnable=new Runnable() {
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			stopPlayMusic();
		}
	};
	
	static Runnable beforeLLoadRunnable=new Runnable() {
		public void run() {
			//Log.d("bell", "1");
			progressBar.setVisibility(View.VISIBLE);
			progressBar.bringToFront();
		}
	};
	
	static Runnable afterLLoadRunnable=new Runnable() {
		public void run() {
			progressBar.setVisibility(View.INVISIBLE);
	        //timeTextView.setText("5:30");
	        countTextView.setText(""+numofCrystals+"/"+numofCrystalsAll);
			setViewsVisable(1);
			isGameRunning=true;

			isCountTime=true;
			myRender.setRunState(true);
			handler.removeCallbacks(runnable);
			handler.post(runnable);
		}
	};
	
	public void setControlVisable(int v) {
		controlVisable=v;
		//Log.d("setCV","1111");
		handler.post(run_setControlVisable);
	}
	
	static Runnable run_setControlVisable=new Runnable() {
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			setViewsVisable(controlVisable);
		}
	};
	
	public void beforeLayoutLoad(int level_)
	{
		game_level_choose=level_;
		handler.post(beforeLLoadRunnable);
	}
	
	public void afterLayoutLoad(int num_,int level_)
	{
		numofCrystalsAll=num_;
		numofCrystals=0;
		game_level_choose=level_-1;
		gameTIME=timeArray[game_level_choose];
		gameTime=gameTIME;
		handler.post(afterLLoadRunnable);
	}
	
    public static void gameFail() {
    	isGameRunning=false;
    	myRender.setRunState(false);
    	isCountTime=false;
    	//handler.removeCallbacks(addBallRunnable);
    	//adLayout.addView(mAdView);
    	
    	handler2.sendEmptyMessageDelayed(0, 100);
    }
	
    public void gameSuccess(int i) {
    	
    	Log.d("gameSuccess","0");
		SharedPreferences.Editor editor = sharedPreferences.edit();
		if(i==game_level_record)
			game_level_record++;
		if(game_level_record>levelNumAll)
			game_level_record=levelNumAll;
		Log.d("level_record", ""+game_level_record);
		editor.putInt("level", game_level_record);
		editor.commit();
    	
    	if(isGameRunning)
    	{
    		Log.d("gameSuccess","1");
    		isGameRunning=false;
    		myRender.setRunState(false);
    		isCountTime=false;
    		//handler.removeCallbacks(addBallRunnable);
    		//adLayout.addView(mAdView);
    		handler_success.sendEmptyMessageDelayed(0, 100);
    	}
    }
    
    public void captureCoin() {
    	//Log.d("captureCoin", ""+numofCoins);
    	handler.post(showScoreRunnable);
	}
    
    public static void playSoundE(final int mpId) {

    	if ( playSound ) {
    		soundPool.play(soundPoolMap.get(mpId), 1, 1, 0, 0, 1);
    	}
    }

    Runnable loadSoundRunnable=new Runnable() {
		
		@Override
		public void run() {
			// TODO Auto-generated method stub
			soundPool = new SoundPool(20, AudioManager.STREAM_MUSIC, 0);
			
			soundPool.setOnLoadCompleteListener(new OnLoadCompleteListener() {
				
				@Override
				public void onLoadComplete(SoundPool soundPool, int sampleId, int status) {
					// TODO Auto-generated method stub
				Log.d("id", String.valueOf(sampleId));	
				}
			});
			
			soundPoolMap = new SparseIntArray();
			
			soundPoolMap.put(SE_CAPTURE, soundPool.load(context, R.raw.capture, 1));
			soundPoolMap.put(SE_BAD, soundPool.load(context, R.raw.bad, 1));
			
			// set background music
			prepareBGMusic();
		}
	};
    
	public void prepareBGMusic() {
    	try {
    	if (mediaPlayer==null) {	
    		mediaPlayer=MediaPlayer.create(this, R.raw.bg1);
    		//Log.d("MediaPlayer", "MediaPlayer");
    		if (mediaPlayer!=null) {
    		    mediaPlayer.stop();
    		}
		}    
    	
    	mediaPlayer.setOnSeekCompleteListener(new OnSeekCompleteListener() {
			
			@Override
			public void onSeekComplete(MediaPlayer mp) {
				// TODO Auto-generated method stub
				
			}
		});
    	
	    mediaPlayer.setOnCompletionListener(new OnCompletionListener() {
			
			@Override
			public void onCompletion(MediaPlayer mp) {
				// TODO Auto-generated method stub
				mp.setLooping(true);
				mp.start();
			}
		});		
	    
	    mediaPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
	    //mediaPlayer.set
	    mediaPlayer.prepare();
	    playMusic();
	}  catch (Exception e) {
	    // TODO Auto-generated catch block
	    e.printStackTrace();
	}
	}
	
    public static void playMusic(){ 	
    	if (playingMusic&&mediaPlayer!=null) {
    		if(!mediaPlayer.isPlaying())
				mediaPlayer.start();
		}
    }
	
    public static void stopPlayMusic() {
    	if (mediaPlayer!=null) {
    		if(mediaPlayer.isPlaying())
    			mediaPlayer.pause();
		}
	}
	
    public static void releaseMusicPlayer() {
    	if (mediaPlayer!=null) {
		    mediaPlayer.stop();
		    mediaPlayer.reset();
		    mediaPlayer.release();
		    mediaPlayer=null;
		}
	}
    
	public static Handler handler2=new Handler()
	{
		public void handleMessage(android.os.Message msg) 
		{
			stopPlayMusic();
			/*playSoundE(SE_BAD);
			recordTextView.setVisibility(View.INVISIBLE);
    		sharedPreferences=context.getSharedPreferences("user_record", Activity.MODE_PRIVATE);
    		SharedPreferences.Editor editor = sharedPreferences.edit();
    		
    		
	    	if(gameMode==GAMEMODE_LEVEL)
	    	{
	    		
	    		game_level_record=sharedPreferences.getInt("game_level_record", -1);
	    		if(game_level_record<numofCoins)
	    		{
	    			recordTextView.setVisibility(View.VISIBLE);
	    			editor.putInt("game_level_record", numofCoins);
	    			game_level_record=numofCoins;
	    			editor.commit();
	    			Toast.makeText(context, "new record!", Toast.LENGTH_SHORT).show();
	    		}
	    		
	    		scoresTextView.setText(""+numofCoins);
	    	}else {
	    		gameTime=System.currentTimeMillis();

	        	gameTime-=gameTIME;
		
	    		timer_record=sharedPreferences.getLong("timer_record", -1);
	    		if(timer_record<gameTime)
	    		{
	    			recordTextView.setVisibility(View.VISIBLE);
	    			editor.putLong("timer_record", gameTime);
	    			timer_record=gameTime;
	    			editor.commit();
	    			Toast.makeText(context, "new record!", Toast.LENGTH_SHORT).show();
	    		}
	    		scoresTextView.setText(""+gameTime/1000+" : "+gameTime%1000);
			}
	    	*/
			startGameoverLayout();
		};
	};
	
	public static Handler handler_success=new Handler()
	{
		public void handleMessage(android.os.Message msg) 
		{
			Log.d("handler_success","1");
			stopPlayMusic();
			startGameSuccessLayout();
		};
	};
	
	protected void onPause() {
		mAdView.pause();
		super.onPause();
	};
	
	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
		mglSurfaceView.onResume();
		playMusic();
		mAdView.resume();
		//sensorManager.registerListener(this, 
				//sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
				//SensorManager.SENSOR_DELAY_GAME);
		// trun off sensor
		//sensorManager.unregisterListener(this);
	}
	
	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		mAdView.destroy();
		super.onDestroy();
	}
	
	@Override
	protected void onActivityResult(int requestCode, int resultCode, Intent data) {
		// TODO Auto-generated method stub
		super.onActivityResult(requestCode, resultCode, data);
		switch (requestCode) {
		case 1:
			myRender.setRunState(true);
			addBallTime=45;
			//adLayout.setVisibility(View.INVISIBLE);
			//adLayout.removeView(mAdView);
			//gameTIME=System.currentTimeMillis()+timeArray[3]*1000;
			//handler.postDelayed(runnable, 100);
			Native.resume();
			break;

		default:
			break;
		}
	}
	
	@Override
	protected void onStop() {
		// TODO Auto-generated method stub
		super.onStop();
		stopPlayMusic();
		
		//sensorManager.unregisterListener(this);
	}
	

	final int SCENETYPE_LEVELS=1;
	final int SCENETYPE_GAME=2;
	
	void back()
	{
		gameoverLayout.setVisibility(View.INVISIBLE);
		gameSuccessLayout.setVisibility(View.INVISIBLE);
		adLayout.setVisibility(View.INVISIBLE);
		
		isCountTime=false;
		final int sceneType=Native.getSceneType();

		// TODO Auto-generated method stub					
		try {
			Thread.sleep(20);
			myRender.setRunState(false);	
			if(Native.exit()==1);

			if(sceneType==SCENETYPE_GAME)
			{
				controlVisable=0;
				setViewsVisable(controlVisable);
			}else 
				if(sceneType==SCENETYPE_LEVELS)
				{
					Log.d("exit","exit");
					SharedPreferences.Editor editor = sharedPreferences.edit();
					
					editor.putBoolean("music", playingMusic);
					editor.commit();
					releaseMusicPlayer();
					System.exit(0);
				}

		} catch (Exception e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
	}
	
	public void gameResume() {
		isCountTime=true;
		handler.removeCallbacks(runnable);
		handler.post(runnable);
		Native.resume();
	}
	
	public void exitShow()
	{
		final int sceneType=Native.getSceneType();
		//Log.d("st",""+sceneType);
		String titleString = null;
		if(sceneType==SCENETYPE_LEVELS)
			titleString="EXIT ?";
		else if(sceneType==SCENETYPE_GAME)
			titleString="BACK ?";
		
		new AlertDialog.Builder(this).setTitle(titleString)
		.setPositiveButton("YES", new DialogInterface
				.OnClickListener() {
					
					@Override
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub					
						back();
					}
				}).setNegativeButton("NO", new DialogInterface
				.OnClickListener() {
					public void onClick(DialogInterface dialog, int which) {
						// TODO Auto-generated method stub
						myRender.setRunState(true);
						if(isGameRunning)
						{Log.d("isGameRunning",""+sceneType);
							playMusic();
							gameResume();
						}
					}
				}).setOnCancelListener(new OnCancelListener() {
					public void onCancel(DialogInterface dialog) {
						// TODO Auto-generated method stub
						myRender.setRunState(true);
						if(isGameRunning)
						{
							playMusic();
							gameResume();
						}
					}
				}).show();
	}
	
	public boolean onTouch(View v, MotionEvent event) {
		// TODO Auto-generated method stub
		//String string = "";
		//Log.d("OPENGL es touch",""+event.getAction());
		// TODO Auto-generated method stub	
		if(event.getAction()==MotionEvent.ACTION_DOWN)
		{
			switch (v.getId()) {
			case R.id.button_jumph:
				//Log.d("ID_HIT1","ID_HIT1");
				Native.onButtonDown(ID_JUMP1);
				break;

			case R.id.button_jumps:
				Native.onButtonDown(ID_JUMP_SHORT);//ID_HIT4
				break;
			case R.id.button_shot1:
				Native.onButtonDown(ID_HIT1);
				break;
			}
		}else
			if (event.getAction()==MotionEvent.ACTION_UP) 
			{
				switch (v.getId()) {
				case R.id.button_jumph:
					//Log.d("ID_HIT1","ID_HIT1");
					Native.onButtonUp(ID_JUMP1);
					break;

				case R.id.button_jumps:
					Native.onButtonUp(ID_JUMP_SHORT);//ID_HIT4
					break;
				case R.id.button_shot1:
					Native.onButtonUp(ID_HIT1);
					break;
				}
			}
		
		return false;
	}
	
    public boolean onKeyDown(int keyCode, KeyEvent event) { 
		if (keyCode==KeyEvent.KEYCODE_BACK) {
			if(isGameRunning)
				stopPlayMusic();
			myRender.setRunState(false);
			isCountTime=false;
			//handler.removeCallbacks(runnable);
			//Native.stop();
			exitShow();
	    	return true;
		}				   
	    return false;
	}

    public static void startGameoverLayout() {
    	
    	if(gameoverLayout.isShown()) return;
    	
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, windowHeight, 0);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				
				gameoverLayout.setVisibility(View.VISIBLE);
				gameoverLayout.setActivated(true);
				gameoverLayout.bringToFront();
				//mAdView.setVisibility(View.VISIBLE);
				adLayout.setVisibility(View.VISIBLE);
				adLayout.bringToFront();
			}
		});
    	
    	gameoverLayout.startAnimation(translateAnimation);
	}
    
    static Handler restartHandler=new Handler(){
    	public void handleMessage(android.os.Message msg) {
    		Native.restartGame();
    	};
    };
    
    public static void closeGameoverLayout() {
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, 0, windowHeight);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				gameoverLayout.setVisibility(View.INVISIBLE);
				
				myRender.setRunState(true);
				playMusic();
				//mAdView.setVisibility(View.INVISIBLE);
				//Native.restartGame();
				//startGame();
				//restartHandler.sendEmptyMessageDelayed(0, 100);
			}
		});
    	
    	gameoverLayout.startAnimation(translateAnimation);
	}
    
    public static void startGameSuccessLayout() {
    	
    	if(gameSuccessLayout.isShown()) return;
    	
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, windowHeight, 0);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				
				gameSuccessLayout.setVisibility(View.VISIBLE);
				gameSuccessLayout.setActivated(true);
				gameSuccessLayout.bringToFront();
				gameSuccessLayout.refreshDrawableState();
				adLayout.setVisibility(View.VISIBLE);
				adLayout.bringToFront();
			}
		});
    	
    	gameSuccessLayout.startAnimation(translateAnimation);
	}
    
    public static void closeGameSuccessLayout() {
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, 0, windowHeight);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				myRender.setRunState(true);
				gameSuccessLayout.setVisibility(View.INVISIBLE);
				playMusic();
				//mglSurfaceView.getHolder().notify();
				//startGame();
				//beforeLayoutLoad()
				//Native.next();
			}
		});
    	
    	gameSuccessLayout.startAnimation(translateAnimation);
	}
    
    public static void closeGameSuccessLayoutAndNext() {
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, 0, windowHeight);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				myRender.setRunState(true);
				gameSuccessLayout.setVisibility(View.INVISIBLE);
				playMusic();
				//mglSurfaceView.getHolder().notify();
				//startGame();
				//beforeLayoutLoad()
				Native.next();
			}
		});
    	
    	gameSuccessLayout.startAnimation(translateAnimation);
	}
    
    public static void closeGameOverLayoutAndRestart() {
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, 0, windowHeight);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				myRender.setRunState(true);
				gameoverLayout.setVisibility(View.INVISIBLE);
				playMusic();
				//mglSurfaceView.getHolder().notify();
				//startGame();
				//beforeLayoutLoad()
				Native.restartGame();
			}
		});
    	
    	gameoverLayout.startAnimation(translateAnimation);
	}
    
    public void startSetLayout() {
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, windowHeight, 0);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				setLayout.setVisibility(View.VISIBLE);
				//handler.postDelayed(addBallRunnable, 2000);
			}
		});
    	
		setLayout.startAnimation(translateAnimation);
	}
    
    public void clooseSetLayout() {
    	TranslateAnimation translateAnimation=new TranslateAnimation(0, 0, 0, windowHeight);
    	translateAnimation.setDuration(300);
    	translateAnimation.setAnimationListener(new AnimationListener() {
			
			@Override
			public void onAnimationStart(Animation animation) {}
			
			@Override
			public void onAnimationRepeat(Animation animation) {}
			
			@Override
			public void onAnimationEnd(Animation animation) {
				// TODO Auto-generated method stub
				setLayout.setVisibility(View.INVISIBLE);
				adLayout.setVisibility(View.INVISIBLE);
				//startGame();
				//handler.postDelayed(addBallRunnable, 2000);
			}
		});
    	
		setLayout.startAnimation(translateAnimation);
	}
    
    public void addCrystalNum()
    {
    	numofCrystals++;
    	//Log.d("add crystal 0",""+numofCrystals);
    	handler.post(new Runnable() {
			
			@Override
			public void run() {
				//Log.d("add crystal 1",""+numofCrystals);
				// TODO Auto-generated method stub
				countTextView.setText(""+numofCrystals+"/"+numofCrystalsAll);
			}
		});
    	
    }
    
	public void onClick(View v) {
		// TODO Auto-generated method stub
		//Log.d("onClick","onClick");
		switch (v.getId()) {
		case R.id.button_jumph:
			//Log.d("ID_HIT1","ID_HIT1");
			Native.setLeadRoleAction(ID_JUMP1);
			break;

		case R.id.button_jumps:
			Native.setLeadRoleAction(ID_HIT4);
			break;
			
		case 201://stop
			Log.d("stop","stop");
			Native.stop();
			break;
			
		case 202://resume
			Log.d("resume","resume");
			//myRender.setRunState(true);
			//gameTIME=System.currentTimeMillis();
			//handler.postDelayed(runnable, 100);
			//Native.resume();
			break;
		case R.id.button_selectlevel:
			isGameRunning=true;
			gameMode=GAMEMODE_LEVEL;
			Native.setGameMode(gameMode);
			clooseSetLayout();
			Log.d("start", "1");
			playMusic();
			break;
		case R.id.button_demo:
			isGameRunning=true;
			gameMode=GAMEMODE_DEMO;
			Native.setGameMode(gameMode);
			clooseSetLayout();
			Log.d("start", "1");
			playMusic();
			break;
		//case R.id.button_set:
			//Native.init();
		//	break;
		//case R.id.button_about:

			//Native.exit();
		//	break;
			
		case R.id.button_restart:
			adLayout.setVisibility(View.INVISIBLE);
			isGameRunning=true;
			closeGameOverLayoutAndRestart();
			
			//gameoverLayout.setVisibility(View.INVISIBLE);
			playMusic();
			break;
		case R.id.button_back:
			//gameoverLayout.setVisibility(View.INVISIBLE);
			adLayout.setVisibility(View.INVISIBLE);
			//startSetLayout();
			closeGameoverLayout();
			back();
			break;
		case R.id.button_back1:
			//gameSuccessLayout.setVisibility(View.INVISIBLE);
			adLayout.setVisibility(View.INVISIBLE);
			//startSetLayout();
			closeGameSuccessLayout();
			back();
			break;
		case R.id.button_next:
			adLayout.setVisibility(View.INVISIBLE);
			//gameSuccessLayout.setVisibility(View.INVISIBLE);
			//startSetLayout();
			closeGameSuccessLayoutAndNext();
			//beforeLayoutLoad();

			break;
		case R.id.button_share:
		case R.id.button_share1:
			Intent intent=new Intent(Intent.ACTION_SEND);
		      
		      intent.setType("text/plain");
		      intent.putExtra(Intent.EXTRA_SUBJECT, "share");
		      intent.putExtra(Intent.EXTRA_TEXT, 
		    		  "I'm playing game on my phone, https://play.google.com/store/apps/details?id=com.ly.game.fight");
		      intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
		      startActivity(Intent.createChooser(intent, getTitle()));
			break;
		}
		//Native.moveCamera(wx1, wz1);
	}

    public void copyfile() {
    	try {
    		File file=new File(Environment.getExternalStorageDirectory()+"/Android/data/"+getPackageName());
    		pagPathString=file.getAbsolutePath();
    		//Native.sendPath(pagPathString+"/");
    		if(true) return;
    		
    		if(!file.exists()) file.mkdirs();  
    		else {
				return;
			}
    		
    		Log.d("file", pagPathString);
    		file=new File(pagPathString+"/files");
    		if(!file.exists()) file.mkdirs();
    		
    		String pathString=pagPathString+"/files";
    		
    		String[] strings=getResources().getAssets().list("my");//.list("./assets/");
    		
    		for (int i = 0; i < strings.length; i++) {
    			file=new File(pathString+"/"+strings[i]);
    			if(!file.exists()) file.mkdirs();
    			String[] strings1=getResources().getAssets().list("my/"+strings[i]);
    			for (int j = 0; j < strings1.length; j++) {
    				
    				InputStream iStream = getResources().getAssets().open("my/"+strings[i]+"/"+strings1[j]);

    				FileOutputStream fos= new FileOutputStream(file.getAbsolutePath()+"/"+strings1[j]);
    				byte[] buffer= new byte[8192];
    				int count = 0;
    				while((count = iStream.read(buffer))>=0)
    				{
    					fos.write(buffer,0,count);
    				}
    				fos.close();
    				iStream.close();
    				
				}

			}
    	} catch (Exception e) {
    		// TODO Auto-generated catch block
    		e.printStackTrace();
    	}
    }
		
    public void onConfigurationChanged(Configuration newConfig) {
		 try {
			 super.onConfigurationChanged(newConfig);
			 if (this.getResources().getConfiguration().orientation ==
					 Configuration.ORIENTATION_LANDSCAPE) {
			 // land
			 } else if (this.getResources().getConfiguration().orientation == 
					 Configuration.ORIENTATION_PORTRAIT) {
			 // port
			 }
		} catch (Exception ex) {
	}

	}
    

}
