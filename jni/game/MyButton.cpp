#include "MyButton.h"
#include "GLSL/GLSL.h"
#include "log.h"
#include "commond.h"
#include "MJNI.h"

MyButton::MyButton()
:state(STATE_NORMAL),
 id(0),
 isActive(false),
Graph()
{
	Center2D[0]=0;
	Center2D[1]=0;
	R=50;
	frameSum=50.0f;
}

void  MyButton::setCenter()
{
	pfTemp4[0] = pTransform->pTOmatrix->mMatrixQueue.back()[12];
	pfTemp4[1] = pTransform->pTOmatrix->mMatrixQueue.back()[13];
	pfTemp4[2] = pTransform->pTOmatrix->mMatrixQueue.back()[14];
	pfTemp4[3] = 1.0f;

	getMatrix().multiplyMV(pfTemp4, 0, getGL().MVPMatrix, 0, pfTemp4, 0);

	if (pfTemp4[3] == 0.0f) return;

	R = 1.0f / pfTemp4[3];

	Center2D[0] = getGL().viewport[0] + getGL().viewport[2]
			         * (pfTemp4[0] * R + 1.0f)* 0.5f;

	Center2D[1] = getGL().viewport[3]//getGL().viewport[1] +
			         *(1.0f- (pfTemp4[1] * R + 1.0f) * 0.5f);

	//LOGI("x,y: %d, %d",	Center2D[0],	Center2D[1] );
	R=100;

	scale_origen[0] = pTransform->pTOmatrix->mMatrixQueue.back()[0];
	scale_origen[1] = pTransform->pTOmatrix->mMatrixQueue.back()[5];
	scale_origen[2] = pTransform->pTOmatrix->mMatrixQueue.back()[10];
}

bool MyButton::isTouch(float x, float y)
{
	if( ( (x-Center2D[0])*(x-Center2D[0])+(y-Center2D[1])*(y-Center2D[1]) )<R*R )
	return true;

	return false;
}

bool MyButton::onTouchDown(float x, float y)
{
	if( ( (x-Center2D[0])*(x-Center2D[0])+(y-Center2D[1])*(y-Center2D[1]) )<R*R )
	{
		changeState(STATE_DOWN);
		return true;
	}

	return false;

	//state=STATE_DOWN;
}

bool MyButton::onTouchUp(float x, float y)
{
	changeState(STATE_UP);
	return true;
	if( ( (x-Center2D[0])*(x-Center2D[0])+(y-Center2D[1])*(y-Center2D[1]) )<R*R )
	{
		changeState(STATE_UP);
		return true;
	}
	else
	{

	}
	return false;
	//state=STATE_UP;
}

void MyButton::onTouchUpLess()
{
	changeState(STATE_UPLESS);
}

void MyButton::update()
{
	if(!isActive)
	{
		updateNoActive();
		return;
	}
	//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();

	if(state==STATE_DOWN)
	{
		//getGL().scale(0.5f,1.0f,0.5f);
		pTransform->pTOmatrix->mMatrixQueue.back()[0]=scale_origen[0]*0.5;
		pTransform->pTOmatrix->mMatrixQueue.back()[5]=scale_origen[1]*0.5;
		pTransform->pTOmatrix->mMatrixQueue.back()[10]=scale_origen[2]*0.5;
	}
	else if(state==STATE_UP)
	{
		frame+=3.0f;
		//getGL().scale(frame/frameSum,1.0f,frame/frameSum);
		pTransform->pTOmatrix->mMatrixQueue.back()[0]=scale_origen[0]*frame/frameSum;
		pTransform->pTOmatrix->mMatrixQueue.back()[5]=scale_origen[1]*frame/frameSum;
		pTransform->pTOmatrix->mMatrixQueue.back()[10]=scale_origen[2]*frame/frameSum;
		if(frame>=frameSum)
		{
			frame=25.0f;
			state=STATE_NORMAL;
			pTransform->pTOmatrix->mMatrixQueue.back()[0]=scale_origen[0];
			pTransform->pTOmatrix->mMatrixQueue.back()[5]=scale_origen[1];
			pTransform->pTOmatrix->mMatrixQueue.back()[10]=scale_origen[2];
			pMjni->toGameScene(id);
			/*
			pMjni->beforeLayoutLoad();
			pGame->load(id);//id
			pMjni->afterLayoutLoad();
			pGame->startGame();
			pAppScene=pGame;
			SysSceneType=SCENETYPE_GAME;
			*/

		}
	}else if(state==STATE_UPLESS)
	{
		frame+=3.0f;
		//getGL().scale(frame/frameSum,1.0f,frame/frameSum);
		pTransform->pTOmatrix->mMatrixQueue.back()[0]=scale_origen[0]*frame/frameSum;
		pTransform->pTOmatrix->mMatrixQueue.back()[5]=scale_origen[1]*frame/frameSum;
		pTransform->pTOmatrix->mMatrixQueue.back()[10]=scale_origen[2]*frame/frameSum;
		if(frame>=frameSum)
		{
			frame=25.0f;
			state=STATE_NORMAL;
			pTransform->pTOmatrix->mMatrixQueue.back()[0]=scale_origen[0];
			pTransform->pTOmatrix->mMatrixQueue.back()[5]=scale_origen[1];
			pTransform->pTOmatrix->mMatrixQueue.back()[10]=scale_origen[2];
		}
	}

	drawNoMovement();
}

void MyButton::updateNoActive()
{
	//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();

	float pColor[3]={0.5,0.4,0.5};

	getGL().material->setAmbient(pColor);

	//getGL().material->setDiffuse(pColor);
	//getGL().material->setSpecular(pColor);

	getGL().material->draw();

	drawNoMovement();
}

void MyButton::setActive(bool isActive_)
{
	isActive=isActive_;
}

void MyButton::changeState(int state_)
{
	switch(state)
	{
	case STATE_NORMAL:
		if(state_==STATE_DOWN) state=STATE_DOWN;
		return;
	case STATE_DOWN:
		if(state_==STATE_UP||state_==STATE_UPLESS)
			{
				state=state_;
				frame=25.0f;
			}
		return;
	case STATE_UP:
		return;
		break;
	}
	state=state_;
	//state=state_;
	//if(state==STATE_UP)

}
