#pragma once
#include "Entity.h"

class MShape:public Entity
{
public:
        Entity * pEntity;//������Graph

        MShape();
        ~MShape();
        void setGraph(Entity *);//����������Graph���Դ��ݱ任����
        void draw();
        protected:
        int shapeType; //shape type : 0: circle, 1: cube

public:
        void operator = (const MShape& mShape_);
};
