//static jmethodID mSendStr;
#include "mygame1jni.h"
//#include "map.h"
#include "leadingRole.h"
#include "skyBox.h"
#include "terrain.h"
#include "conBall.h"

static jclass jNativesCls;
static JavaVM *g_VM;

FILE * fp;
char chars[100];
int words;
float * flp;

static float Z= 0.0f , Horizon=2.0f;
static int FIRST=0;
static Camera camera;

static bool rotating=false;

vector<Entity *> entityVector;
int W=0, H=0;
Entity entity;
Entity entity2;
Entity entity3;
LeadingRole leadingRole;
SkyBox skyBox;
ConBall conBall;
char fileName4[]="./sdcard/terrain/map1-1.lmap";
Terrain terrain=Terrain(fileName4);

static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}

static void init_scene(int w, int h)
{
      //  glDisable(GL_DITHER);

        /*
         * Some one-time OpenGL initialization can be made here
         * probably based on features of this particular context
         */
	W=w;
	H=h;
        glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	   // glEnable(GL_CULL_FACE);
	    //glFrontFace(GL_CCW);
        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
       // glCullFace(GL_BACK);
        glViewport(0, 0, w, h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        //glhPerspectivef2(70.0f,768.0f/1280.0f, 1.0f, 100.0f);
        xgluPerspective(65.0f,(float)w/(float)h, 1.0f, 100.0f);
        camera.setCamera(0.0, Horizon, Z,   0.0, Horizon, 1.0,   0.0, 1.0, 0.0);

}

void setLight(){

	        float light_position[]={5.0f,5.0f,5.0f,5.0f};  //光源位置
	        float light_ambient[]={1.0f,1.0f,1.0f,1.0f};   //环境光
	        float light_diffuse[]={1.0f,1.0f,1.0f,1.0f};   //漫反射
	        float light_specular[]={1.0f,1.0f,1.0f,1.0f};   //镜面反射

	        glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	        glEnable(GL_LIGHT0);

	        float light_position2[]={6.0f,5.0f,-3.0f,3.0f};  //光源位置
	        float light_ambient2[]={1.0f,1.0f,1.0f,1.0f};   //环境光
	        float light_diffuse2[]={1.0f,1.0f,1.0f,1.0f};   //漫反射
	        float light_specular2[]={1.0f,1.0f,1.0f,1.0f};   //镜面反射
	        glLightfv(GL_LIGHT1, GL_POSITION, light_position2);
	        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient2);
	        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse2);
	        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular2);
	        glEnable(GL_LIGHT1);

	        glEnable(GL_LIGHTING);
}

static void drawFrame()
{

        /*
         * By default, OpenGL enables features that improve quality
         * but reduce performance. One might want to tweak that
         * especially on software renderer.
         */
        glDisable(GL_DITHER);

        /*
         * Usually, the first thing one might want to do is to clear
         * the screen. The most efficient way of doing this is to use
         * glClear().
         */

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //打开alpha测试
        glEnable(GL_ALPHA_TEST); // 启用Alpha测试
        glAlphaFuncx(GL_GREATER, 60);
        /*
         * Now we're ready to draw some 3D objects
         */
       // glTranslatef(0,-0.5,-2);
        //glScalef(0.5,0.5,0.5);

        setLight();
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_DST_ALPHA);
        glEnable(GL_BLEND);

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);

        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
        glEnable(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
        //glFrontFace(GL_CW);

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        //suLookAt(0.0, 1.0, 5.0,   0.0, 1.0, 1.0,   0.0, 1.0, 0.0);
        glPushMatrix();
                	       // glLoadIdentity();
        Vector3 vec3l = camera.getPosition();
        glScalef(5.0f,5.0f,8.0f);
        glTranslatef(0.5 - mPFirst.x/W , .5 - mPFirst.y/H , -0.8 );
        conBall.draw2();

        glBindTexture(GL_TEXTURE_2D, 0);
        glPopMatrix();

        	        camera.setLook();

/*

        	        for(vector<Entity *>::size_type i = 0; i !=entityVector.size(); ++i )
        	        	 {
        	        	    Vector3 vec3=entityVector[i]->position;
        	        	    glTranslatef(vec3.x, vec3.y, vec3.z);
        	        	    entityVector[i]->draw2();
        	        	  }
*/
        	      //  entity.draw2();
        	        entity2.draw2();
        	        Vector3 Cposition=camera.getPosition();
        	       // LOGE("%f, %f, %f\n", Cposition.x,Cposition.y,Cposition.z);
        	        glTranslatef(Cposition.x,Cposition.y,Cposition.z);

        	        Vector3 vec3=leadingRole.getPosition();
        	        glTranslatef(vec3.x, vec3.y, vec3.z);
        	        glRotatef(180,0,1,0);
        	     //   leadingRole.draw();

        	        glRotatef( skyBox.getYRot() , 0 , 1 , 0 );
        	     //   skyBox.draw2();
        	       //

        	/*
        int i=0;
        while(i<(ep->numofNewMtl)){
        	if (ep->np[i].mTextureId!=0)
        		{
        			glBindTexture(GL_TEXTURE_2D,ep->np[i].mTextureId);
        			glTexCoordPointer(2,GL_FLOAT,0,ep->np[i].VTNp->tp);
        			//LOGI("GL %d = %d\n", ep->np[i].mTextureId, ep->np[i].mTextureId);
        		}
        	glNormalPointer(GL_FLOAT, 0, ep->np[i].VTNp->np);
        	glVertexPointer(3, GL_FLOAT, 0, ep->np[i].VTNp->vp);
            glDrawArrays(GL_TRIANGLES,0,ep->np[i].VTNp->lV/12);
        	i++;
        }
        */

/*
        glTranslatef(3,0,0);
        i=0;
        while(i<(ep->numofNewMtl)){
                	if (ep->np[i].mTextureId!=0)
                		{
                			glBindTexture(GL_TEXTURE_2D,ep->np[i].mTextureId);
                			glTexCoordPointer(2,GL_FLOAT,0,ep->np[i].VTNp->tp);
                			//LOGI("GL %d = %d\n", ep->np[i].mTextureId, ep->np[i].mTextureId);
                		}
                	glNormalPointer(GL_FLOAT, 0, ep->np[i].VTNp->np);
                	glVertexPointer(3, GL_FLOAT, 0, ep->np[i].VTNp->vp);
                    glDrawArrays(GL_TRIANGLES,0,ep->np[i].VTNp->lV/12);
                	i++;
                }
 */
        glDisable(GL_TEXTURE_2D);
}

//extern "C"{
JNIEXPORT jint JNICALL Java_com_ly_mygame1Jni_Natives_startLoadGame(JNIEnv* env,
		jobject thiz){
	//(*env)->GetJavaVM(env, &g_VM);
	//static struct EntityData *ep;//=(struct Entity *)malloc(100);
	//LOGE("Could not link program:\n%d\n", ep);
	if(FIRST==0){
		//ep=(struct EntityData *)malloc(100);
	    //loadModel(ep);
		//Entity entity();
		char fileName1[]="pcylinder3-1.ly\0";//my_scene2_2.ly\0";//"my_scene1_7.ly";
		entity.setFileName(fileName1);//fileName1);
		char fileName1_1[]="cylinder1-2.amn\0";
		entity.setAniFile(fileName1_1);
		//entity.load2();

		entityVector.push_back(&entity);


		char fileName2[]="jiliye3_1.ly\0";
		leadingRole.setFileName(fileName2);
	//	leadingRole.load();
		Vector3 position1=Vector3(0.0f, -2.0f, -2.0f);
		leadingRole.setPosition(position1);
		//entityVector.push_back(&leadingRole);

		char fileName3[]="skybox1-1.ly\0";
		skyBox.setFileName(fileName3);
		skyBox.setYRotSpeed(0.02f);
		skyBox.load2();

		char fileName4[]="conball1-1.ly\0";
		conBall.setFileName(fileName4);
		//skyBox.setYRotSpeed(0.02f);
		conBall.load2();

		char fileName5[]="manModel2-1.ly\0";//my_scene2_2.ly\0";//"my_scene1_7.ly";
		entity2.setFileName(fileName5);//fileName1);
		char fileName5_1[]="man_walk1.amn\0";
		entity2.setAniFile(fileName5_1);
		entity2.load2();

		entityVector.push_back(&entity2);

		terrain.init();
	    FIRST=1;
	}
	drawFrame();

	int k=10;//ep->np->VTNp->lV;
	return k;

}

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_exit(JNIEnv* env,
		jobject thiz)
{
	delete &entity;
	delete &entity2;
	delete &leadingRole;
	delete &skyBox;
	delete &conBall;
	delete &terrain;
}


JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_initScene(JNIEnv* env,
		jobject thiz, jint w, jint h)
{
	init_scene(w,h);
}

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_moveCameraFirstP(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y)
{
	mPFirst.x=x;
	mPFirst.z=y;
}

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_moveCamera(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y)
{
	//sX+=x/1000;
	//sZ+=y/1000;
	//sZ=z;
	float xx=(x-mPFirst.x)/W;
	float yy=(y-mPFirst.z)/H;
	Vector3 vec3l = camera.getPosition();
	Vector3 vec3V=camera.getView()-vec3l;
	camera.yawCamera(-xx);
	camera.moveCamera(yy);
	/*
	Vector3 vec3n = camera.getPosition();
	float *xyz;
	xyz=terrain.getXY( vec3l.x , vec3l.z , vec3n.x , vec3n.z );
	camera.yawCamera(xx);
	camera.moveCamera(-yy);

	camera.yawCamera(-xyz[0]+vec3l.x);
	camera.moveCamera(xyz[1]-vec3l.z);
	camera.setHeight(xyz[2]+Horizon);

	vec3n = camera.getPosition();

	camera.setView(vec3V+vec3n);
*/
}

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_rotateCameraLast(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y){
	rPLast.x=x;
	rPLast.y=y;
}

JNIEXPORT void JNICALL Java_com_ly_mygame1Jni_Natives_rotateCamera(JNIEnv* env,
		jobject thiz, jfloat x, jfloat y)
{

	rotating=true;
	//sX=x;
	//sY=y;
	//sZ=z;
	rPNow.x=x;
	rPNow.y=y;
	Vector3 yaw;
	Vector3 cross=camera.getView()- camera.getPosition();
	//yaw.x=rPLast.x-rPNow.x;
	//yaw.y=rPNow.y-rPLast.y;
	//yaw.z=0;
	yaw=rPNow-rPLast;
	yaw.z=0;
	yaw=camera.changeCoord(yaw);
	cross=cross.crossProduct(yaw);
	yaw=cross.normalize();
	yaw=yaw;
	float theta=sqrt((rPNow.x-rPLast.x)*(rPNow.x-rPLast.x)
			+(rPNow.y-rPLast.y)*(rPNow.y-rPLast.y))/1000;
	//LOGI("theta:%f\n", theta);
	camera.rotateView(theta, yaw.x, yaw.y, yaw.z);
	rPLast.x=x;
	rPLast.y=y;
	//LOGI("x:%f  %f\n", cross.x,yaw.x);
}

//}
