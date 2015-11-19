#include <jni.h>
//#include <test.h>
#include <stdio.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//#include <GLES/glext.h>
#include "GLSL.h"
#include "bulletTest.h"
#include "log.h"
#include "glu.h"
#include "graph.h"
//#include "matrix.h"


#define STRINGIFY(A) #A
#define final const

int W=0, H=0;
static float Z= 0.0f , Horizon=0.0f;

float *ps=0;
float *pn=0;
unsigned int *pi=0;
int pNum;
float ball_r=3;

float g=0;

GLSL *glsl=new GLSL();

float mModelMatrix[16];//view Matrix

float mViewMatrix[16]; //model Matrix

float mProjectionMatrix[16]; //projection Matrix

float mMVPMatrix[16]; // modle * view * projection Matrix

/**
 * Stores a copy of the model matrix specifically for the light position.
 */
float mLightModelMatrix[16];

//********program1 for vertex
GLuint gProgram;

GLuint MVPMatrixHandle;

GLuint MVMatrixHandle;

GLuint gvPositionHandle;

GLuint gvNormalHandle;

GLuint gvColorHandle;

GLuint lightPosHandle;

//********program2 for light
GLuint gLightProgram;

GLuint gLightvPositionHandle;

GLuint MVPLightMatrixHandle;
 //GLuint gvNormalHandle;
 //GLuint gvColorHandle;

extern "C" {

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_init(JNIEnv * env, jobject obj);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_testNative(JNIEnv * env, jobject obj);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_initScene(JNIEnv* env,
                                                                    jobject thiz, jint w, jint h);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_draw(JNIEnv * env, jobject obj);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_exit(JNIEnv * env, jobject obj);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_addSphere(JNIEnv * env, jobject obj, jint n);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_reduceSphere(JNIEnv * env, jobject obj);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_setGravity(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_onSurfaceChanged(JNIEnv* env,
                                                                    jobject thiz, jint w, jint h);

    JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_onSurfaceCreated(JNIEnv* env, jobject thiz);

    void setLight1();

    void setLight();

    void initBt();

    int bullet_exit();

    void addSphere(int n);

    void reduceSphere();

    btVector3 getXYZ(btRigidBody* fallRigidBody_);

    btVector3 colVelocity(btVector3 velocity_);

    void QuatToMatrix( btQuaternion* quat, float m[16] );
};


static void printGLString(const char *name, GLenum s) {
    const char *v = (const char *) glGetString(s);
    LOGI("GL %s = %s\n", name, v);
}


static void checkGlError(const char* op) {
    for (GLint error = glGetError(); error; error
            = glGetError()) {
        LOGI("after %s() glError (0x%x)\n", op, error);
    }
}

static const char gVertexShader[] =STRINGIFY(
                attribute vec4 vPosition;
                        uniform float vColor;
                        varying lowp vec4 myColor;
                        float reflectVec = length(myColor-vPosition);
                    void main() {
                    myColor= vColor*vPosition;
                    gl_Position=vPosition;
                    }
);

static const char gFragmentShader[] =STRINGIFY(
                        varying lowp vec4 myColor;
                    void main() {
                    gl_FragColor=5.0 * myColor * vec4(0.5,0.5,0.5,0.5);
                    }
);

static const char gVertexShader2[] =STRINGIFY(
                attribute vec4 vPosition;
                attribute vec3 vNormal;

                uniform mat4 MVPMatrix;
                uniform mat4 MVMatrix;
                uniform vec3 lightPos;

                varying vec4 vColor;

                const float SpecularContribution = 0.3;
                const float DiffuseContribution = 1.0 - SpecularContribution;

                void main() {
                        //Transform the Vertex into eye space.
                        vec3 modelViewVertex = vec3(MVMatrix * vPosition);
                        //Transform the normal's orientation into eye space.
                        vec3 modelViewNormal = normalize( vec3( MVMatrix * vec4( vNormal, 0.0 ) ) );
                        //used for attenuation
                        float distance = length(lightPos - modelViewVertex);
                        //lighting direction vector
                        vec3 lightVector = normalize(lightPos - modelViewVertex);
                        //
                        float diffuse = max(dot(modelViewNormal , lightVector), 0.1);
                        diffuse = diffuse * (1.0/(1.0 + ( 0.25 * distance *distance)));

                        vec3 reflectVec = reflect(-lightVector, modelViewNormal);
                        vec3 viewVec = normalize(-modelViewVertex);
                        float spec = 0.0;
                        if (diffuse > 0.0)
                        {
                        spec = max(dot(reflectVec, viewVec), 0.0);
                        spec = pow(spec, 16.0);
                        }
                        vColor = vec4( 0.2,0.7,0.4, 1.0 ) + vec4( 1.0,1.0,1.0, 1.0 )*
                                        (DiffuseContribution * diffuse + SpecularContribution * spec);

                    gl_Position= MVPMatrix * vPosition;
                }
);

static const char gFragmentShader2[] =STRINGIFY(
                        varying vec4 vColor;
                    void main() {
                    gl_FragColor=vColor;
                    }
);

static const char gVertexShader3[] =STRINGIFY(
                attribute vec3 VertexPosition;
                attribute vec3 VertexNormal;
            varying vec3 LightIntensity;

            uniform vec4 LPosition; // Light position in eye coords.
            uniform vec3 La; // Ambient light intensity
            uniform vec3 Ld; // Diffuse light intensity
            uniform vec3 Ls; // Specular light intensity

                uniform vec3 Ka; // Ambient reflectivity
                uniform vec3 Kd; // Diffuse reflectivity
                uniform vec3 Ks; // Specular reflectivity
                uniform float Shininess; // Specular shininess factor

                uniform mat4 ModelViewMatrix;
                uniform mat3 NormalMatrix;
                uniform mat4 ProjectionMatrix;
                uniform mat4 MVP;
                void getEyeSpace( out vec3 norm, out vec4 position )
                {
                norm = normalize( NormalMatrix * VertexNormal);
                position = ModelViewMatrix * vec4(VertexPosition,1.0);
                }
                vec3 phongModel( vec4 position, vec3 norm )
                {
                vec3 s = normalize(vec3(LPosition - position));
                vec3 v = normalize(-position.xyz);
                vec3 r = reflect( -s, norm );
                vec3 ambient = La * Ka;
                float sDotN = max( dot(s,norm), 0.0 );
                vec3 diffuse = Ld * Kd * sDotN;
                vec3 spec = vec3(0.0);
                if( sDotN > 0.0 )
                spec = Ls * Ks *
                pow( max( dot(r,v), 0.0 ), Shininess );
                return ambient + diffuse + spec;
                }
                void main()
                {

                    LPosition; // Light position in eye coords.
                    La; // Ambient light intensity
                    Ld; // Diffuse light intensity
                    Ls; // Specular light intensity

                        Ka; // Ambient reflectivity
                        Kd; // Diffuse reflectivity
                        Ks; // Specular reflectivity
                        Shininess; // Specular shininess factor

                vec3 eyeNorm;
                vec4 eyePosition;
                // Get the position and normal in eye space
                getEyeSpace(eyeNorm, eyePosition);
                // Evaluate the lighting equation.
                LightIntensity = phongModel( eyePosition, eyeNorm );
                gl_Position = MVP * vec4(VertexPosition,1.0);
                }
);

static const char gLightVertexShader[] =STRINGIFY(
                attribute vec4 vPosition;
                uniform mat4 MVPMatrix;
                uniform mat4 MVMatrix;
                void main() {

                        mat4 m4=MVMatrix;
                    gl_Position = MVPMatrix * vPosition;
                    gl_PointSize=15.0;
                }
);

static const char gLightFragmentShader[] =STRINGIFY(

                void main() {

                    gl_FragColor = vec4(1.0, 1.0, 1.0, 1.0);
                }
);

static const char gVertexShader1[] =STRINGIFY(
                in vec4 MCvertex;
                in vec3 MCnormal;
                uniform mat4 MVMatrix;
                uniform mat4 MVPMatrix;
                uniform mat3 NormalMatrix;
                uniform vec3 LightPosition;
                const float SpecularContribution = 0.3;
                const float DiffuseContribution = 1.0 - SpecularContribution;
                out float LightIntensity;
                out vec2 MCposition;
                void main()
                {
                vec3 ecPosition = vec3(MVMatrix * MCvertex);
                vec3 tnorm = normalize(NormalMatrix * MCnormal);
                vec3 lightVec = normalize(LightPosition - ecPosition);
                vec3 reflectVec = reflect(-lightVec, tnorm);
                vec3 viewVec = normalize(-ecPosition);
                float diffuse = max(dot(lightVec, tnorm), 0.0);
                float spec = 0.0;
                if (diffuse > 0.0)
                {
                spec = max(dot(reflectVec, viewVec), 0.0);
                spec = pow(spec, 16.0);
                }
                LightIntensity = DiffuseContribution * diffuse +
                SpecularContribution * spec;
                MCposition = MCvertex.xy;
                gl_Position = MVPMatrix * MCvertex;
                }
);

GLuint loadShader(GLenum shaderType, const char* pSource) {
    GLuint shader = glCreateShader(shaderType);
    if (shader) {
        glShaderSource(shader, 1, &pSource, NULL);
        glCompileShader(shader);
        GLint compiled = 0;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
        if (!compiled) {
            GLint infoLen = 0;
            glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
            if (infoLen) {
                char* buf = (char*) malloc(infoLen);
                if (buf) {
                    glGetShaderInfoLog(shader, infoLen, NULL, buf);
                    LOGE("Could not compile shader %d:\n%s\n",
                            shaderType, buf);
                    free(buf);
                }
                glDeleteShader(shader);
                shader = 0;
            }
        }
    }
    return shader;
}

GLuint createProgram(const char* pVertexSource, const char* pFragmentSource) {
    GLuint vertexShader = loadShader(GL_VERTEX_SHADER, pVertexSource);
    if (!vertexShader) {
        return 0;
    }

    GLuint pixelShader = loadShader(GL_FRAGMENT_SHADER, pFragmentSource);
    if (!pixelShader) {
        return 0;
    }

    GLuint program = glCreateProgram();
    if (program) {
        glAttachShader(program, vertexShader);
        checkGlError("glAttachShader");
        glAttachShader(program, pixelShader);
        checkGlError("glAttachShader");
        glLinkProgram(program);
        GLint linkStatus = GL_FALSE;
        glGetProgramiv(program, GL_LINK_STATUS, &linkStatus);
        if (linkStatus != GL_TRUE) {
            GLint bufLength = 0;
            glGetProgramiv(program, GL_INFO_LOG_LENGTH, &bufLength);
            if (bufLength) {
                char* buf = (char*) malloc(bufLength);
                if (buf) {
                    glGetProgramInfoLog(program, bufLength, NULL, buf);
                    LOGE("Could not link program:\n%s\n", buf);
                    free(buf);
                }
            }
            glDeleteProgram(program);
            program = 0;
        }
    }
    return program;
}

bool setupGraphics(int w, int h) {
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    LOGI("setupGraphics(%d, %d)", w, h);
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return false;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "vPosition");

    checkGlError("glGetAttribLocation");
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);

    glViewport(0, 0, w, h);
    checkGlError("glViewport");
    return true;
}

const GLfloat gTriangleVertices[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };

void renderFrame() {
    static float grey;
    grey += 0.01f;
    if (grey > 1.0f) {
        grey = 0.0f;
    }
    glClearColor(grey, grey, grey, 1.0f);
    checkGlError("glClearColor");
    glClear( GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    checkGlError("glClear");

    glUseProgram(gProgram);
    checkGlError("glUseProgram");


    glVertexAttribPointer(gvPositionHandle, 2, GL_FLOAT, GL_FALSE, 0, gTriangleVertices);
    checkGlError("glVertexAttribPointer");
    glEnableVertexAttribArray(gvPositionHandle);
    checkGlError("glEnableVertexAttribArray");
    glDrawArrays(GL_TRIANGLES, 0, 3);
    checkGlError("glDrawArrays");
}

/*

void initShader()
{
        //EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    printGLString("Version", GL_VERSION);
    printGLString("Vendor", GL_VENDOR);
    printGLString("Renderer", GL_RENDERER);
    printGLString("Extensions", GL_EXTENSIONS);

    //model
    gProgram = createProgram(gVertexShader, gFragmentShader);
    if (!gProgram) {
        LOGE("Could not create program.");
        return ;
    }
    gvPositionHandle = glGetAttribLocation(gProgram, "VertexPosition");
    gvNormalHandle   = glGetAttribLocation(gProgram, "VertexNormal");
    MVPMatrixHandle  = glGetUniformLocation(gProgram, "MVP" );
    MVMatrixHandle   = glGetUniformLocation(gProgram, "ModelViewMatrix" );

    lightPosHandle    = glGetUniformLocation(gProgram, "LPosition" );

    lightPosHandle    = glGetUniformLocation(gProgram, "LPosition" );

    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
            gvPositionHandle);
    LOGI("glGetAttribLocation(\"vNormal\") = %d\n",
                gvNormalHandle);
    LOGI("glGetUniformLocation(\"MVPMatrix\") = %d\n",
                MVPMatrixHandle);
    LOGI("glGetUniformLocation(\"MVMatrix\") = %d\n",
                MVMatrixHandle);
    LOGI("glGetUniformLocation(\"LPosition\") = %d\n",
                lightPosHandle);

    //light
    gLightProgram = createProgram(gLightVertexShader, gLightFragmentShader);
    if (!gLightProgram) {
        LOGE("Could not create program.");
        return ;
    }
    gLightvPositionHandle = glGetAttribLocation(gLightProgram, "vPosition");
    MVPLightMatrixHandle    = glGetUniformLocation(gLightProgram, "MVPMatrix" );
    GLuint MVLightMatrixHandle    = glGetUniformLocation(gLightProgram, "MVMatrix" );
    LOGI("glGetAttribLocation(\"vPosition\") = %d\n",
                gLightvPositionHandle);
    LOGI("glGetUniformLocation(\"MVPMatrix\") = %d\n",
                MVPLightMatrixHandle);
    LOGI("glGetUniformLocation(\"MVMatrix\") = %d\n",
                        MVLightMatrixHandle);
}
*/

/*

*/

const GLfloat gTriangleVertices1[] = { 0.0f, 0.5f, -0.5f, -0.5f,
        0.5f, -0.5f };
void Cube_draw1()
{
        //CUBE
#undef one
#define one 0.1f
        GLfloat vertices[24] = {
                        -one, -one, -one,
                        one, -one, -one,
                        one,  one, -one,
                        -one,  one, -one,
                        -one, -one,  one,
                        one, -one,  one,
                        one,  one,  one,
                        -one,  one,  one,
        };

        GLfloat colors[] = {
                        0,    0,    0,  one,
                        one,    0,    0,  one,
                        one,  one,    0,  one,
                        0,  one,    0,  one,
                        0,    0,  one,  one,
                        one,    0,  one,  one,
                        one,  one,  one,  one,
                        0,  one,  one,  one,
        };

        byte indices[] = {
                        0, 4, 5,    0, 5, 1,
                        1, 5, 6,    1, 6, 2,
                        2, 6, 7,    2, 7, 3,
                        3, 7, 4,    3, 4, 0,
                        4, 7, 6,    4, 6, 5,
                        3, 0, 1,    3, 1, 2
        };

static GLuint ubo1,ubo2;
static int time=0;
if(time==0){
        glGenBuffers(1,&ubo1);
        glBindBuffer(GL_ARRAY_BUFFER, ubo1);
    glBufferData(GL_ARRAY_BUFFER, 24*sizeof(float), vertices, GL_STATIC_DRAW);
}


glBindBuffer(GL_ARRAY_BUFFER, ubo1);
LOGI("6");
glFrontFace(GL_CW);

glUseProgram(gProgram);
checkGlError("glUseProgram");
LOGI("gProgram: %d",gProgram);
glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
LOGI("7");
checkGlError("glVertexAttribPointer");
glEnableVertexAttribArray(gvPositionHandle);
checkGlError("glEnableVertexAttribArray");
//glDrawArrays(GL_TRIANGLES, 0, 8);

        //glEnableClientState(GL_VERTEX_ARRAY);
        //glVertexPointer(3, GL_FLOAT, 0, vertices);
        //glColorPointer(4, GL_FLOAT, 0 , colors);
if(time==0){
        glGenBuffers(1,&ubo2);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo2);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36, indices, GL_STATIC_DRAW);
}

time=1;

glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo2);

        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_BYTE, 0);
        checkGlError("glDrawArrays");
        //exit shader
}


GLuint* ubo1;
int pNum1=0;
GLfloat n1=0;

void drawFrame()
{
        ///LOGI("drawFrame1");

    glDisable(GL_DITHER);

    glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,GL_MODULATE);

    float clearColor=0;
    glClearColor(clearColor, clearColor, clearColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
         * Now we're ready to draw some 3D objects
         */

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        //glTranslatef(0, 0, -3.0f);

        glEnable(GL_LINE_SMOOTH);
        glEnable(GL_POINT_SMOOTH);

        dynamicsWorld->stepSimulation(1/60.f,60);

    glEnable(GL_LIGHTING);
    setLight();
        //Cube_ground_plane();
        //Cube_plane();
        //glBindBuffer (GL_VERTEX_ARRAY, 0);
        //glBindBuffer (GL_COLOR_ARRAY, 0);
        //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ARRAY_BUFFER, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);
        //glBufferData(GL_ARRAY_BUFFER, 1, NULL, GL_STATIC_DRAW);
        //LOGI("df1");
        drawAxis();
        Cube_draw_L();
        glPushMatrix();
        glScalef(0.5f,0.5f,0.5f);
        Cube_draw();
    glPopMatrix();
        //LOGI("df2");
                ////////////Cube_draw()////////
                for (int i=0; i<box_RigidBodys.size() ;i++)
                {
                        //btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
                        btRigidBody* body = box_RigidBodys[i];
                        if (body && body->getMotionState())
                        {

                glPushMatrix();
                //btVector3 pos=getXYZ(body);
                //glTranslatef( pos.getX(), pos.getY() , pos.getZ());
                float  matrix[16];
                body->getWorldTransform().getOpenGLMatrix(matrix);
        //      btQuaternion quat=body->getOrientation();
        //      QuatToMatrix( &quat, matrix );
                glMultMatrixf(matrix);
                //glScalef(1,3,1);
                Cube_draw();
            glPopMatrix();

         //   btVector3 velocity_ls=body->getLinearVelocity();
         //   velocity_ls=colVelocity(velocity_ls);
        //    body->setLinearVelocity(velocity_ls);
                        }
                }

        //LOGI("df3");
        //LOGI("drawFrame2");
        for (int i=0; i<move_RigidBodys.size() ;i++)
        {
                //btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = move_RigidBodys[i];
                if (body && body->getMotionState())
                {

        glPushMatrix();
        //btVector3 pos=getXYZ(body);
        //glTranslatef( pos.getX(), pos.getY() , pos.getZ());
        float  matrix[16];
        body->getWorldTransform().getOpenGLMatrix(matrix);
//      btQuaternion quat=body->getOrientation();
//      QuatToMatrix( &quat, matrix );
        glMultMatrixf(matrix);
    draw_sphere(pNum);
    glPopMatrix();

    btVector3 velocity_ls=body->getLinearVelocity();
    velocity_ls=colVelocity(velocity_ls);
    body->setLinearVelocity(velocity_ls);
                }
        }
/*
   // LOGI("drawFrame2.5");
    glPushMatrix();
    btVector3 pos2=getXYZ(fallRigidBody2);
    glTranslatef( pos2.getX(), pos2.getY() , pos2.getZ());
    draw_sphere( ps, pn, pi, pNum);
    glPopMatrix();

    glPushMatrix();
    btVector3 pos3=getXYZ(fallRigidBody3);
    glTranslatef( pos3.getX(), pos3.getY() , pos3.getZ());
    draw_sphere( ps, pn, pi, pNum);
    glPopMatrix();

    //LOGI("drawFrame3");

    btVector3 velocity_ls=fallRigidBody->getLinearVelocity();
    velocity_ls=colVelocity(velocity_ls);
    fallRigidBody->setLinearVelocity(velocity_ls);

    velocity_ls=fallRigidBody2->getLinearVelocity();
    velocity_ls=colVelocity(velocity_ls);
    fallRigidBody2->setLinearVelocity(velocity_ls);

    velocity_ls=fallRigidBody3->getLinearVelocity();
    velocity_ls=colVelocity(velocity_ls);
    fallRigidBody3->setLinearVelocity(velocity_ls);
*/
   // LOGI("drawFrame4");
        //float  matrix[16];
//      btQuaternion quat=fallRigidBody->getOrientation();
//      QuatToMatrix( &quat, matrix );
        //glMultMatrixf(matrix);

        //LOGI("getY %f ", getY());
        //Cube_draw();
        //glPushMatrix();
        //glLoadIdentity();
        //draw_sphere(0, 10, 0, 3);

        //glPopMatrix();
        //dynamicsWorld->setGravity(btVector3(0,g,0));
        //g-=0.1;
}

void drawFrame1()
{

        n1+= 0.01f;
        if(n1>4) n1=0;

    float clearColor=0;
    glClearColor(clearColor, clearColor, clearColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        /*
         * Now we're ready to draw some 3D objects
         */

   // LOGI("draw1");

                glUseProgram(gProgram);

            glBindBuffer(GL_ARRAY_BUFFER, ubo1[0]);
//LOGI("draw1-2");
                glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
                //LOGI("8");
                glEnableVertexAttribArray(gvPositionHandle);

                glUniform1f(gvColorHandle,n1);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo1[2]);

                //LOGI("ubo1[2]:%d",ubo1[2]);

            glDrawElements(GL_TRIANGLES, pNum1, GL_UNSIGNED_INT, 0);

    //Cube_draw1();
        //Cube_draw_L();
        //drawAxis();
}


void drawFrame2()
{

        n1+= 0.01f;
        if(n1>4) n1=0;

    float clearColor=0;
    glClearColor(clearColor, clearColor, clearColor, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glUseProgram(gProgram);
   // LOGI("draw1");
    final float mLightPosInModelSpace[]={1.5f, 0.1f, 0.5f, 1.0f};
    //calculate Matrix
    //for mViewMatrix
        // Position the eye in front of the origin.
        final float eyeX = 0.0f;
        final float eyeY = 0.0f;
        final float eyeZ = -7.5f;

        // We are looking toward the distance
        final float lookX = 0.0f;
        final float lookY = 0.0f;
        final float lookZ = 0.0f;

        // Set our up vector. This is where our head would be pointing were we holding the camera.
        final float upX = 0.0f;
        final float upY = 1.0f;
        final float upZ = 0.0f;

        // Set the view matrix. This matrix can be said to represent the camera position.
        // NOTE: In OpenGL 1, a ModelView matrix is used, which is a combination of a model and
        // view matrix. In OpenGL 2, we can keep track of these matrices separately if we choose.
        Matrix::setLookAtM(mViewMatrix, 0, eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

    Matrix::setIdentityM(mModelMatrix, 0);
    Matrix::translateM(mModelMatrix, 0, 0.0f, 0.0f, 5.0f);
    Matrix::multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mModelMatrix, 0);

    glUniformMatrix4fv(MVMatrixHandle, 1, false, mMVPMatrix);

    Matrix::multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);

    glUniformMatrix4fv(MVPMatrixHandle, 1, false, mMVPMatrix);


    glUniform3f(lightPosHandle,
                mLightPosInModelSpace[0], mLightPosInModelSpace[1], mLightPosInModelSpace[2]);

                //vertex pos
            glBindBuffer(GL_ARRAY_BUFFER, ubo1[0]);
//LOGI("draw1-2");
                glVertexAttribPointer(gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
                //LOGI("8");
                glEnableVertexAttribArray(gvPositionHandle);
                //vertex normal
            glBindBuffer(GL_ARRAY_BUFFER, ubo1[1]);
//LOGI("draw1-2");
                glVertexAttribPointer(gvNormalHandle, 3, GL_FLOAT, GL_FALSE, 0, 0);
                //LOGI("8");
                glEnableVertexAttribArray(gvNormalHandle);
    // Pass in the combined matrix.
    //glUniformMatrix4fv(MVPMatrixHandle, 1, false, mMVPMatrix);

                //glUniform1f(gvColorHandle,n1);

                //pass in index
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo1[2]);

                //LOGI("ubo1[2]:%d",ubo1[2]);

            glDrawElements(GL_TRIANGLES, pNum1, GL_UNSIGNED_INT, 0);

            //******draw light
            glBindBuffer(GL_ARRAY_BUFFER, NULL);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, NULL);

            glUseProgram(gLightProgram);

            glVertexAttrib3f(gLightvPositionHandle,
                        mLightPosInModelSpace[0], mLightPosInModelSpace[1], mLightPosInModelSpace[2]);
            glDisableVertexAttribArray(gLightvPositionHandle);
        // Calculate position of the light. Rotate and then push into the distance.
        Matrix::setIdentityM(mLightModelMatrix, 0);
        Matrix::translateM(mLightModelMatrix, 0, 0.0f, 0.0f, -1.0f);
        //Matrix::rotateM(mLightModelMatrix, 0, angleInDegrees, 0.0f, 1.0f, 0.0f);
        //Matrix::translateM(mLightModelMatrix, 0, 0.0f, 0.0f, 2.0f);

        //Matrix::multiplyMV(mLightPosInWorldSpace, 0, mLightModelMatrix, 0, mLightPosInModelSpace, 0);
        //Matrix::multiplyMV(mLightPosInEyeSpace, 0, mViewMatrix, 0, mLightPosInWorldSpace, 0);
                // Pass in the transformation matrix.
        Matrix::setIdentityM(mMVPMatrix, 0);
                Matrix::multiplyMM(mMVPMatrix, 0, mViewMatrix, 0, mLightModelMatrix, 0);
                Matrix::multiplyMM(mMVPMatrix, 0, mProjectionMatrix, 0, mMVPMatrix, 0);

                glUniformMatrix4fv(MVPLightMatrixHandle, 1, false, mMVPMatrix);

                // Draw the point.
                glDrawArrays(GL_POINTS, 0, 1);
    //Cube_draw1();
        //Cube_draw_L();
        //drawAxis();
}


static float a=0;

void drawFrame3()
{
  a+=0.5f;
  if(a>360) a=0;

  glsl->useProgram();
  //LOGI("drawFrame3");
  float clearColor=0;
  glClearColor(clearColor, clearColor, clearColor, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  float LightPos[]={6.0f, -6.0f, 3.5f};
  glsl->light->setPos(LightPos);
  glsl->light->draw();

  glsl->material->draw();
  // Position the eye in front of the origin.
  final float eyeX = 0.0f;
  final float eyeY = 0.0f;
  final float eyeZ = -5.5f;

  // We are looking toward the distance
  final float lookX = 0.0f;
  final float lookY = 0.0f;
  final float lookZ = 0.0f;

  // Set our up vector. This is where our head would be pointing were we holding the camera.
  final float upX = 0.0f;
  final float upY = 1.0f;
  final float upZ = 0.0f;
  glsl->chooseMatrix(GLSL::VMATRIX);
  glsl->setIdentity();
  glsl->setLookAt(eyeX, eyeY, eyeZ, lookX, lookY, lookZ, upX, upY, upZ);

  glsl->attribPointer(glsl->gvPositionHandle, ubo1[0], 0, 0);
  glsl->attribPointer(glsl->gvNormalHandle, ubo1[1], 0, 0);
//LOGI("4");


  glsl->chooseMatrix(GLSL::MMATRIX);
  glsl->setIdentity();
  glsl->translate( 0.0f, 0.0f, 2.0f);
  glsl->mutiplyMVMatrix();
//  LOGI("4");
  float* tempMatrix=(float*)malloc(16*sizeof(float));
  for(int i;i<16;i++) tempMatrix[i]=glsl->MVMatrix[i];
  Matrix::invertM(glsl->NormalMatrix,0,tempMatrix,0);

  glUniformMatrix3fv(glsl->NormalMatrixHandle, 1, false, glsl->MVMatrix);
//  LOGI("5");
  glsl->chooseMatrix(GLSL::MVMATRIX);
 // glsl->rotate( a ,0.0f, 1.0f, 0.0f);
  glsl->sendMatrix();

  glsl->mutiplyMPMatrix();
  glsl->chooseMatrix(GLSL::MVPMATRIX);

  glsl->sendMatrix();

 //
 // glsl->chooseMatrix(glsl->MMATRIX);
 // glsl->setIdentity();
 // glsl->chooseMatrix(glsl->MVMATRIX);
 // glsl->setIdentity();

  //pass in index
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ubo1[2]);
  glDrawElements(GL_TRIANGLES, pNum1, GL_UNSIGNED_INT, 0);

}

void setLight(){

      float light_position[]={0.0f,20.0f,10.0f,1.0f};  //光源位置
      float light_ambient[]={0.6f,0.6f,0.6f};   //环境光
      float light_diffuse[]={0.6f,0.6f,0.6f};   //漫反射
      float light_specular[]={0.6f,0.6f,0.6f};   //镜面反射

      glsl->light->setAmbient(light_ambient);
      glsl->light->setDiffuse(light_diffuse);
      glsl->light->setSpecular(light_specular);
}GLuintGLuintGLuint

void setLight1()
{

        GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
        GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
        GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
        /*      light_position is NOT default value     */
        GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
        GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };

        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT0, GL_POSITION, light_position0);

        glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
        glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
        glLightfv(GL_LIGHT1, GL_POSITION, light_position1);

        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);


        glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);

        glClearColor(btScalar(0.7),btScalar(0.7),btScalar(0.7),btScalar(0));

        //  glEnable(GL_CULL_FACE);
        //  glCullFace(GL_BACK);
}

void setMaterial(){
      float material_ambient[]={0.6f,0.6f,0.6f};   //环境光
      float material_diffuse[]={0.6f,0.6f,0.6f};   //漫反射
      float material_specular[]={0.6f,0.6f,0.6f};   //镜面反射
      float shininess = 10.0f;

      glsl->material->setAmbient(material_ambient);
      glsl->material->setDiffuse(material_diffuse);
      glsl->material->setSpecular(material_specular);
      glsl->material->setShininess(shininess);
}

static void onSurfaceCreated()
{
    LOGI("initShader");

    //initShader();
    glsl->init();
    setLight();
    setMaterial();
    LOGI("getSphereData1");

    ubo1=(GLuint*)malloc(3*sizeof(GLuint));

    pNum1=getSphereData1(0.5f, ubo1);
}

static void onSurfaceChanged(int w, int h)
{
      //  glDisable(GL_DITHER);

        /*
         * Some one-time OpenGL initialization can be made here
         * probably based on features of this particular context
         */
        W=w;
        H=h;
      //  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
      //  glEnable(GL_CULL_FACE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

           // glEnable(GL_CULL_FACE);
           // glFrontFace(GL_CCW);
      //  glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);

        glViewport(0, 0, w, h);

                final float ratio = (float) w / h;
                final float left = -ratio;
                final float right = ratio;
                final float bottom = -1.0f;
                final float top = 1.0f;
                final float near = 1.0f;
                final float far = 150.0f;

                glsl->frustum(left, right, bottom, top, near, far);
               // glsl->perspective(60.0f,(float)w/(float)h, 1.0f, 100.0f);
              //  Matrix::frustumM(mProjectionMatrix, 0, left, right, bottom, top, near, far);
        //glhPerspectivef2(70.0f,768.0f/1280.0f, 1.0f, 100.0f);
       // xgluPerspective(60.0f,(float)w/(float)h, 1.0f, 120.0f);
        LOGI("4");
      //  gluLookAt(0, one/2, one+10 ,   0.0, one/2-5, -one,   0.0, 1.0, 0.0);
        LOGI("5");

        //pNum=getSphereData(ps,pn, pi, ball_r);
        LOGI("init_scene : end");
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
      //  glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_FASTEST);
      //  glEnable(GL_CULL_FACE);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

           // glEnable(GL_CULL_FACE);
           // glFrontFace(GL_CCW);
      //  glShadeModel(GL_SMOOTH);
        glEnable(GL_DEPTH_TEST);
        glCullFace(GL_BACK);

        glViewport(0, 0, w, h);

        LOGI("1");
        //glMatrixMode(GL_PROJECTION);
        LOGI("2");
       // glLoadIdentity();
        LOGI("3");
        //glhPerspectivef2(70.0f,768.0f/1280.0f, 1.0f, 100.0f);
       // xgluPerspective(60.0f,(float)w/(float)h, 1.0f, 120.0f);
        LOGI("4");
      //  gluLookAt(0, one/2, one+10 ,   0.0, one/2-5, -one,   0.0, 1.0, 0.0);
        LOGI("5");

        //setLight();
        glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_DST_ALPHA);
        glEnable(GL_BLEND);
        LOGI("6");



        //pNum=getSphereData(ps,pn, pi, ball_r);
        LOGI("init_scene : end");
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_init(JNIEnv * env, jobject obj)
{
        initBt();
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_initScene(JNIEnv* env,
                                                                    jobject thiz, jint w, jint h)
{
         init_scene(w, h);
         return 1;
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_draw(JNIEnv * env, jobject obj)
{
        drawFrame3();
    return 1;
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_testNative(JNIEnv * env, jobject obj)
{
        int a=0;

        return a;
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_exit(JNIEnv * env, jobject obj)
{
        bullet_exit();
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_addSphere(JNIEnv * env, jobject obj, jint n)
{
        addSphere(n);
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_reduceSphere(JNIEnv * env, jobject obj)
{
        reduceSphere();
}


JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_setGravity(JNIEnv * env, jobject obj, jfloat x, jfloat y, jfloat z)
{
        //LOGI("%f , %f , %f",x,y,z);
        dynamicsWorld->setGravity(btVector3(y*10,-x*10,-z*10));
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_onSurfaceChanged(JNIEnv* env,
                                                                    jobject thiz, jint w, jint h)
{
        onSurfaceChanged(w , h);
        return 1;
}

JNIEXPORT jint JNICALL Java_com_ly_bullet1_Native_onSurfaceCreated(JNIEnv* env, jobject thiz)
{
        onSurfaceCreated();
        return 1;
}

void addSphere(int n)
{
        btCollisionShape* fallShape2 =new btSphereShape(ball_r);
        for(int i=0;i<n;i++)
        {
                {
                        //new btBoxShape(btVector3(btScalar(1.),btScalar(1.),btScalar(1.)));
                        //new btCapsuleShape(btScalar(1),btScalar(1));
                        //
                        m_collisionShapes.push_back(fallShape2);

                        btTransform startTransform;
                        startTransform.setIdentity();

                        startTransform.setOrigin(SCALING*btVector3( btScalar(0),
                                                                    btScalar(-one),
                                                                    btScalar(0)));
                        btDefaultMotionState* fallMotionState=
                        new btDefaultMotionState(startTransform);// =
                                        //new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,10,0)));


                        btScalar mass = 10;
                        btVector3 fallInertia(0,0,0);
                        fallShape2->calculateLocalInertia(mass,fallInertia);
                        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape2,fallInertia);
                        btRigidBody* fallRigidBody3 = new btRigidBody(fallRigidBodyCI);

                        move_RigidBodys.push_back(fallRigidBody3);

                        fallRigidBody3->setRestitution(1.0);
                        fallRigidBody3->setLinearVelocity(btVector3(20*i,30,-50));
                        fallRigidBody3->setAngularVelocity(btVector3(5,0,0));
                        //fallRigidBody->setGravity(btVector3(0,-100,0));
                        dynamicsWorld->addRigidBody(fallRigidBody3);
                        }
        }
}

void addBox(int n)
{

        btBoxShape* fallShape2 =new btBoxShape(btVector3(SCALING*1,SCALING*1,SCALING*1));
        btTransform startTransform;
        startTransform.setIdentity();
for(int j=0;j<5;j++)
{

        for(int i=0;i<n;i++)
        {
                {
                        //new btBoxShape(btVector3(btScalar(1.),btScalar(1.),btScalar(1.)));
                        //new btCapsuleShape(btScalar(1),btScalar(1));
                        //
                        m_collisionShapes.push_back(fallShape2);

                        startTransform.setIdentity();
                        startTransform.setOrigin(SCALING*btVector3(
                                                                btScalar(j*SCALING*2-one/4),
                                                                btScalar(i*SCALING*2-one/2),
                                                                btScalar(-one/4)));
                        btDefaultMotionState* fallMotionState =
                                        new btDefaultMotionState(startTransform);
                        btScalar mass = 10;
                        btVector3 fallInertia(0,0,0);
                        fallShape2->calculateLocalInertia(mass,fallInertia);
                        btRigidBody::btRigidBodyConstructionInfo fallRigidBodyCI(mass,fallMotionState,fallShape2,fallInertia);
                        btRigidBody* fallRigidBody3 = new btRigidBody(fallRigidBodyCI);

                        box_RigidBodys.push_back(fallRigidBody3);

                        fallRigidBody3->setRestitution(0.8);
                        fallRigidBody3->setLinearVelocity(btVector3(0,0,0));
                        //fallRigidBody3->setAngularVelocity(btVector3(5,0,0));
                        //fallRigidBody->setGravity(btVector3(0,-100,0));
                        dynamicsWorld->addRigidBody(fallRigidBody3);
                        }
        }
}
}

void reduceSphere()
{
        if(move_RigidBodys.size()==0) return ;

                btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[dynamicsWorld->getNumCollisionObjects()-1];
                btRigidBody* body = btRigidBody::upcast(obj);
                if (body && body->getMotionState())
                {
                        delete body->getMotionState();
                }
                dynamicsWorld->removeCollisionObject( obj );
                delete obj;

                btCollisionShape* shape = m_collisionShapes[m_collisionShapes.size()-1];
                delete shape;
                m_collisionShapes.pop_back();

                move_RigidBodys.pop_back();
}

void initBt(){

        LOGI("initBt 1");
        float Restn = 1.2f;

        broadphase = new btDbvtBroadphase();

        collisionConfiguration = new btDefaultCollisionConfiguration();

        dispatcher = new btCollisionDispatcher(collisionConfiguration);

        solver = new btSequentialImpulseConstraintSolver;

        dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher,broadphase,solver,collisionConfiguration);

        dynamicsWorld->setGravity(btVector3(0,-20,0));

        btCollisionShape* groundShape ;

{//buttom
        groundShape =new btStaticPlaneShape(btVector3(0,1,0),-one/2);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));

        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//top
        groundShape =new btStaticPlaneShape(btVector3(0,-1,0),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//front
        groundShape =new btStaticPlaneShape(btVector3(0,0,-1),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//back
        groundShape =new btStaticPlaneShape(btVector3(0,0,1),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//left
        groundShape =new btStaticPlaneShape(btVector3(1,0,0),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

{//right
        groundShape =new btStaticPlaneShape(btVector3(-1,0,0),-one);

        m_collisionShapes.push_back(groundShape);

        btDefaultMotionState* groundMotionState = new btDefaultMotionState(btTransform(btQuaternion(0,0,0,1),btVector3(0,0,0)));
        btRigidBody::btRigidBodyConstructionInfo
    groundRigidBodyCI(0,groundMotionState,groundShape,btVector3(0,0,0));
        btRigidBody* groundRigidBody = new btRigidBody(groundRigidBodyCI);

        groundRigidBody->setRestitution(Restn-0.5);

        dynamicsWorld->addRigidBody(groundRigidBody);
}

        LOGI("initBt 2");

//addSphere(1);
//addBox(7);

}

btVector3 getXYZ(btRigidBody* fallRigidBody_)
{
   // dynamicsWorld->stepSimulation(1/60.f,5);

    btTransform trans;
    trans=fallRigidBody_->getWorldTransform();
    //fallRigidBody_->getMotionState()->getWorldTransform(trans);

    return trans.getOrigin();
}

int bullet_exit()
{
        //dynamicsWorld->removeRigidBody(fallRigidBody);
        //delete fallRigidBody->getMotionState();
        //delete fallRigidBody;

        //delete fallRigidBody2->getMotionState();
        //delete fallRigidBody2;

        //remove the rigidbodies from the dynamics world and delete them
        int i;
        for (i=dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
        {
                btCollisionObject* obj = dynamicsWorld->getCollisionObjectArray()[i];
                btRigidBody* body = btRigidBody::upcast(obj);
                if (body && body->getMotionState())
                {
                        delete body->getMotionState();
                }
                dynamicsWorld->removeCollisionObject( obj );
                delete obj;
        }
        //dynamicsWorld->removeRigidBody(groundRigidBody);
        //delete groundRigidBody->getMotionState();
        //delete groundRigidBody;


        //delete fallShape;

        //delete fallShape2;

        //delete groundShape;
        //delete collision shapes

        for (int j=0;j<m_collisionShapes.size();j++)
        {
                btCollisionShape* shape = m_collisionShapes[j];
                delete shape;
        }
        m_collisionShapes.clear();

        delete dynamicsWorld;
        delete solver;
        delete collisionConfiguration;
        delete dispatcher;
        delete broadphase;
        return 1;
}

btVector3 colVelocity(btVector3 velocity_)
{
        float max=100 , max1=100;
        float min=1,   min1=1;
        if(velocity_.getX()>max1||velocity_.getX()<-max1)
        {
                if(velocity_.getX()>0)
                        velocity_.setX(max);
                else
                        if(velocity_.getX()<0)
                                velocity_.setX(-max);
        }
        else
                if(velocity_.getX()<min1&&velocity_.getX()>-min1)
                        {
                                if(velocity_.getX()>0)
                                        velocity_.setX(min);
                                else
                                        if(velocity_.getX()<=0)
                                                velocity_.setX(-min);
                        }
///////////////////////////////////
        if(velocity_.getY()>max1||velocity_.getY()<-max1)
        {
                if(velocity_.getY()>0)
                        velocity_.setY(max);
                else
                        if(velocity_.getY()<0)
                                velocity_.setY(-max);
        }
        else
                if(velocity_.getY()<min1&&velocity_.getY()>-min1)
                        {
                                if(velocity_.getY()>0)
                                        velocity_.setY(min);
                                else
                                        if(velocity_.getY()<=0)
                                                velocity_.setY(-min);
                        }
////////////////////////////////////
        if(velocity_.getZ()>max1||velocity_.getZ()<-max1)
        {
                if(velocity_.getZ()>0)
                        velocity_.setZ(max);
                else
                        if(velocity_.getZ()<0)
                                velocity_.setZ(-max);
        }
        else
                if(velocity_.getZ()<min1&&velocity_.getZ()>-min1)
                        {
                                if(velocity_.getZ()>0)
                                        velocity_.setZ(min);
                                else
                                        if(velocity_.getZ()<=0)
                                                velocity_.setZ(-min);
                        }

        return velocity_;
}

void QuatToMatrix( btQuaternion* quat, float m[16] )
{
 float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

 //计算相关的系数
 x2 = quat->x() + quat->x();
 y2 = quat->y() + quat->y();
 z2 = quat->z() + quat->z();

 xx = quat->x() * x2;
 xy = quat->x() * y2;
 xz = quat->x() * z2;
 yy = quat->y() * y2;
 yz = quat->y() * z2;
 zz = quat->y() * z2;
 wx = quat->w() * x2;
 wy = quat->w() * y2;
 wz = quat->w() * z2;

 //将其填入矩阵位置
 m[0] = 1.0f - (yy +zz);
 m[4] = xy - wz;
 m[8] = xz + wy;
 m[12] = 0.0f;
 m[1] = xy + wz;
 m[5] = 1.0f-(xx+zz);
 m[9] = yz + wx;
 m[13] = 0.0f;
 m[2] = xz + wy;
 m[6] = yz + wx;
 m[10] = 1.0f - (xx + yy);
 m[14] = 0.0f;
 m[3] = 0.0f;
 m[7] = 0.0f;
 m[11] = 0.0f;
 m[15] = 1.0f;

}
