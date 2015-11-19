#include "Transform.h"

Transform::Transform()
	:pTOmatrix(new OMatrix()),
	pROmatrix(new OMatrix()),
	pSOmatrix(new OMatrix())
{

}

Transform::~Transform()
{
	delete pTOmatrix;
	delete pSOmatrix;
	delete pROmatrix;
}

void Transform::copy(Transform* pTransform_)
{
	pTOmatrix->copy(pTransform_->pTOmatrix);
	pSOmatrix->copy(pTransform_->pSOmatrix);
	pROmatrix->copy(pTransform_->pROmatrix);
}

void Transform::scale(float x, float y, float z){
	pSOmatrix->scale(x, y, z);
}

void Transform::translate(float x, float y, float z){
	pTOmatrix->translate(x, y, z);
}

void Transform::rotate(float a, float x, float y, float z){
	pROmatrix->rotate(a ,x, y, z);
}
