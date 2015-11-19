#ifndef _ENTITY_H_
#define _ENTITY_H_
//#include <vector>
//#include "Vector3.h"
//#include <GLES/gl.h>
//#include <EGL/egl.h>
#include "graphic/OMatrix.h"
#include "GLSL/GLSL.h"
#include "graphic/Transform.h"

class Entity
{
public:

        int selectName;
        static int selectNameSum;
        bool isSelect;
        Transform *pTransform;

        float matrixOrg[16];

        float modelMatrix[16];

        float normalMatrix[16];
public:
        Entity();
        ~Entity();
        void setSelect(bool isSelect_){ isSelect=isSelect_; }
        void setSelectName();

        int getSelectName(){ return selectName;}

        void virtual getData(){}
        void virtual setData(){}
        void virtual draw();

        void calculateMatrix();

        void resetPosMatrix();

        void setPosBuf(GLuint ubo,GLsizei stride, const GLvoid* pointer);

        void setNormBuf(GLuint ubo,GLsizei stride, const GLvoid* pointer);

        void setTexCoordBuf(GLuint ubo,GLsizei stride, const GLvoid* pointer);

        void setPos(float x, float y, float z);

    	void scale(float x, float y, float z);

    	void translate(float x, float y, float z);

    	void rotate(float a, float x, float y, float z);

    	void copy(Entity* pE);

    	void move(float x, float y, float z);
};
#endif //_ENTITY_H_
