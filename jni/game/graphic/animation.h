#pragma once
#include "joint.h"
#include "Frame.h"
#include "KeyFrame.h"

#include <GLES2/gl2.h>
#include <vector>
using std::vector;

class Animation{
public:
        int direction; //1��ʾ����-1��ʾ����
        int frame;       //��ǰ֡λ��
        int frameNum;
        float *frameTime;
        int jointNum;
        Joint *joints;
        char *fileName;
        //float *jointMatrixs;
        char change_stay;//��Ͳ���
        int id;
        float floorHeight;

        int type;

        GLuint *ubos;

        GLuint ubo;// for all frames

        float *pV;

        vector<KeyFrame*> vKF;

        //int *keyFrame;

        Frame *pFrames;
public:
        Animation();

        ~Animation();

        enum { TYPE_GROUND , TYPE_SKY };

        void setPV(float *pV_);

        void setType( int type_);

        int getType();

        void setFloorHeight(int floorHeight_);

        int getFrameFlag();

        int act();

        int getFrameType();

        void setKeyFrame(int *keyFrame_, int len);

        void addKeyFrame(KeyFrame *pKF);

        int getKeyFrame(int frame);

};
