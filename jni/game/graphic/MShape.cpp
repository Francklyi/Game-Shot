#include "MShape.h"
#include "Graph.h"

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
	pTransform->copy(pGraph1->pTransform);
	//*pTOmatrix=*pGraph1->pTOmatrix;
	//*pSOmatrix=*pGraph1->pSOmatrix;
	//*pROmatrix=*pGraph1->pROmatrix;
}

void MShape::draw(){
	Entity::draw();
	getGL().glslUpdate();
}

void MShape::copy(MShape* pMShape)
{
	mass=pMShape->mass;
	restn=pMShape->restn;
	shapeType=pMShape->shapeType;
	pTransform->copy(pMShape->pTransform);
	//pTOmatrix->copy(pMShape->pTOmatrix);
	//pSOmatrix->copy(pMShape->pSOmatrix);
	//pROmatrix->copy(pMShape->pROmatrix);
}

void MShape::operator = (const MShape& mShape_)
{

}
