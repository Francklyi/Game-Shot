#include "SceneTutorial.h"
#include "GLSL/GLSL.h"
#include "log.h"
#include "Camera.h"
#include "fileDeal/loadFile.h"
#include "commond.h"

SceneTutorial::SceneTutorial(){
	firstTouchLast=Vector3();
	firstTouchNow=Vector3();
	secorndTouchNow=Vector3();
	secorndTouchLast=Vector3();

	vSquare=vector<Square*>(0);
	vgPlanes=vector<OptionGraph*>(0);
	pgBackground=0;
	vpShowWord=vector<ShowWord*>(0);
	//pShowWord=new ShowWord();
}

void SceneTutorial::onCreate(){
	//pg_background=new Graph();
	//char fileName[]="skybox3.ly";
	//pg_background->setFileName(fileName);
	//pg_background->load();

	//Square* pSquare=new Square();
	//vSquare.push_back(pSquare);

	char filgpath[] = "choose3.lyout";//"fight7.lyout";//combineChars(layoutPath, pSceneName);
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

		for(int i=0; i<NumEntity; i++)
		{
			OptionGraph *pGTemp = new OptionGraph();
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
				pgBackground=pGTemp;
			else if(pGTemp->roleType==Graph::ROLE_FIXED1)
				vgPlanes.push_back(pGTemp);

			//pGMain->graphList.push_back(gdata2);
			//dataPos=onShapeRead(gdata2,data,dataPos);
		}
		free(strFN);
		//dataPos+=64;
	}
	float *pfca=(float *)(data+dataPos);
	pCamera=new Camera();
	pCamera->setCamera(pfca[0],pfca[1],pfca[2], pfca[3],pfca[4],pfca[5], pfca[6],pfca[7],pfca[8]);
	//pCamera->setCamera(0.5,50,-100, 0,0,0, 0, 1, 0);

	dataPos+=9*sizeof(float);
	LOGI("setCamera 1");

}

void SceneTutorial::onSize(int w, int h){

	pCamera->setLook();
	getMatrix().multiplyMM(getGL().MVPMatrix, 0, getGL().PMatrix, 0, getGL().VMatrix, 0);

	for(int i=0; i<vgPlanes.size();i++)
	{
		vgPlanes[i]->setData();

		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();

		ShowWord *pShowWord=new ShowWord();
		//pShowWord->setShowNum(i+1);
		pShowWord->pTransform->pTOmatrix->mMatrixQueue.back()[12]
		   =vgPlanes[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12];
		pShowWord->pTransform->pTOmatrix->mMatrixQueue.back()[13]
		   =vgPlanes[i]->pTransform->pTOmatrix->mMatrixQueue.back()[13];
		pShowWord->pTransform->pTOmatrix->mMatrixQueue.back()[14]
		   =vgPlanes[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14];
		pShowWord->pTransform->pSOmatrix->mMatrixQueue.back()[0]=0.1;
		pShowWord->pTransform->pSOmatrix->mMatrixQueue.back()[5]=0.1;
		pShowWord->pTransform->pSOmatrix->mMatrixQueue.back()[10]=0.1;
		pShowWord->calculateMatrix();
		pShowWord->setColor(0.0,0.0,-0.4);
		vpShowWord.push_back(pShowWord);
	}

	LOGI("onSurfaceChanged : end");
}

void SceneTutorial::onUpdate(){

	//getGL().setPGPNow(getGL().pGLSL_BASE);

	//getGL().PMatrix=PMatrix;

	//getGL().useProgram();
	getGL().setMode(NORMAL);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//pCamera->setCamera(	0, 0, -10,0, 0, 0, 0,1, 0);
	//getGL().setLookAt( 0,230, 0,  0,0,0,  0,0,1);

	pCamera->setLook();

	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	pgBackground->draw();

	for(int i=0; i<vgPlanes.size();i++)
	{
		getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();
		vgPlanes[i]->draw();

		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();

		vpShowWord[i]->draw();
	}


	//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();
	//vSquare[0]->draw();
}

void SceneTutorial::onTouchDown(float x, float y){
	firstTouchLast.x=x;
	firstTouchLast.y=y;
	LOGI("xy0: %f, %f",	x,	y );
	for(int i=0; i<vgPlanes.size();i++)
	{
		if(vgPlanes[i]->isTouch(x, y))
		{
			LOGI("touch: %d",i);
			pAppScene=pGame;
			return;
		}
	}

}

void SceneTutorial::onTouchUp(float x, float y){

}

void SceneTutorial::onTouchMove(float x, float y){
	firstTouchNow.x=x;
	firstTouchNow.y=y;

    float len=sqrt((firstTouchNow.x-firstTouchLast.x)*(firstTouchNow.x-firstTouchLast.x)
                    +(firstTouchNow.y-firstTouchLast.y)*(firstTouchNow.y-firstTouchLast.y))/500;

    len=firstTouchNow.x > firstTouchLast.x ? len: -len;
    wSize+=len;
    //LOGI("wSize:%f",wSize);
    //getMatrix().orthoM(PMatrix, 0, left*wSize, right*wSize, bottom*wSize, top*wSize, near, far);

	firstTouchLast.x=x;
	firstTouchLast.y=y;
}
