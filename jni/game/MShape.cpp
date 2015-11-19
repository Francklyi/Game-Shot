#include "MShape.h"
#include "graphic/Graph.h"

MShape::MShape()
{

}

MShape::~MShape()
{

}

void MShape::setGraph(Entity * pE)
{
        pEntity=pE;
        Graph *pGraph1=(Graph *)pE;
        *pOmatrix=*pGraph1->pOmatrix;
        *pSOmatrix=*pGraph1->pSOmatrix;
        *pROmatrix=*pGraph1->pROmatrix;
}

void MShape::draw(){
        Entity::draw();
        getGL().glslUpdate();
}

void MShape::operator = (const MShape& mShape_)
{

}
