#include "LeadRole.h"
#include "Vector3.h"
#include <math.h>
#include "math/m_math.h"
#include "ArmyRole.h"
#include "commond.h"
#include "Camera.h"
#include "log.h"
#include "MJNI.h"
#include "roles/Bullet0.h"
#include "roles/BulletSmall.h"

LeadRole::LeadRole(char * fGraphFile):
funcDealMovement(0)
//,pMoveNow(0)
,command(0)
,lengthToEnemy(1000)
,pEnemyR(0)
,jumpForwardZ(0)
,coinNum(0)
,pG_hitme(0)
,sceneBelowID(0)
,numID(0)
,Role()
{
	sceneID=0;
	setGlslFuncType(GLSL::FUNS_LIGHT_PHONG);
	sceneIDLast=0;
	sceneIDNow=0;
	pLine=new Line();
	pvEnemy=0;
	vCommands=vector<int>(0);
	//pG=new Graph();
	setFileName(fGraphFile);
	load();
	vAims=vector<Role*>(0);

	vCommand=deque<int>(20);
	//vCommand.reserve(15);
	vCommand.clear();
	pfMoveSky[0]=0;
	pfMoveSky[1]=0;
	pfMoveSky[2]=0;
	//vMovement=vector<Movement*>(0);
}

LeadRole::~LeadRole()
{
	//if(pG) free(pG);
	//pG=0;
}

void LeadRole::setVEnemy(vector<ArmyRole*> *pvEnemy_)
{
	pvEnemy=pvEnemy_;
}

void LeadRole::setEnemyRace(GraphRace *pEnemyR_)
{
	pEnemyR=pEnemyR_;
}

void LeadRole::addCommand(int cmd)
{
	vCommand.push_back(cmd);
	//act(cmd);
	//pAFG->change_stay = 1;
	//vCommands.push_back(cmd);
	//LOGI("addCommand:%d",cmd);
	//dealCommands(cmd);
	//command=cmd;
	//LOGI("12345");
}

void LeadRole::clearCommands()
{
	//vCommands.clear();
}

void LeadRole::dealCommands(int cmd)
{
	//if(command==ID_JUMP1&&pAFG->change_stay != 0)
	//	return;
	switch(cmd)
	{
	case ID_RUN:
		act(cmd);
		pAFG->change_stay = 1;
		break;
	case ID_HIT1:
		act(cmd);
		pAFG->change_stay = 1;
		break;
	case ID_HIT4:
		if(pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight<1.0)
		if(pAFG->frame!=0)
		{
			if(pAFG->frame>5&&
					pAFG->frame<20)
			{
				pAFG->startFrameNum(21);//frame=21;
			}
			else
				if(pAFG->frame>25&&
					pAFG->frame<38)
				{
					pAFG->startFrameNum(5);//frame=5;
				}
		}
		else
		{
			act(cmd);
			pAFG->change_stay = 1;
		}
		break;
	case ID_JUMP1://
	case ID_JUMP_SHORT:
		//if(command!=ID_JUMP1||command!=ID_JUMP_SHORT||pAFG->change_stay == 0)
		if(pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight<1.0)
		{
			//LOGI("jump");
				lastPos[0]=pTransform->pTOmatrix->mMatrixQueue.back()[12]
				                                                      -pGame->vpScene[sceneIDLast]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12];
				lastPos[1]=pTransform->pTOmatrix->mMatrixQueue.back()[13]
				                                                      -pGame->vpScene[sceneIDLast]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[13];
				lastPos[2]=pTransform->pTOmatrix->mMatrixQueue.back()[14]
				                                                      -pGame->vpScene[sceneIDLast]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14];

				//lastPos[0]=pTransform->pTOmatrix->mMatrixQueue.back()[12];
				//lastPos[1]=pTransform->pTOmatrix->mMatrixQueue.back()[13];
				//lastPos[2]=pTransform->pTOmatrix->mMatrixQueue.back()[14];

			act(cmd);
		}
		break;
	default:
		//if(pAFG->change_stay == 0)
		if(pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight<1.0)
		act(cmd);
		//pAFG->change_stay = 1;
		break;
	}
	//vCommands.pop_back();
}

void LeadRole::setMovements()
{
	char aFile_run[]="run_test9_1.amn\0";//"walkman6.amn\0";
	char aFile_standmove[]="standmove1_2.amn\0";

	char file_jumphigh[]="jump_new5_1.amn\0";//"jump_forward4_1.amn\0";//"jump_test5_1.amn\0";//"walkManNew1.amn\0";
	char aFile2[]="hit_test4.amn\0";//"jump_hide2.amn\0";//"jump_test5_2.amn\0";
	char aFile3[]="behitted5_1.amn\0";
	char aFile4[]="hit_hand5_1.amn\0";
	char file_falldownup[]="falldownup1.amn\0";
	char aFile6[]="hit_hand_leg3_1.amn\0";
	char jump_short[]="jump_short1_1.amn\0";
	char aFile_shot1[]="shot1.amn\0";

	addMove( ID_RUN, aFile_run, Animation::TYPE_GROUND);

	int keyLen=5;
	int keyFrames[]={0,9,19,29,gp->vMovements.back()->getAnimation()->frameNum};

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}
	//pKF=new KeyFrame();
	//pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	//gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	addMove( ID_WALK1, aFile_standmove, Animation::TYPE_GROUND);

	KeyFrame *pKF=new KeyFrame();
	pKF->setKeyFrameNum(0);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	addMove( ID_JUMP1, file_jumphigh, Animation::TYPE_SKY);

	keyLen=5;
	int keyFrames1[]={0,10,26,27,28};

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames1[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}
	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);


	addMove( ID_HIT1, aFile_shot1, Animation::TYPE_GROUND);

	keyLen=5;
	int keyFrames_shot1[]={0,15,25,35,gp->vMovements.back()->getAnimation()->frameNum};

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames_shot1[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}

/*
	addMove( ID_HIT2, aFile2, Animation::TYPE_GROUND);
	int infoLen=3;
	int info0[3][7]={
			{9,Frame::TYPE_HIT1, 0,0,5, 36, 10},
			{18,Frame::TYPE_HIT1, 0,0,5, 36, 15},
			{32,Frame::TYPE_HIT2, 0,0,5, 36, 20},
	};

	for(int i=0; i<infoLen;i++)
	{
		gp->vMovements.back()->getAnimation()
						  ->pFrames[info0[i][0]].setInfo(info0[i]);
	}

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(0);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	addMove( ID_BEHIT1, aFile3, Animation::TYPE_GROUND);

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(0);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);


	addMove( ID_HIT3, aFile6, Animation::TYPE_GROUND);
	infoLen=3;
	int info[3][7]={
			{20,Frame::TYPE_HIT1, 0,0,5, 36, 10},
			{41,Frame::TYPE_HIT1, 0,0,5, 36, 15},
			{76,Frame::TYPE_HIT2, 0,0,0, 150, 20}
			//{53,Frame::TYPE_HIT1}
	};

	for(int i=0; i<infoLen;i++)
	{
		gp->vMovements.back()->getAnimation()
						  ->pFrames[info[i][0]].setInfo(info[i]);
	}

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(0);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	addMove( ID_HIT4, aFile4, Animation::TYPE_GROUND);

	infoLen=4;
	int info2[4][7]={
			{9,Frame::TYPE_HIT1, 0,0,5, 36, 10},
			{24,Frame::TYPE_HIT1, 0,0,5, 36, 12},
			{38,Frame::TYPE_HIT1, 0,0,5, 36, 15},
			{53,Frame::TYPE_HIT1, 0,0,5, 36, 18}
	};

	for(int i=0; i<infoLen;i++)
	{
		gp->vMovements.back()->getAnimation()
						  ->pFrames[info2[i][0]].setInfo(info2[i]);
	}

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(0);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
*/
	addMove( ID_FALLDOWN1, file_falldownup, Animation::TYPE_GROUND);

	keyLen=3;
	int keyFrames2[]={0,16,26,40};

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames2[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}

	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);


	addMove( ID_JUMP_SHORT, jump_short, Animation::TYPE_SKY);

	keyLen=3;
	int keyFrames3[]={0,10,29};

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames3[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}
	pKF=new KeyFrame();
	pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	setVpAFG();

	delete[] gp->vp_;
	gp->vp_=0;
}

void LeadRole::makeTransform()
{
	//LOGI("lm0");
	//float *pos=pTransform->pTOmatrix->mMatrixQueue.back();
	//LOGI("pos 0 : %f, %f, %f ", pos[12],pos[13],pos[14]);
	if(gp)
	{
		//LOGI("lm1 sceneID:%d",sceneID);
		if(sceneID!=-1)
		if(pGame->vpScene[sceneID]->pG_floor->pMovepath)
		{
			pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pGame->vpScene[sceneID]->pfMoveStepNow[0];
			pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pGame->vpScene[sceneID]->pfMoveStepNow[1];
			pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pGame->vpScene[sceneID]->pfMoveStepNow[2];

		}
		//LOGI("lm2");
		nowPos[0]=pTransform->pTOmatrix->mMatrixQueue.back()[12];
		nowPos[1]=pTransform->pTOmatrix->mMatrixQueue.back()[13];//+bodyHeight
		nowPos[2]=pTransform->pTOmatrix->mMatrixQueue.back()[14];


		//LOGI("getSceneID 0");
		getSceneID();
		//LOGI("getSceneID 1");
		if(gp->jointNump>0 && gp->animations){
			if (pAFG->change_stay == 0) {

				moveState=STATE_STOP;

				pTransform->pTOmatrix->mMatrixQueue.back()[13]=pPScene->h;

				nowPos[0]=pTransform->pTOmatrix->mMatrixQueue.back()[12];
				nowPos[1]=pTransform->pTOmatrix->mMatrixQueue.back()[13];//+bodyHeight
				nowPos[2]=pTransform->pTOmatrix->mMatrixQueue.back()[14];
				//LOGI("m01");
			} else {
				//if(moveCode!=ID_RUN)

				if(pAFG->frame!=pAFG->lastFrame)
				{
					//LOGI("pos 0 : %f, %f, %f ", pos[12],pos[13],pos[14]);
					calVelocity();

					if(moveCode==ID_FALLDOWN1)
					{
						/*

						if(false&&pAFG->frame>20)
							if(sceneID!=-1)
								if(!pPScene->isInside(nowPos,12.0f))
								{
									//LOGI("out");
									floorHeight=-100;
									command=ID_JUMP1;
									act(ID_JUMP1);
									pAFG->startFrameNum(38);//frame=38;
									pAFG->change_stay = 1;
								}
*/
						//LOGI("m12");
					}
					else
					{
						if(sceneID!=-1){
							//if(pPScene->isInside(nowPos,12.0f))
							//{
								floorHeight=pPScene->h-0.5;
								pPScene->calculate(nowPos, nextPos,5.0f);

								if(nextPos[1]<floorHeight)
									if(moveCode!=ID_FALLDOWN1)
										nextPos[1]=floorHeight;

								//nextPos[1]=pPScene->h;
							//}
						}
						else
						{
							floorHeight=-100;
							//nextPos[1]-=1;
							if(command==ID_JUMP1||command==ID_JUMP_SHORT)
							{

							}else
							{
								command=ID_JUMP1;
								act(ID_JUMP1);
								pAFG->startFrameNum(26);//frame=38;
								pAFG->change_stay = 1;
								//addCommand(ID_JUMP1);
								//pAFG->startFrameNum(38);//frame=38;
							}
						}
						//LOGI("m12");
					}
					//if(abs(nowPos[1]-floorHeight)>3)
					//	posState=POSSTATE_SKY;
					//else
					//	posState=POSSTATE_GROUND;
					//LOGI("m13");
					if(nextPos[1]<(floorHeight>10?-10: -40))
					{
						//跳出
						//HP-=5;
						//pfTemp=pGame->vpScene[sceneIDLast]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back();
						nextPos[0]=pGame->vpScene[sceneIDLast]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[12];//lastPos[0]+
						nextPos[1]=pGame->vpScene[sceneIDLast]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[13];//lastPos[1]+
						nextPos[2]=pGame->vpScene[sceneIDLast]->pG_floor->pTransform->pTOmatrix->mMatrixQueue.back()[14];//lastPos[2]+

						if(pGame->vpScene[sceneIDLast]->pG_floor->pMovepath)
						{
							nextPos[0]+=pGame->vpScene[sceneIDLast]->pfMoveStepNow[0];
							nextPos[1]+=pGame->vpScene[sceneIDLast]->pfMoveStepNow[1];
							nextPos[2]+=pGame->vpScene[sceneIDLast]->pfMoveStepNow[2];

						}
						command=ID_RUN;
						act(ID_RUN);
						moveState= STATE_STOP;
						//pAFG->end();//frame=0;
						//pAFG->change_stay = 0;
					}
					//LOGI("pos 2 : %f, %f, %f ", pos[12],pos[13],pos[14]);
					pTransform->pTOmatrix->mMatrixQueue.back()[12]=nextPos[0];
					pTransform->pTOmatrix->mMatrixQueue.back()[13]=nextPos[1];//-bodyHeight
					pTransform->pTOmatrix->mMatrixQueue.back()[14]=nextPos[2];

					//pos=pTransform->pTOmatrix->mMatrixQueue.back();
					//LOGI("pos30 : %f, %f, %f , %f", pos[0],pos[1],pos[2],pos[3]);
					//LOGI("pos31 : %f, %f, %f , %f", pos[4],pos[5],pos[6],pos[7]);
					//LOGI("pos32 : %f, %f, %f , %f", pos[8],pos[9],pos[10],pos[11]);
					//LOGI("pos33 : %f, %f, %f , %f", pos[12],pos[13],pos[14],pos[15]);
				}

				//LOGI("m2");
			}

		}
	}


	//LOGI("lm10");
	pfTemp=pTransform->pTOmatrix->mMatrixQueue.back();
	//float eh;
	if((moveCode==ID_JUMP1||moveCode==ID_JUMP_SHORT||moveCode==ID_FALLDOWN1)&&pAFG->change_stay != 0)
		camera1->m_View.y=pfTemp[13]+10.0f;
	else
		camera1->m_View.y=pPScene->h+10.0f;
//LOGI("h:%f",camera1->m_View.y);
	camera1->setCamera(

			//camera1->m_Position.x,camera1->m_Position.y,camera1->m_Position.z,
			(camera1->m_Position.x-camera1->m_View.x +pfTemp[12]),
			camera1->m_View.y+10.0f,//pfTemp[13]+20,//pCamera->m_Position.y,
			(camera1->m_Position.z-camera1->m_View.z +pfTemp[14]),
			pfTemp[12],
			camera1->m_View.y,//0.6f*(pfTemp[13]+pPScene->h)+10,
			pfTemp[14],
			0.0f,1.0f, 0.0f);
	//getGL().setLookAt( 0,230, 0,  0,0,0,  0,0,1);

	camera1->setLook();

	//LOGI("m3");
	//LOGI("pos : %f, %f, %f ", nextPos[0],nextPos[1],nextPos[2]);

	//LOGI("pos 4 : %f, %f, %f ", pos[12],pos[13],pos[14]);
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();

}

void LeadRole::update()
{
	//LOGI("L0");
	//LOGI("pAFG->frame:%d, %d, %d",command,pAFG->lastFrame,pAFG->frame);
	//getEnviromentInfo();

	pAFG->getFrameNum();
	//getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();
	//LOGI("L1");
	if(vCommand.size()>0)
	{

		dealCommands(vCommand.front());
		command=vCommand.front();
		vCommand.pop_front();
	}
	//LOGI("L1.1");
	if(pAFG->frame!=pAFG->lastFrame)
	onUpdateStart();

	//float *pos=pTransform->pTOmatrix->mMatrixQueue.back();
	//LOGI("pos 0: %f, %f, %f ", pos[12],pos[13],pos[14]);
	if(hasChange)
	{
		setRotateY(changeRotateY);

		hasChange=false;
	}
	gp->pmtls[0]->mTextureId=getGL().shadowTexId;
	//LOGI("L1.2");
	draw();
	//LOGI("L2");

	if(pAFG->frame!=pAFG->lastFrame)
	onUpdateEnd();

	//LOGI("pos 1: %f, %f, %f ", pos[12],pos[13],pos[14]);
	//LOGI("L1");
	//LOGI("L3");
    //if(moveCode==2) dealMovementRun1();

}

void LeadRole::onUpdateStart()
{
	dealMovements0(moveCode);
}

void LeadRole::onUpdateEnd()
{
	dealMovements1(moveCode);
}

void LeadRole::reset()
{
	Role::reset();
	//getMatrix().setIdentityM(pTransform->pTOmatrix->mMatrixQueue.back(),0);
	//pTransform->pTOmatrix->mMatrixQueue.back()[12]=0;
	//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;//-bodyHeight
	//pTransform->pTOmatrix->mMatrixQueue.back()[14]=0;
	pAFG->startFrameNum(0);
	pAFG->change_stay = 0;
	HP=20;
	coinNum=0;
	pAFG->change_stay = 0;
	sceneID=0;
	sceneBelowID=0;

	pfMoveSky[0]=0;
	pfMoveSky[1]=0;
	pfMoveSky[2]=0;
}

void LeadRole::setSceneID(int sceneID)
{

}

int LeadRole::getSceneID()
{
	xi=(pTransform->pTOmatrix->mMatrixQueue.back()[12] - pGame->pTerrain->mapX0)/pGame->pTerrain->mapCellWidth;
	yi=(pTransform->pTOmatrix->mMatrixQueue.back()[14] - pGame->pTerrain->mapY0)/pGame->pTerrain->mapCellWidth;
	terrainCellNum=yi*(pGame->pTerrain->mapWidth-1)+xi;
	pGame->pTerrain->cellNumLR=terrainCellNum;

	if(pGame->pTerrain->pIScenes[terrainCellNum]==-1)
	{
		if(sceneID!=-1 && !(pGame->vpScene[sceneID]->pG_floor->pMovepath))
			sceneIDLast=sceneID;
		floorHeight=-100;
		//LOGI("Terrain Cell: %d",terrainCellNum );
		sceneID=-1;
		return sceneID;
	}

	floorHeight=-100.0f;

	//float dhMin=pGame->vpScene[pGame->pTerrain->vSceneIDs[terrainCellNum]->at(0)];
	numID=0;
	sceneBelowID=-1;
	for(int i=0;i<pGame->pTerrain->pISceneIDs[pGame->pTerrain->pIScenes[terrainCellNum]];i++)
	{
		numID=pGame->pTerrain->pISceneIDs[pGame->pTerrain->pIScenes[terrainCellNum]+i+1];

		if(pGame->vpScene[numID]->pG_floor->pMovepath)
		{
			pGame->vpScene[numID]->renewPhyScene();
		}

		if(pGame->vpScene[numID]->pPScene->isInside(nowPos,10.0f) )
		{
			if(floorHeight>(pGame->vpScene[numID]->pPScene->h-0.5f))
				continue;
			//LOGI("numID:%d",numID);
			floorHeight=pGame->vpScene[numID]->pPScene->h-0.5f;
			sceneBelowID=numID;
			//if(sceneID!=numID)
		}

	}
	if(sceneBelowID!=-1)
	{
		if((fabsf(nowPos[1]-pGame->vpScene[sceneBelowID]->pPScene->h))<10.0f)
		{
			if(sceneID!=-1 && !(pGame->vpScene[sceneID]->pG_floor->pMovepath))
				sceneIDLast=sceneID;
			sceneID=sceneBelowID;

			setVEnemy(&(pGame->vpScene[sceneID]->vEnemy));
			//LOGI("scene id: %d",sceneID);
			floorHeight=pGame->vpScene[sceneID]->pPScene->h-0.5f;



			if(pGame->vpScene[sceneID]->pG_floor->pMovepath&&
					!(pGame->vpScene[sceneID]->pG_floor->pMovepath->isMoving))
			{
				if((pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight)<1.0f)
					pGame->vpScene[sceneID]->pG_floor->pMovepath->isMoving=true;
			}
			setPhyScene( pGame->vpScene[sceneID]->pPScene );
			if(moveCode!=ID_JUMP1)
			{
				sceneIDNow=sceneID;
			}
			return sceneID;
		}
	}

	if(sceneID!=-1)
		if(!(pGame->vpScene[sceneID]->pG_floor->pMovepath))
		sceneIDLast=sceneID;
	floorHeight=-100;
	sceneID=-1;

	return sceneID;
}

void LeadRole::dealMovements0(int ID)
{
	//float pfs[4]={0,0,0,1};
	switch(ID)
	{
	case ID_RUN:
		dealMovementRun1();
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	case ID_JUMP1:
		if(sceneID==-1)
		//if(pGame->vpScene[sceneIDLast]->pG_floor->pMovepath)
		{
			pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveSky[0];
			if(pfMoveSky[1]>0) pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveSky[1];
			pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveSky[2];

		}
		dealMovementJump1();
		break;
	case ID_JUMP_SHORT:
		if(sceneID==-1)
		//if(pGame->vpScene[sceneIDLast]->pG_floor->pMovepath)
		{
			pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pfMoveSky[0];
			if(pfMoveSky[1]>0) pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pfMoveSky[1];
			pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pfMoveSky[2];

		}
		dealMovementJumpShort();
		break;
	case ID_HIT1:
		if(pAFG->frame%5==0){//25
			float *pf4=new float[4];
			pf4[0]=-2.125f;
			pf4[1]=13.501f;
			pf4[2]=6.652f;
			pf4[3]=1.0f;
			pTransform->pTOmatrix->multiplyMV(pf4);
			//gp->getFramePointer(pf3, gp->vMovements.back()->getAnimation()
			//		, pAFG->frame,675);
			//LOGI("pos1: %f, %f, %f",pf4[0],pf4[1],pf4[2]);
			Bullet0* pB1=new BulletSmall();
			float *pf3=new float[3];
			pf3[0]=pf4[0]-pTransform->pTOmatrix->mMatrixQueue.back()[12];
			pf3[1]=pf4[1]-pTransform->pTOmatrix->mMatrixQueue.back()[13];
			pf3[2]=pf4[2]-pTransform->pTOmatrix->mMatrixQueue.back()[14];
			pB1->init(pTransform->pTOmatrix->mMatrixQueue.back(), pf3);
			pGame->pBS->addBullet(pB1);
			delete pf4;
			pf4=0;
			delete pf3;
			pf3=0;
		}
		break;
	case ID_HIT2:
	case ID_HIT4:
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		//if(pAFG->frame==0)
		//turnToEnemy();
		break;
	case ID_FALLDOWN1:

		dealFallDown();
		//if(pAFG->frame==1)
		//turnToEnemy();
		break;
	case ID_BEHIT1:
		if(pAFG->frame==1)
		turnToEnemy();
		break;
	}
}

void LeadRole::dealMovements1(int ID)
{
	float pfs[4]={0,0,5,1};
	switch(ID)
	{
	case ID_RUN:

		break;
	case ID_HIT2:
	case ID_HIT3:
	case ID_HIT4:
		//LOGI("123");
		if(ID==ID_HIT4)
		if(pAFG->frame==20 || pAFG->frame==34 )
		{
						pAFG->startFrameNum(57);//frame=57;
		}
		pFrameNow=&(gp->animations->pFrames[pAFG->frame]);//frame
	    if(pFrameNow->getType()!=Frame::TYPE_NORMAL)
	    {

	    	floats1[0]=pFrameNow->pos[0];
	    	floats1[1]=pFrameNow->pos[1];
	    	floats1[2]=pFrameNow->pos[2];
	    	floats1[3]=1;

	    	getMatrix().multiplyMV(floats1,0, pTransform->pTOmatrix->mMatrixQueue.back(), 0,  floats1, 0);

	    	float pos1X=floats1[0];//pTransform->pTOmatrix->mMatrixQueue.back()[12]+
	    	//float pos1Y=pTransform->pTOmatrix->mMatrixQueue.back()[13];
	    	float pos1Z=floats1[2];//pTransform->pTOmatrix->mMatrixQueue.back()[14]+

	    	float pos2X;
	    	float pos2Z;
	    	float l;
	    	//LOGI("vAims.size(): %d",vAims.size() );

	    	if(pvEnemy)
	    	for(int i=0; i<pvEnemy->size(); i++)
	    	{
	    		//(ArmyRole*)(enemyRace->vpg[i]);
	    		pos2X=(*pvEnemy)[i]->pTransform->pTOmatrix->mMatrixQueue.back()[12];
		    	//float pos2Y=aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[13];
		    	pos2Z=(*pvEnemy)[i]->pTransform->pTOmatrix->mMatrixQueue.back()[14];


		    	l=(pos1X-pos2X)*(pos1X-pos2X)+(pos1Z-pos2Z)*(pos1Z-pos2Z);

		    	if(l<pFrameNow->hitLength2)
		    	{
		    		//LOGI("Len: %f\n /n",l );
		    		(*pvEnemy)[i]->behitted(pFrameNow->type,pFrameNow->hp);
		    	}
	    	}

	    }
	    	//
		break;
	case ID_FALLDOWN1:
		//dealFallDown();
		//LOGI("FALL: %d, %f",pAFG->frame,pTransform->pTOmatrix->mMatrixQueue.back()[13]);
		//if(pAFG->frame==40 )
		//{
		//	if(HP<=0)
		//	{
				//pMjni->fail();
				//state=2;
				//removeFromRace();
				//act(ID_GETUP1);
				//pAFG->frame=5;
				//pAFG->change_stay = 1;
		//	}
		//}else
		//if(pAFG->frame==gp->vMovements.back()->getAnimation()->frameNum-1 )
		//{
			//pTransform->pTOmatrix->mMatrixQueue.back()[13]=floorHeight;
		//}
		//if(pAFG->frame==0 )
		////pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	}
}

void LeadRole::dealMovementRun1()
{
	//LOGE("frame: %d",pAFG->frame);
	if(moveState==STATE_STOP)
		pAFG->startFrameNum(31);//frame=35;

	switch(pAFG->frame)
	{
	case 31:
		pAFG->end();//frame=0;
		pAFG->change_stay = 0;

	    //pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	//case 31:
	//	pAFG->end();//frame=0;
	//	pAFG->change_stay = 0;
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
	//	break;
	case 9:
	case 19:
		if(moveState==STATE_STOP)
		{
			//LOGI("moveState==STATE_STOP");
			pAFG->startFrameNum(31);//frame=33;
		}
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	case 29:
		if(moveState==STATE_RUNNING)
			pAFG->startFrameNum(10);//frame=10;
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	}
}

void LeadRole::dealMKick1()
{
	if(pAFG->frame==24)
	{
		pAFG->startFrameNum(54);//frame=54;
	}
}

void LeadRole::dealMovementJump1()
{
	if(pAFG->change_stay!=0)
	{
		getMatrix().translateM( pTransform->pTOmatrix->mMatrixQueue.back(),0,
				//pTransform->pTOmatrix->mMatrixQueue.back(),0,
				0, 0.0f, pSysTime->frameInterval*jumpForwardZ);
		//pTransform->pTOmatrix->mMatrixQueue.back()[14]+= pSysTime->frameInterval*jumpForwardZ;
		if(pAFG->frame<10)
		{
			if(hasChange)
				jumpForwardZ=1.0f;//0.7f
			if(sceneID!=-1)
			if(pGame->vpScene[sceneID]->pG_floor->pMovepath)
			{
				pfMoveSky[0]=pGame->vpScene[sceneID]->pfMoveStepNow[0];
				pfMoveSky[1]=pGame->vpScene[sceneID]->pfMoveStepNow[1];
				pfMoveSky[2]=pGame->vpScene[sceneID]->pfMoveStepNow[2];

			}else
			{
				pfMoveSky[0]=0;
				pfMoveSky[1]=0;
				pfMoveSky[2]=0;
			}
		}
		else
			if(pAFG->frame >=26 && pAFG->frame <28)
			{
				if (pAFG->frame ==27)// 最高 34， 下落中 35-40 ，落地41-47
				{
					//LOGI("frame ==39");

					float *floats11 =&(pMoveNow->getAnimation()->joints[0].Matrix_1[16* ( 26 )]);
					pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pSysTime->frameInterval*floats11[4];
					pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pSysTime->frameInterval*(floats11[5]-0.8f);
					pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pSysTime->frameInterval*floats11[6];
					pAFG->startFrameNum(26);
					//pAFG->frame=26;
					//pAFG->startFrame=26;
					//
					//frame=38;
					//changeMatrix();
				}
				if(pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight<1.0)
				{
					if(pGame->buttonState==Game::BUTTONSTATE_DOWN
							&& pGame->commandCode==ID_JUMP1)
					{
						addCommand(ID_JUMP1);
						//pAFG->startFrameNum(0);
						jumpForwardZ=0.0f;
					}
					else
					{
						//pAFG->frame=47;
						pAFG->end();//frame=0;
						pAFG->change_stay = 0;
						jumpForwardZ=0.0f;
						pTransform->pTOmatrix->mMatrixQueue.back()[13]=floorHeight;
						if(moveState== STATE_RUNNING)
						{
							//LOGI("CBTouchMove 2");
							command=ID_RUN;
							act(ID_RUN);
						}
					}
				}
			}
	}

}

void LeadRole::dealMovementJumpShort()
{//LOGI("dealMovementJumpShort:%d",pAFG->frame);
	if(pAFG->change_stay!=0)
	{
		getMatrix().translateM( pTransform->pTOmatrix->mMatrixQueue.back(),0,
				//pTransform->pTOmatrix->mMatrixQueue.back(),0,
				0, 0.0f, pSysTime->frameInterval*jumpForwardZ);
		//pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pSysTime->frameInterval*jumpForwardZ;
		if(pAFG->frame<10)
		{
			if(hasChange)
				jumpForwardZ=0.9f;
			if(sceneID!=-1)
			if(pGame->vpScene[sceneID]->pG_floor->pMovepath)
			{
				pfMoveSky[0]=pGame->vpScene[sceneID]->pfMoveStepNow[0];
				pfMoveSky[1]=pGame->vpScene[sceneID]->pfMoveStepNow[1];
				pfMoveSky[2]=pGame->vpScene[sceneID]->pfMoveStepNow[2];

			}else
			{
				pfMoveSky[0]=0;
				pfMoveSky[1]=0;
				pfMoveSky[2]=0;
			}
		}
		else
			if(pAFG->frame >26 && pAFG->frame <33)
			{
				if (pAFG->frame ==29)// 39 最高 26， 下落中 27-32 ，落地41-47
				{
					//LOGI("frame == %d",pAFG->frame);

					float *floats11 =&(pMoveNow->getAnimation()->joints[0].Matrix_1[16* ( 28 )]);
					pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pSysTime->frameInterval*floats11[4];
					pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pSysTime->frameInterval*(floats11[5]-0.6f);
					pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pSysTime->frameInterval*floats11[6];
					pAFG->startFrameNum(28);
					//pAFG->frame=28;
					//pAFG->startFrame=28;
					//
					//frame=38;
					//changeMatrix();
				}
				if(pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight<1.0)
				{
					if(pGame->buttonState==Game::BUTTONSTATE_DOWN
							&& pGame->commandCode==ID_JUMP_SHORT)
					{
						addCommand(ID_JUMP_SHORT);
						//pAFG->startFrameNum(0);
						jumpForwardZ=0.0f;
					}
					else
					{
						//pAFG->frame=47;
						pAFG->end();//frame=0;
						pAFG->change_stay = 0;
						jumpForwardZ=0.0f;
						pTransform->pTOmatrix->mMatrixQueue.back()[13]=floorHeight;
						if(moveState== STATE_RUNNING)
						{
							//LOGI("CBTouchMove 2");
							command=ID_RUN;
							act(ID_RUN);
						}
					}

				}
			}
	}

}

void LeadRole::dealFallDown()
{
	//LOGI("dealFallDown 0");
	if(pAFG->change_stay!=0)
	{
		//LOGI("FallDown %d",pAFG->frame);
		//if(pAFG->frame>0&&pAFG->frame<11)
		//{
			//
			//pTransform->pTOmatrix->mMatrixQueue.back()[13]=nowPos[1]+0.4f;
			//pTransform->pTOmatrix->mMatrixQueue.back()[13]+=0.2f;
		//}else
			//if(pAFG->frame >11 && pAFG->frame <32)
			//{
		if(pAFG->frame==1)
		{
			pTransform->pTOmatrix->mMatrixQueue.back()[13]+=1.0f;
			fallVel=0.4f;
		}
		else if(pAFG->frame>1&&pAFG->frame<24)
		{
			pTransform->pTOmatrix->mMatrixQueue.back()[13]+=0.2f;

		}else
				if (pAFG->frame ==26)// 最高 34， 下落中 35-40 ，落地41-47
				{
					//LOGI("frame ==39");

					//float *floats11 =&(pMoveNow->getAnimation()->joints[0].Matrix_1[16* ( 25 )]);
					//pTransform->pTOmatrix->mMatrixQueue.back()[12]+=pSysTime->frameInterval*floats11[4];
					//pTransform->pTOmatrix->mMatrixQueue.back()[13]+=pSysTime->frameInterval*floats11[5];
					//pTransform->pTOmatrix->mMatrixQueue.back()[14]+=pSysTime->frameInterval*floats11[6];
					fallVel+=0.2f;
					pTransform->pTOmatrix->mMatrixQueue.back()[13]-=fallVel;//0.5f;
					pAFG->startFrameNum(25);
					//
					//frame=38;
					//changeMatrix();
					if(sceneID!=-1&&pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight<-3.5)
					{
						//LOGI("out:%f",floorHeight);
						//{
						pAFG->startFrameNum(26);
						//pAFG->frame=47;
						//pAFG->end();//frame=0;
						//pAFG->change_stay = 0;
						//pTransform->pTOmatrix->mMatrixQueue.back()[13]=floorHeight;
						//}
					}
				}
				else if(pAFG->frame==40)
				{
					if(pTransform->pTOmatrix->mMatrixQueue.back()[13]-floorHeight<-8.3f)
					{
						pTransform->pTOmatrix->mMatrixQueue.back()[13]=floorHeight-8.3f;
					}
				}

			//}
	}
	//LOGI("dealFallDown 1");
}
//set the hit data
void LeadRole::hit()
{

}

//set be hitted data
void LeadRole::behitted(int type, float hp)
{
	//LOGI("behitted");
	HP-= 2; // remember change

	//turnToEnemy();

	if(HP<=0)
	{
		act(ID_FALLDOWN1);
		pAFG->change_stay = 1;
		//HP=20;
	}
	else
	{
		act(ID_BEHIT1);
		timeBetweenDelay=0;
		pAFG->change_stay = 1;
	}

}

void LeadRole::getEnviromentInfo()
{
	float * pos0=pTransform->pTOmatrix->mMatrixQueue.back();
	float * pos=aimRole->pTransform->pTOmatrix->mMatrixQueue.back();
	lengthToEnemy= (pos0[12]-pos[12])*(pos0[12]-pos[12])
			+(pos0[13]-pos[13])*(pos0[13]-pos[13])
			+(pos0[14]-pos[14])*(pos0[14]-pos[14]);

}

void LeadRole::nextMove()
{
	if(lengthToEnemy<30)
	{// 靠近

	}
	if(lengthToEnemy<10)
	{// 按攻击频率攻击

	}
}

void LeadRole::getCloseToEnemy()
{

}

void LeadRole::turnToEnemy()
{
	if(pG_hitme==0) return;

	float * pos0=pTransform->pTOmatrix->mMatrixQueue.back();
	float * pos=pG_hitme->pTransform->pTOmatrix->mMatrixQueue.back();

	float x=pos[12]-pos0[12];
	float y=pos0[14]- pos[14];

	if(y==0.0) {y+=0.02;}//LOGI("y: %f",y);
	else
	if(y==-0.0) {y=0.01;}

	double a=x*InvSqrt(x*x + y*y+0.01f);
	//if(abs(a)==1.0f)
		//LOGI("a: %f",a);
	//a= abs(a)==1.0f?a+0.01f:a;

	a=asin(a);
	if(-y<0) a=M_PI-a;
	//return a*180/M_PI;

	float TouchTheta1=a*180/M_PI;
	//getTouchTheta(pos[12]-pos1[12],pos1[14]- pos[14]);

	float *matrix1=pTransform->pTOmatrix->mMatrixQueue.back();
	float ThetaBody=atan2(-matrix1[8], matrix1[10])*180/M_PI;


	//float pf[4];
	//pf[0]=2.0;//rotateTouchTheta2-ThetaView+ThetaBody
	//pf[0]=TouchTheta1+ThetaBody;
	//pf[1]=0;
	//pf[2]=1;
	//pf[3]=0;

	if(fabs(TouchTheta1+ThetaBody)<5)
		return;
	//LOGI("Theta: %f , %f, %f, %f", TouchTheta1,ThetaView,ThetaBody,pf[1]);
	//getMatrix().rotateM(pTransform->pTOmatrix->mMatrixQueue.back(),0,
	//		pf[0], pf[1],pf[2],pf[3]);
	//getMatrix().rotateM(pTransform->pTOmatrix->mMatrixQueue.back(),0,
	//		TouchTheta1+ThetaBody, 0, 1, 0);

	addRotateY(TouchTheta1+ThetaBody);
}

void LeadRole::interactionEnemy(Role* pAR)
{
	float * pos0=pTransform->pTOmatrix->mMatrixQueue.back();
	float * posAR=pAR->pTransform->pTOmatrix->mMatrixQueue.back();

	float length= (pos0[12]-posAR[12])*(pos0[12]-posAR[12])
				//+(pos0[13]-posAR[13])*(pos0[13]-posAR[13])
				+(pos0[14]-posAR[14])*(pos0[14]-posAR[14]);

	if(length<64)
	{

	}

}

void LeadRole::interactionSameKind(Role* pAR)
{

}

void LeadRole::CBTouchDown(float x, float z)
{
	//pEP->body->activate(true);

	if( pAFG->change_stay == 0 )// && moveCode!=ID_WALK1
	{
		if((x*x+z*z)<3000)
		{
			command=ID_WALK1;
			act(ID_WALK1);
		}else
		{
			command=ID_RUN;
			act(ID_RUN);
		}

		moveState= STATE_RUNNING;
	}

}

void LeadRole::CBTouchUp(float x, float z)
{
	//LOGI("CBTouchUp");
	//pEP->body->setLinearVelocity(btVector3(0,0,0));
	moveState= STATE_STOP;

}

void LeadRole::CBTouchMove(float x, float z)
{
	//LOGI("CBTouchMove 1");
	//pEP->body->activate(true);
	if(pAFG->change_stay == 0&&moveCode!=ID_RUN)
	{
		//LOGI("CBTouchMove 2");
		command=ID_RUN;
		act(ID_RUN);

		//moveState= STATE_RUNNING;
	}
	moveState= STATE_RUNNING;
}

