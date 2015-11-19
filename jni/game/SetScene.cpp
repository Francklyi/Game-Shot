#include "SetScene.h"
#include "GLSL/GLSL.h"
#include "log.h"
#include "Camera.h"
#include "fileDeal/loadFile.h"
#include "commond.h"
#include "RenderManager.h"
#include "MJNI.h"

SetScene::SetScene()
:state(STATE_BACKGROUND)
,IdVScene(0)
,touchState(TOUCH_DOWN)
,sceneNum(0)
{
	firstTouchLast=Vector3();
	firstTouchNow=Vector3();
	secorndTouchNow=Vector3();
	secorndTouchLast=Vector3();

	vSquare=vector<Square*>(0);
	vMyButton=vector<MyButton*>(6);
	vMyButton.clear();
	vButtonAll=vector<MyButton*>(20);
	vButtonAll.clear();
	pgBackground=0;
	vpShowWord=vector<ShowWord*>(0);
	//pShowWord=new ShowWord();

	frameAllGetIn=50;
	frameGetIn=0;
	frameAllGetOut=50;
	frameGetOut=0;

	gameLevelRecord=1;
	//state=STATE_GETIN;

	//vvButtonAll=vector<vector<MyButton*>* >(6);
	//vvButtonAll.push_back(new vector<MyButton*>(0));
	//for(int i=0;i<6;i++)
	//{
	//	vvButtonAll[i]=new vector<MyButton*>(0);
	//}
	//vvButtonAll[0]->clear();
}

void SetScene::onCreate(){

	char filgpath[] = "choose7.lyout";//"fight7.lyout";//combineChars(layoutPath, pSceneName);
	LOGI("Graph2:%s", filgpath);
	char *data = getLoadFile().getDataOfLyoutFile(filgpath);//getCharsofFile(filgpath);
	//pGRG->clear();
	int dataPos = 0;
	int NumGraphRace = 0;
	int NumEntity = 0;
	int entityNum = 0;
	int LfileName = 0;
	dataPos++;
	dataPos+=sizeof(int);//version
	NumGraphRace = data[dataPos];
	dataPos++;
	for (int h=0; h<NumGraphRace; h++)
	{
		NumEntity=data[dataPos];
		dataPos++;
		LfileName=data[dataPos];
		dataPos++;
		char *strFN=(char*)malloc(LfileName+1);
		for (int j=0; j<LfileName; j++)
		{
			strFN[j]=data[dataPos];
			dataPos++;
		}
		strFN[LfileName]=0;

		GraphData *pGD=GraphData::getInstance(strFN);//new GraphData();
		//pGD->setFileName(strFN);
		//pGD->load();
		pGD->setBatchs(1);

		for(int i=0; i<NumEntity; i++)
		{
			MyButton *pGTemp = new MyButton();
			//MessageBox(_T(chars));
			pGTemp->setFileName(strFN);
			pGTemp->gp=pGD;
			//pGMain->pGRG->add(gdata2);
			//gdata2->load();
			for (int j=0; j<16; j++)
			{
				float f_=*(float*)(data+dataPos);
				dataPos+=4;
				pGTemp->pTransform->pTOmatrix->mMatrixQueue[0][j]=f_;
			}
			for (int j=0; j<16; j++)
			{
				float f_=*(float*)(data+dataPos);
				dataPos+=4;
				pGTemp->pTransform->pSOmatrix->mMatrixQueue[0][j]=f_;
			}
			for (int j=0; j<16; j++)
			{
				float f_=*(float*)(data+dataPos);
				dataPos+=4;
				pGTemp->pTransform->pROmatrix->mMatrixQueue[0][j]=f_;
			}
			pGTemp->calculateMatrix();
			// read type of the  role
			pGTemp->roleType=*(int*)(data+dataPos);
			dataPos+=4;
			if(pGTemp->roleType==Graph::ROLE_FIXED0)// bg
			{
				pgBackground=pGTemp;
			}
			else if(pGTemp->roleType==Graph::ROLE_FIXED1)
				vButtonAll.push_back(pGTemp);

			//pGMain->graphList.push_back(gdata2);
			//dataPos=onShapeRead(gdata2,data,dataPos);
		}

		free(strFN);
		//dataPos+=64;
	}
	float *pfca=(float *)(data+dataPos);
	pCamera=new Camera();
	//pCamera->setCamera(pfca[0],pfca[1],pfca[2], pfca[3],pfca[4],pfca[5], pfca[6],pfca[7],pfca[8]);

	pCamera->setCamera(0.0f,5.0f,13.5f,
			0.0f, 1.2f,-3.5f,
			0.0f,1.0f,0.0f);

	//LOGI("c0:%f,%f,%f",pfca[0],pfca[1],pfca[2]);
	//LOGI("c1:%f,%f,%f",pfca[3],pfca[4],pfca[5]);
	//LOGI("c2:%f,%f,%f",pfca[6],pfca[7],pfca[8]);

	pCamera->m_PointVector = pCamera->m_View - pCamera->m_Position;
	inOutVec1=pCamera->m_PointVector*(1.0f/(float)frameAllGetIn);
	inOutVec2=-inOutVec1;


	//for(int i=0;i<6;i++)
	//{
	//	vMyButton.push_back(vButtonAll[i]);
	//}

	invX=vButtonAll[3]->pTransform->pTOmatrix->mMatrixQueue.back()[12]
	   - vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[12];
	invY=vButtonAll[3]->pTransform->pTOmatrix->mMatrixQueue.back()[13]
	   - vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[13];
	invZ=vButtonAll[3]->pTransform->pTOmatrix->mMatrixQueue.back()[14]
	   - vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[14];

	//pCamera->setCamera(0.5,50,-100, 0,0,0, 0, 1, 0);

	//dataPos+=9*sizeof(float);
	sceneNum=vButtonAll.size();

}

void SetScene::onSize(int w, int h){
	screenHeight=h;
	screenOffY=0;
	pCamera->setLook();
	getMatrix().multiplyMM(getGL().MVPMatrix, 0, getGL().PMatrix, 0, getGL().VMatrix, 0);

	for(int i=0; i<sceneNum;i++)
	{
		vButtonAll[i]->setCenter();

		vButtonAll[i]->id=i;

		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();

		ShowWord *pShowWord=new ShowWord();
		//pShowWord->setShowNum(i+1);
		pShowWord->pTransform->pTOmatrix->mMatrixQueue.back()[12]
		   =vButtonAll[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12];
		pShowWord->pTransform->pTOmatrix->mMatrixQueue.back()[13]
		   =vButtonAll[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13];
		pShowWord->pTransform->pTOmatrix->mMatrixQueue.back()[14]
		   =vButtonAll[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14]+1.0f;

		pShowWord->pTransform->pSOmatrix->mMatrixQueue.back()[0]=0.1;
		pShowWord->pTransform->pSOmatrix->mMatrixQueue.back()[5]=0.1;
		pShowWord->pTransform->pSOmatrix->mMatrixQueue.back()[10]=0.1;

		pShowWord->calculateMatrix();
		pShowWord->setColor(0.0,0.0,-0.4);
		vpShowWord.push_back(pShowWord);
	}

	float pfTemp20[20];

	pfTemp20[0] = vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[12];
	pfTemp20[1] = vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[13];
	pfTemp20[2] = vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[14];
	pfTemp20[3] = 1.0f;

	getMatrix().multiplyMV(pfTemp20, 0, getGL().MVPMatrix, 0, pfTemp20, 0);

	pfTemp20[3] = vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[12]
	              - pCamera->m_Position.x;
	pfTemp20[4] = vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[13]
	              - pCamera->m_Position.y;
	pfTemp20[5] = vButtonAll[0]->pTransform->pTOmatrix->mMatrixQueue.back()[14]
	              - pCamera->m_Position.z;

	pfTemp20[6] = pfTemp20[0] - pCamera->m_Position.x;
	pfTemp20[7] = pfTemp20[1] - pCamera->m_Position.y;
	pfTemp20[8] = pfTemp20[2] - pCamera->m_Position.z;

	viewRadio=sqrt( (pfTemp20[3]*pfTemp20[3])+(pfTemp20[4]*pfTemp20[4])+(pfTemp20[5]*pfTemp20[5]) )/
			sqrt( (pfTemp20[6]*pfTemp20[6])+(pfTemp20[7]*pfTemp20[7])+(pfTemp20[8]*pfTemp20[8]) );

	viewRadio=viewRadio*2.0f/w;

	LOGI("onSurfaceChanged : end");
}

void SetScene::onUpdate(){

	getGL().setPGPNow(getGL().pGLSL_BASE);
	getGL().startFrame();

	//pRenderManager->clear();
	//getGL().setPGPNow(getGL().pGLSL_BASE);

	//getGL().PMatrix=PMatrix;

	//getGL().useProgram();
	//getGL().setMode(NORMAL);
	isRunning=true;

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//pCamera->setCamera(	0, 0, -10,0, 0, 0, 0,1, 0);
	//getGL().setLookAt( 0,230, 0,  0,0,0,  0,0,1);
	for(int i=0; i<sceneNum;i++)
	{
		vButtonAll[i]->setActive(true);
		if(gameMode==GAMEMOVE_LEVEL&&i>=gameLevelRecord)
		vButtonAll[i]->setActive(false);
	}

	pCamera->setLook();
	updateBackground();
	switch(state)
	{
	case STATE_FIX:
		updateFix();
		break;
	case STATE_GETIN:
		updateGetIn();
		break;
	case STATE_GETOUT:
		updateGetOut();
		break;
	case STATE_TRANSLATE:
		break;
	}

	//pRenderManager->render();
	//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();
	//vSquare[0]->draw();
}

void SetScene::updateBackground()
{
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	//getGL().rotate(1.0f,0,0,90.0f);
	pgBackground->drawNoMovement();
}

void SetScene::updateGetIn()
{
	if(frameGetIn==0)
	{
		for(int i=0; i<sceneNum;i++)
		{
			vButtonAll[i]->resetPosMatrix();
			vpShowWord[i]->resetPosMatrix();
			vButtonAll[i]->move(-1*pCamera->m_PointVector.x, -1*pCamera->m_PointVector.y, -1*pCamera->m_PointVector.z);
			vpShowWord[i]->move(-1*pCamera->m_PointVector.x, -1*pCamera->m_PointVector.y, -1*pCamera->m_PointVector.z);
		}
	}
	if(frameGetIn<frameAllGetIn)
	{
		frameGetIn++;
	}
	else
	{
		//for(int i=0; i<sceneNum;i++)
		//{
		//	vButtonAll[i]->resetPosMatrix();
		//	vpShowWord[i]->resetPosMatrix();
		//}
		frameGetIn=0;
		state=STATE_FIX;
	}

    //pCamera->m_View.x+=inOutVec2.x;
    //pCamera->m_View.y+=inOutVec2.y;
    //pCamera->m_View.z+=inOutVec2.z;

    //pCamera->m_Position.x+=inOutVec2.x;
    //pCamera->m_Position.y+=inOutVec2.y;
    //pCamera->m_Position.z+=inOutVec2.z;


    //pgBackground->move(inOutVec2.x, inOutVec2.y, inOutVec2.z);

    //return;

	for(int i=0; i<sceneNum;i++)
	{
		//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
		//getGL().chooseMatrix(GLSL::MMATRIX);
		//getGL().setIdentity();
		vButtonAll[i]->move(inOutVec1.x, inOutVec1.y, inOutVec1.z);
		vButtonAll[i]->update();

		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();
		vpShowWord[i]->move(inOutVec1.x, inOutVec1.y, inOutVec1.z);
		if((i+1)>=10)
			vpShowWord[i]->draw((i+1)/10,(i+1)%10);
		else
			vpShowWord[i]->draw((i+1));
		getGL().material->drawNoMaterial();
	}
}

void SetScene::updateGetOut()
{
	if(frameGetIn==0)
		{
			pMjni->afterLayoutLoad(pGame->numCrystalAll,pGame->gameLevelNow);
		}
		if(frameGetIn<frameAllGetIn)
		{
			frameGetIn++;
		}
		else
		{
			//for(int i=0; i<sceneNum;i++)
			//{
				//vButtonAll[i]->resetPosMatrix();
				//vpShowWord[i]->resetPosMatrix();
			//}
			frameGetIn=0;
			state=STATE_FIX;
			pGame->startGame();
			pAppScene=pGame;
			SysSceneType=SCENETYPE_GAME;
		}

	   // pCamera->m_View.x+=inOutVec1.x;
	   // pCamera->m_View.y+=inOutVec1.y;
	   // pCamera->m_View.z+=inOutVec1.z;

	   // pCamera->m_Position.x+=inOutVec1.x;
	   // pCamera->m_Position.y+=inOutVec1.y;
	   // pCamera->m_Position.z+=inOutVec1.z;


	    //pgBackground->move(inOutVec1.x, inOutVec1.y, inOutVec1.z);

	    //return;

		for(int i=0; i<sceneNum;i++)
		{
			//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
			//getGL().chooseMatrix(GLSL::MMATRIX);
			//getGL().setIdentity();
			vButtonAll[i]->move(inOutVec2.x, inOutVec2.y, inOutVec2.z);
			vButtonAll[i]->update();

			getGL().chooseMatrix(GLSL::MMATRIX);
			getGL().setIdentity();
			vpShowWord[i]->move(inOutVec2.x, inOutVec2.y, inOutVec2.z);
			if((i+1)>=10)
				vpShowWord[i]->draw((i+1)/10,(i+1)%10);
			else
				vpShowWord[i]->draw((i+1));
			getGL().material->drawNoMaterial();
		}


}

void SetScene::updateFix()
{
	float pColor[3]={0.6,0.2,0.6};

	for(int i=0; i<sceneNum;i++)
	{

		//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
		//getGL().chooseMatrix(GLSL::MMATRIX);
		//getGL().setIdentity();
		vButtonAll[i]->update();

		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();

		if((i+1)>=10)
			vpShowWord[i]->draw((i+1)/10,(i+1)%10);
		else
			vpShowWord[i]->draw((i+1));
		getGL().material->drawNoMaterial();
	}

}

void SetScene::onTouchDown(float x, float y){

	touchState=TOUCH_DOWN;
	firstTouchLast.x=x;
	firstTouchLast.y=y;
	touchDownX=x;
	touchDownY=y;

	//LOGI("xy0: %f, %f",	x,	y );+screenOffY
	//if(state=STATE_FIX)
	for(int i=0; i<sceneNum;i++)
	{
		if(vButtonAll[i]->onTouchDown(x, y))
		{
			//LOGI("touchDown: %d",i);
			//pAppScene=pGame;
			return;
		}
	}

}

void SetScene::onTouchUp(float x, float y){
	//LOGI("y+screenOffY: %d,%d",screenOffY,(int)(y+screenOffY));
	if(touchState==TOUCH_MOVE)
	{
		getMatrix().multiplyMM(getGL().MVPMatrix, 0, getGL().PMatrix, 0, getGL().VMatrix, 0);
		for(int i=0; i<sceneNum;i++)
		{
			vButtonAll[i]->setCenter();
			//vButtonAll[i]->onTouchUpLess();
		}
	}
	else if(state==STATE_FIX)
	for(int i=0; i<sceneNum;i++)
	{
		vButtonAll[i]->onTouchUp(x, y);
		//if(vButtonAll[i]->onTouchUp(x, y))+screenOffY
		//{
			//LOGI("touchUp: %d",i);
			//pAppScene=pGame;
		//	return;
		//}
	}
	touchState=TOUCH_UP;
}

void SetScene::onTouchMove(float x, float y){
	//return;

	firstTouchNow.x=x;
	firstTouchNow.y=y;

	if(touchState==TOUCH_DOWN)
	if(	(firstTouchNow.y-touchDownY)*(firstTouchNow.y-touchDownY)
			+(firstTouchNow.x-touchDownX)*(firstTouchNow.x-touchDownX)
			>300)
	{
		//LOGI("onTouchMove 2");
		for(int i=0; i<sceneNum;i++)
		{
			vButtonAll[i]->onTouchUpLess();
		}
		touchState=TOUCH_MOVE;
	}

	//screenOffYLast=screenOffY;
	//screenOffY-=firstTouchNow.y-firstTouchLast.y;

	//380 3920
//LOGI("onTouchMove:%f",pCamera->m_View.y);

	float len=sqrt((firstTouchNow.y-firstTouchLast.y)*(firstTouchNow.y-firstTouchLast.y))*viewRadio;// /1000.0f;

	len=firstTouchNow.y < firstTouchLast.y ? len: -len;
	//wSize+=len;
	//LOGI("len :%f",pCamera->m_View.y+len*invY);
	//getMatrix().orthoM(PMatrix, 0, left*wSize, right*wSize, bottom*wSize, top*wSize, near, far);

	if((pCamera->m_View.y+len*invY)<2.0f
			&&(pCamera->m_View.y+len*invY)>(vButtonAll.back()->pTransform->pTOmatrix->mMatrixQueue.back()[13]-1))
	{
		pCamera->m_View.x+=len*invX;
		pCamera->m_View.y+=len*invY;
		pCamera->m_View.z+=len*invZ;

		pCamera->m_Position.x+=len*invX;
		pCamera->m_Position.y+=len*invY;
		pCamera->m_Position.z+=len*invZ;


		pgBackground->move(len*invX,len*invY,len*invZ);
	}
	firstTouchLast.x=x;
	firstTouchLast.y=y;
}

void SetScene::back()
{
	//exit(0);
}

void SetScene::moveVScene(int id, float x, float y, float z)
{
	//for(int i=0;i<vvButtonAll[id]->size();i++)
	//{
		//vvButtonAll[id]->at(i)->move(x,y,z);
	//}
}
