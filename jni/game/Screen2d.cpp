#include "Screen2d.h"
#include "GLSL/GLSL.h"
#include "log.h"

Screen2d::Screen2d()
{

}

void Screen2d::init()
{
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	getMatrix().screenPto3dP(0, 0, pLeftUp);
	//getMatrix().screenPto3dP(getGL().viewport[2], 0, pRightUp);
	//getMatrix().screenPto3dP(getGL().viewport[2], getGL().viewport[3], pRightDown);
	//getMatrix().screenPto3dP(0, getGL().viewport[3], pLeftDown);

	vectorE[0]=-pLeftUp[0]*2.0f/(float)(getGL().viewport[2]);
	vectorE[1]=-pLeftUp[1]*2.0f/(float)(getGL().viewport[3]);
	vectorE[2]=pLeftUp[2];
}

void Screen2d::getPos(int x, int y, float *pos)
{
	pos[0]=vectorE[0]*x+pLeftUp[0];
	pos[1]=vectorE[1]*y+pLeftUp[1];
	pos[2]=pLeftUp[2];
}

Screen2d *pScreen2d=0;
