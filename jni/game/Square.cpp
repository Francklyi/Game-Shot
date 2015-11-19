#include "Square.h"
#include "GLSL/GLSL.h"
#include "GLSL/matrix.h"

Square::Square()
{
	pColor[0]=0.1f; pColor[1]=1.0f; pColor[2]=0.0f;
	index[0]=0; index[1]=2; index[2]=1;
	index[3]=0; index[4]=3; index[5]=2;
}

void Square::setPos()
{
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	float pf4[4];
	//getGL().glslUpdate();
	getMatrix().screenPto3dP(100, 100, vertics);
	getMatrix().screenPto3dP(200, 100, &(vertics[3]));

	interval[0]=vertics[3]-vertics[0];
	interval[1]=vertics[4]-vertics[1];
	interval[2]=vertics[5]-vertics[2];

	getMatrix().screenPto3dP(200, 200, &(vertics[6]));
	//getMatrix().screenPto3dP(10, 30, &(vertics[9]));

	vertics[9] =vertics[6]-interval[0];
	vertics[10]=vertics[7]-interval[1];
	vertics[11]=vertics[8]-interval[2];
}

void Square::draw()
{
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	getGL().material->setAmbient(pColor);
	getGL().material->setDiffuse(pColor);
	getGL().material->setSpecular(pColor);

	getGL().material->draw();


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0, 0, texCoord);

	getGL().glslUpdate();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,index);

	//glFlush();
	//line.draw(&p1, &pMid);
}
