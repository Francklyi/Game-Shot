#include"line.h"
#include "GLSL/GLSL.h"

Line::Line()
:pfPointers(0)
,num(0)
{
	pColor[0]=0.5f;
	pColor[1]=0.5f;
	pColor[2]=0.0f;
}

Line::~Line()
{

}

void Line::setPointers(Vector3 p1_, Vector3 p2_)
{
  p1=p1_;
  p2=p2_;
}

float Line::getLength()
{
  return (p1-p2).length();
}

void Line::draw()
{
  getGL().glslUpdate();
  float vertics[]= { p1.x, p1.y, p1.z, p2.x, p2.y, p2.z };
  unsigned short index[] = { 0 , 1 };

  getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
  getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0,3, 0, vertics);
  getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0,2, 0, vertics);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDrawElements(GL_LINES,2,GL_UNSIGNED_SHORT,index);
}

void Line::draw(Vector3 *p1_, Vector3 *p2_)
{

  float vertics[]= { p1_->x, p1_->y, p1_->z, p2_->x, p2_->y, p2_->z };
  unsigned short index[] = { 0 , 1 };
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
  getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0,3, vertics);
  getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0,2, 0, vertics);

  getGL().glslUpdate();

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  glDrawElements(GL_LINES,2,GL_UNSIGNED_SHORT,index);
}

void Line::drawMults()
{
	//getGL().material->setAmbient(pColor);

	getGL().material->setDiffuse(pColor);
	//getGL().material->setSpecular(pColor);

	getGL().material->draw();

	getGL().setHasTex(0.0f);

	getMatrix().multiplyMM(pf16, 0, getGL().PMatrix, 0, getGL().VMatrix, 0);


    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, 1, false, getGL().VMatrix );

    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, 1, false, pf16 );
	//float vertics[]= { p1_->x, p1_->y, p1_->z, p2_->x, p2_->y, p2_->z };
	//unsigned short index[] = { 0 , 1 };
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, pfPointers);
	getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0,3, pfPointers);
	getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0,2, 0, pfPointers);

	//getGL().glslUpdate();
	glLineWidth(2.125f);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glDrawElements(GL_LINES,2,GL_UNSIGNED_SHORT,index);
	glEnable(GL_BLEND);
	glBlendFunc(GL_ONE, GL_ONE);
	glDrawArrays(GL_LINE_STRIP,0,num);
	glDisable(GL_BLEND);

	glLineWidth(1.125f);
}
