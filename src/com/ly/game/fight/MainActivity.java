package com.ly.game.fight;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import android.net.Uri;
import android.os.Bundle;
import android.os.Environment;
import android.os.Parcelable;
import android.app.Activity;
import android.content.ContentResolver;
import android.content.Context;
import android.content.Intent;
import android.content.res.AssetManager;
import android.content.res.Configuration;
import android.database.Cursor;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.drawable.BitmapDrawable;
import android.graphics.drawable.Drawable;
import android.util.Log;
import android.view.KeyEvent;
import android.view.LayoutInflater;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;
import android.view.View.OnClickListener;
import android.view.ViewGroup.LayoutParams;
import android.view.animation.Animation;
import android.view.animation.ScaleAnimation;
import android.view.animation.Animation.AnimationListener;
import android.widget.Button;

public class MainActivity extends Activity implements OnClickListener {	
	Button button_mTimer;
	Button button_levelMode;
	Button button_endlessMode;
	Button button_share;
	Button button_set;
	Button button_about;
	static String pagPathString;
	boolean isCreated=false;
	View child;
    @Override
    public void onCreate(Bundle savedInstanceState) {
    	
    	getWindow().setFlags(WindowManager.LayoutParams.FLAG_FULLSCREEN,
        		WindowManager.LayoutParams.FLAG_FULLSCREEN);
        requestWindowFeature(Window.FEATURE_NO_TITLE);

        super.onCreate(savedInstanceState);
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
        
        if(!isCreated) copyfile();
        isCreated=true;
       // Log.d("oncreate","oncreate");
        
        setContentView(R.layout.activity_main);
        AssetManager assetManager=getAssets();
        try {
        	//Bitmap bitmap=BitmapFactory.decodeStream(assetManager.open("gamepics/reef_north1.jpg"));
			getWindow().setBackgroundDrawable(Drawable.createFromStream(assetManager.open("gamepics/reef_north1.jpg"), ""));
		} catch (IOException e) {
			// TODO Auto-generated catch block
			e.printStackTrace();
		}
       // LayoutInflater inflater = LayoutInflater.from(this);  
      //  child = inflater.inflate(R.layout.activity_main, null); 
        //setContentView(R.layout.activity_main);
      //  addContentView(child,new LayoutParams(LayoutParams.MATCH_PARENT, LayoutParams.MATCH_PARENT));
        setConponent();
    }

    private void setConponent() {
		// TODO Auto-generated method stub
    	
    	//button_mTimer=(Button)findViewById(R.id.button_mtimer);
    	button_levelMode=(Button)findViewById(R.id.button_selectlevel);
    	button_levelMode.setBackgroundResource(R.drawable.button);
    	//button_set=(Button)findViewById(R.id.button_set);
    	//button_about=(Button)findViewById(R.id.button_about);
    	//button_mTimer.setOnClickListener(this);
    	button_levelMode.setOnClickListener(this);
    	//button_set.setOnClickListener(this);
    	//button_about.setOnClickListener(this);
    	
		//button_share=(Button)findViewById(R.id.button_share);
		//button_share.setBackgroundResource(R.drawable.button);
		//button_share.setOnClickListener(this);
    	
    	button_endlessMode=(Button)findViewById(R.id.button_demo);
    	button_endlessMode.setBackgroundResource(R.drawable.button);
    	//button_set=(Button)findViewById(R.id.button_set);
    	//button_about=(Button)findViewById(R.id.button_about);
    	//button_mTimer.setOnClickListener(this);
    	button_endlessMode.setOnClickListener(this);
	}

	public void onClick(View v) {
		// TODO Auto-generated method stub

		Intent intent;
		switch (v.getId()) {
		case R.id.button_selectlevel:
			intent=new Intent(this, GameActivity.class);
			intent.putExtra("type", 1);
			startActivity(intent);
			break;
		case R.id.button_demo:
			intent=new Intent(this, GameActivity.class);
			intent.putExtra("type", 2);
			startActivity(intent);
			break;
			//case R.id.button_set:
			//Native.init();
			//	break;
			//case R.id.button_about:

			//Native.exit();
			//	break;
		case R.id.button_share:
			intent=new Intent(Intent.ACTION_SEND);

			intent.setType("text/plain");
			intent.putExtra(Intent.EXTRA_SUBJECT, "share");
			intent.putExtra(Intent.EXTRA_TEXT, 
					"I'm playing Dodgeball-3D on my phone, https://play.google.com/store/apps/details?id=com.ly.dodgeball");
			intent.setFlags(Intent.FLAG_ACTIVITY_NEW_TASK);
			startActivity(Intent.createChooser(intent, getTitle()));
			break;
		default:
			break;
		}

	}
	
    public boolean onKeyDown(int keyCode, KeyEvent event) { 
		if (keyCode==KeyEvent.KEYCODE_BACK) {	
			//Native.exit();
			
			//if (!hasShortCut(this)) {
				//createDeskShortCut();
			//}
			
			try {
				Thread.sleep(50);
			} catch (InterruptedException e) {
				// TODO Auto-generated catch block
				e.printStackTrace();
			}
	    	System.exit(0);
	    	return true;
		}				   
	    return false;
	}
    
	public void createDeskShortCut() {

		Log.i("coder", "------createShortCut--------");
		// ������ݷ�ʽ��Intent
		Intent shortcutIntent = new Intent(
				"com.android.launcher.action.INSTALL_SHORTCUT");
		// �������ظ�����
		shortcutIntent.putExtra("duplicate", false);
		// ��Ҫ��ʵ�����
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_NAME,
				getString(R.string.app_name));

		// ���ͼƬ
		Parcelable icon = Intent.ShortcutIconResource.fromContext(
				getApplicationContext(), R.drawable.ic_launcher);

		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_ICON_RESOURCE, icon);

		Intent intent = new Intent(getApplicationContext(),
				MainActivity.class);
		// ��������������Ϊ�˵�Ӧ�ó���ж��ʱ�����ϵĿ�ݷ�ʽ��ɾ��
	    intent.setAction("android.intent.action.MAIN");
	    intent.addCategory("android.intent.category.LAUNCHER");
		// ������ͼƬ�����еĳ��������
		shortcutIntent.putExtra(Intent.EXTRA_SHORTCUT_INTENT, intent);
		// ���͹㲥��OK
		sendBroadcast(shortcutIntent);
	}
	
	public static boolean hasShortCut(Context context) {
		String url="";
		if(getSystemVersion()<8)
		{
			url="content://com.android.launcher.settings/favorites?notify=true";
		}else {
			url="content://com.android.launcher2.settings/favorites?notify=true";
		}
		ContentResolver resolver=context.getContentResolver();
		Cursor cursor=resolver.query(Uri.parse(url),
				null, "title=?", 
				new String[]{context.getString(R.string.app_name)}, 
				null);
		if(cursor != null && cursor.moveToFirst())
		{
			cursor.close();
			return true;
		}
		if(cursor!=null) cursor.close();
		return false;	
	}
	
	public static boolean hasShortCut2(Context ctx) {
		boolean isInstallShortcut = false;
		final ContentResolver cr = ctx.getContentResolver();
		final String AUTHORITY ;
		//��andriod 2.1��SDK7���ϣ��Ƕ�ȡlauncher.settings�е�favorites�����ݣ�
		//��andriod 2.2��SDK8���ϣ��Ƕ�ȡlauncher2.settings�е�favorites�����ݡ�
		if(getSystemVersion() < 8){ 
			AUTHORITY = "com.android.launcher.settings"; //
        }else{ 
        	AUTHORITY = "com.android.launcher2.settings";
        } 

		final Uri CONTENT_URI = Uri.parse("content://" + AUTHORITY
				+ "/favorites?notify=true");
		Cursor c = cr.query(CONTENT_URI,
				new String[] { "title", "iconResource" }, "title=?",
				new String[] { ctx.getString(R.string.app_name).trim() }, null);
		if (c != null && c.getCount() > 0) {
			c.close();
			isInstallShortcut = true;
		}
		if(c!=null) c.close();
		return isInstallShortcut;
	}

	public static int getSystemVersion() {
		return android.os.Build.VERSION.SDK_INT;
	}
	

    public void copyfile() {
    	try {
    		File file=new File(Environment.getExternalStorageDirectory()+"/Android/data/"+getPackageName());
    		pagPathString=file.getAbsolutePath();
    		//Native.sendPath(pagPathString);
    		
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

	
	
	public void copyfile0() {
		try {
			File floder = new File(Environment.getExternalStorageDirectory()+"/Android/data/"+getPackageName());
        	
        	if(!floder.exists())
        		floder.mkdirs();
//Log.d("file", "EXIST");
			String[] strings=getResources().getAssets().list("my");//.list("./assets/");
			//Log.d("file", Arrays.toString(strings));
			File file;
			for (int i = 0; i < strings.length; i++) {
				file=new File(floder+"/"+strings[i]);
				if(!file.exists())
				{
				InputStream iStream = getResources().getAssets().open("my/"+strings[i]);
				
				FileOutputStream fos= new FileOutputStream(file);
				
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
