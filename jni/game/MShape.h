#pragma once
#include "Entity.h"

class MShape:public Entity
{
public:
        Entity * pEntity;//所属的Graph

        MShape();
        ~MShape();
        void setGraph(Entity *);//设置所属的Graph，以传递变换矩阵
        void draw();
        protected:
        int shapeType; //shape type : 0: circle, 1: cube

public:
        void operator = (const MShape& mShape_);
};
