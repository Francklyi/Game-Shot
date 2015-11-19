//#include <libxml/parser.h>
//#include <libxml/tree.h>
//#include <stdlib.h>
#include "Entity.h"
//#include "fileDeal/loadFile.h"
#include "log.h"
#include "GLSL/GLSL.h"
#include <GLES2/gl2.h>

int Entity::selectNameSum=100;

Entity::Entity()
:selectName(0),
isSelect(0),
//transMtxs(0),
pTransform(new Transform())
{
}

Entity::~Entity()
{
}

void Entity::setSelectName()
{
        if(selectName==0){
                selectNameSum++;
                selectName=selectNameSum;
        }
        //glLoadName(selectName);
}

void Entity::draw()
{
	getGL().chooseMatrix(GLSL::MMATRIX);
		if(pTransform->pTOmatrix){

			//pop for translate Matrix
			//getGL().pushMatrix();

			getGL().multMatrix(pTransform->pTOmatrix->mMatrixQueue.back());
			//pop for rotate Matrix
			//getGL().pushMatrix();

			//getGL().multMatrix(pTransform->pROmatrix->mMatrixQueue.back());
			//pop for scalse Matrix
			//getGL().pushMatrix();

			//getGL().multMatrix(pTransform->pSOmatrix->mMatrixQueue.back());

		}
}

void Entity::calculateMatrix()
{

	//getGL().multMatrix(pTransform->pTOmatrix->mMatrixQueue.back());

	getMatrix().setIdentityM(getMatrix().sTemp,0);
	getMatrix().setIdentityM(getMatrix().sTemp,16);

	getMatrix().multiplyMM(getMatrix().sTemp, 16,
						pTransform->pROmatrix->mMatrixQueue.back(), 0,
						pTransform->pSOmatrix->mMatrixQueue.back(), 0);

	getMatrix().multiplyMM(pTransform->pTOmatrix->mMatrixQueue.back(), 0,
						pTransform->pTOmatrix->mMatrixQueue.back(), 0,
						getMatrix().sTemp, 16);
	getMatrix().arrayCopy(pTransform->pTOmatrix->mMatrixQueue.back(), 0,
						matrixOrg,0,16);
}

void Entity::resetPosMatrix()
{
	getMatrix().arrayCopy(matrixOrg,0,
			pTransform->pTOmatrix->mMatrixQueue.back(), 0,
						16);
}

void Entity::setPosBuf(GLuint ubo,GLsizei stride, const GLvoid* pointer)
{
        getGL().attribPointer(getGL().gvPositionHandle, ubo,3, stride, pointer);
}

void Entity::setNormBuf(GLuint ubo,GLsizei stride, const GLvoid* pointer)
{
        getGL().attribPointer(getGL().gvNormalHandle, ubo,3, stride, pointer);
}

void Entity::setTexCoordBuf(GLuint ubo,GLsizei stride, const GLvoid* pointer)
{
        getGL().attribPointer(getGL().gvTexCoordHandle, ubo,2, stride, pointer);
}

void Entity::setPos(float x, float y, float z)
{
	pTransform->pTOmatrix->mMatrixQueue.back()[12]=x;
	pTransform->pTOmatrix->mMatrixQueue.back()[13]=y;
	pTransform->pTOmatrix->mMatrixQueue.back()[14]=z;
}


void Entity::scale(float x, float y, float z){
	pTransform->scale(x, y, z);
}

void Entity::translate(float x, float y, float z){
	pTransform->translate(x, y, z);
}

void Entity::rotate(float a, float x, float y, float z){
	pTransform->rotate(a ,x, y, z);
}

void Entity::move(float x, float y, float z)
{
	pTransform->pTOmatrix->mMatrixQueue.back()[12]+=x;
	pTransform->pTOmatrix->mMatrixQueue.back()[13]+=y;
	pTransform->pTOmatrix->mMatrixQueue.back()[14]+=z;
}

void Entity::copy(Entity* pE)
{
	pTransform->copy(pE->pTransform);
}
