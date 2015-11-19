#include "ArmyRole.h"
#include "Vector3.h"
#include <math.h>
#include "math/m_math.h"
#include "LeadRole.h"
#include "log.h"

int ArmyRole::objectSum=0;

ArmyRole::ArmyRole(char * fGraphFile):
funcDealMovement(0)
//,moveCode(-1)
//,pMoveNow(0)
,lengthToEnemy(1000)
,Role()
{
	setGlslFuncType(GLSL::FUNS_LIGHT_PHONG);
	pPhyC=new PhyCylinder();
	pPhyC->setGraph(this);

	vAR=vector<ArmyRole*>(0);

	objectID=objectSum++;
	//pG=new Graph();
	setFileName(fGraphFile);
	pMHP=new MHP();
	pMHP->setData();
	pMHP->setHPAll(10);
	//load();
	//vMovement=vector<Movement*>(0);
}

ArmyRole::~ArmyRole()
{
	//if(pG) free(pG);
	//pG=0;
}

void ArmyRole::reset()
{
	Role::reset();
}

void ArmyRole::makeTransform()
{
	if(gp->jointNump>0 && gp->animations){
		//getGL().chooseMatrix(GLSL::MMATRIX);
		//getGL().setIdentity();
		if (pAFG->change_stay == 0) {
			//pAFG->change_stay = 1;
			//Entity::draw();
			moveState=STATE_STOP;

			//getGL().glslUpdate();
		} else {

			//float x1=-166.0;
			//float y1=57.2;
			//float x2=13.5;
			//float y2=57.2;
			if(pAFG->frame!=pAFG->lastFrame)
			{
				//LOGI("nextPos : %f, %f, %f ", nextPos[0],nextPos[1],nextPos[2]);
				calVelocity();

				for(int i=0;i<vAR.size();i++)
				{
					if(vAR[i]->state!=2)
						pPhyC->interact(vAR[i]);
				}
				//LOGI("pos h: %f, %f ,%f", nowPos[1],nextPos[1],pPScene->h);
				//if(pPScene->hasTouch(nowPos, nextPos)) LOGI("hasTouch");
				//LOGI("isInside:%d",pPScene->isInside(nowPos) );
				//if(pPScene->isInside(nowPos))
				{
					floorHeight=pPScene->h-0.5;
					if(nextPos[1]<floorHeight)
						if(moveCode!=ID_FALLDOWN1)
						{
							//LOGI("nextPos[1] : %f", nextPos[1]);
							nextPos[1]=floorHeight;
						}

					pPScene->calculate(nowPos, nextPos,50.0f);
				}
				//else
				{
				//	floorHeight=-100;
					//nextPos[1]-=1;
				}

				//if(abs(nowPos[1]-floorHeight)>3)
				//	posState=POSSTATE_SKY;
				//else
				//	posState=POSSTATE_GROUND;



				//LOGI("pos : %f, %f, %f ", nextPos[0],nextPos[1],nextPos[2]);floorHeight;//

				pTransform->pTOmatrix->mMatrixQueue.back()[12]=nextPos[0];
				pTransform->pTOmatrix->mMatrixQueue.back()[13]=nextPos[1];//-bodyHeight
				pTransform->pTOmatrix->mMatrixQueue.back()[14]=nextPos[2];

			}
			//getGL().chooseMatrix(GLSL::MMATRIX);
			//getGL().setIdentity();

			//Entity::draw();

			//getGL().glslUpdate();
		}

		//Entity::draw();
		//getGL().glslUpdate();
	}
}

void ArmyRole::update()
{
	//getEnviromentInfo();
	//getGL().setFuncType(GLSL::FUNS_LIGHT_PHONG);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();

	//pAFG->getFrameNum();

	onUpdateStart();
	if(hasChange)
	{
		setRotateY(changeRotateY);

		hasChange=false;
	}
	draw();

    onUpdateEnd();

	//getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	//getGL().chooseMatrix(GLSL::MMATRIX);
	//getGL().setIdentity();
	pMHP->setPos(pTransform->pTOmatrix->mMatrixQueue.back()[12],
			pTransform->pTOmatrix->mMatrixQueue.back()[13]+19,
			pTransform->pTOmatrix->mMatrixQueue.back()[14]
			);
	pMHP->setHPAll(HPAll);
	pMHP->setHP( HP );
	pMHP->draw();

    //if(moveCode==2) dealMovementRun1();

}

void ArmyRole::onUpdateStart()
{
	dealMovements0(moveCode);
}

void ArmyRole::onUpdateEnd()
{
	dealMovements1(moveCode);
}

void ArmyRole::addMovement(int moveID,char *pfile,int infoLen, int *info)
{

	Movement *pM=new Movement(0,moveID,0,pfile,0);
	gp->addMovement(pM);
	pM->HP=2;
	for(int i=0; i<infoLen;i++)
	{
		pM->getAnimation()
						  ->pFrames[info[2*i]].setInfo(info[2*i], info[2*i+1]);
	}

}

void ArmyRole::setMovements()
{
	if (!gp) return;
	if(gp->vMovements.size()>0) return;
	//char aFile_run[]="run_test6_1.amn\0";//"walkman6.amn\0";

	char aFile1[]="enemy_run2_1.amn\0";//"jump_forward1_1.amn\0";//"jump_test5_1.amn\0";//"walkManNew1.amn\0";
	char aFile2[]="hit_hand_leg1.amn\0";//"hit_test4.amn\0";//"jump_hide2.amn\0";//"jump_test5_2.amn\0";
	char aFile3[]="behitted5_1.amn\0";
	char aFile4[]="hit_hand5_1.amn\0";
	char aFile5[]="falldownup1.amn\0";//"falldown1_1.amn\0";

	char aFile6[]="getup1_1.amn\0";

	//addMovement( ID_RUN, aFile_run);

	addMovement( ID_RUN2, aFile1,0, 0);

	int keyLen=2;
	int keyFrames[]={0,gp->vMovements.back()->getAnimation()->frameNum};

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}
	//KeyFrame *pKF=new KeyFrame();
	//pKF->setKeyFrameNum(gp->vMovements.back()->getAnimation()->frameNum);
	//gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);

	int infoLen=1;
	int info0[]={
			45,Frame::TYPE_HIT1,
	};

	addMovement( ID_HIT2, aFile2, infoLen, info0);

	addMovement( ID_BEHIT1, aFile3,0, 0);

	keyFrames[1]=gp->vMovements.back()->getAnimation()->frameNum;

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}

	infoLen=4;
	int info[]={
			9,Frame::TYPE_HIT1,
			24,Frame::TYPE_HIT1,
			38,Frame::TYPE_HIT1,
			53,Frame::TYPE_HIT1
	};

	addMovement( ID_HIT4, aFile4,infoLen, info);

	addMovement( ID_FALLDOWN1, aFile5, 0, 0);
	keyFrames[1]=gp->vMovements.back()->getAnimation()->frameNum;

	for(int i=0; i<keyLen;i++)
	{
		KeyFrame *pKF=new KeyFrame();
		pKF->setKeyFrameNum(keyFrames[i]);
		gp->vMovements.back()->getAnimation()->addKeyFrame(pKF);
	}

	delete[] gp->vp_;
	gp->vp_=0;

}

void ArmyRole::dealMovements0(int ID)
{
	//float pfs[4]={0,0,0,1};
	switch(ID)
	{
	case ID_RUN:
		dealMovementRun1();
		break;
	case ID_RUN2:
		if(pAFG->frame==60)
			pAFG->startFrameNum(10);//frame=10;
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	case ID_HIT2:
	case ID_HIT4:
		if(pAFG->frame==0)
		turnToEnemy();
		break;
	case ID_FALLDOWN1:
		if(pAFG->frame==1)
			turnToEnemy();
		break;
	case ID_BEHIT1:
		if(pAFG->frame==1)
			turnToEnemy();
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=pPScene->h;
		break;

	}
}

void ArmyRole::dealMovements1(int ID)
{
	float pfs[4]={0,0,0,1};
	switch(ID)
	{
	case ID_RUN:

		break;
	case ID_RUN2:
		//if(pAFG->frame==60)
		//	pAFG->frame=10;
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	case ID_HIT2:
	case ID_HIT4:
	    if(pMoveNow->getAnimation()->pFrames[pAFG->frame].getType()==Frame::TYPE_HIT1)
	    {
	    	//if(pMoveNow->getAnimation()->frame!=9) return;

	    	//gp->getFramePointer(pfs,pMoveNow->getAnimation(),
	    	//	    			pMoveNow->getAnimation()->frame, 694);
	    	//LOGI("HIT0: %f, %f, %f",pfs[0],pfs[1],pfs[2]);
	    	pfs[0]=0;pfs[1]=13.0f;pfs[2]=7.5f;pfs[3]=1.0f;
	    	//LOGI("hit: %d", pMoveNow->getAnimation()->frame);
	    	getMatrix().multiplyMV(pfs,0, pTransform->pTOmatrix->mMatrixQueue.back(), 0,  pfs, 0);
	    	//LOGI("HIT1: %f, %f, %f",pfs[0],pfs[1],pfs[2]);
	    	Vector3 pos1(pTransform->pTOmatrix->mMatrixQueue.back()[12],
	    			pTransform->pTOmatrix->mMatrixQueue.back()[13],
	    			pTransform->pTOmatrix->mMatrixQueue.back()[14]);

	    	Vector3 hitpos(pfs[0],pfs[1],pfs[2]);

	    	Vector3 Enemypos(aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[12],
	    			aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[13],
	    			aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[14]);
	    	//LOGI("Enemypos: %f, %f, %f",aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[12],
	    			//aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[13],
	    			//aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[14]);
	    	hitpos=(pos1+hitpos)/2;
	    	hitpos=hitpos-Enemypos;

	    	//Vector3 ab=Enemypos-pos1;
	    	//Vector3 ac=Enemypos-hitpos;
	    	//ab=ab.crossProduct(ac);
	    	float l=hitpos.length();//ab.length()/ac.length();
	    	//LOGI("Len: %f\n /n",l );
	    	//LOGI("aimRole: %f, %f, %f",\
	    			aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[12],\
	    			aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[13],\
	    			aimRole->pTransform->pTOmatrix->mMatrixQueue.back()[14]);
	    	//LOGI("l : %f",l);
	    	if(l<10)
	    	{
	    		((LeadRole*)aimRole)->behitted(10,Frame::TYPE_HIT1);
	    		((LeadRole*)aimRole)->pG_hitme=this;
	    	}

	    }
	    	//
		break;
	case ID_FALLDOWN1:
		if(pAFG->frame==40 )
		{
			if(HP<=0)
			{
				state=2;
				//removeFromRace();
				//act(ID_GETUP1);
				//pAFG->frame=5;
				//pAFG->change_stay = 1;
			}
		}else
		if(pAFG->frame==gp->vMovements.back()->getAnimation()->frameNum )
		{
			pTransform->pTOmatrix->mMatrixQueue.back()[13]=floorHeight;
		}
		break;
	}
}

void ArmyRole::dealMovementRun1()
{
	if(moveState==STATE_STOP)
		pAFG->startFrameNum(35);//frame=35;

	switch(pAFG->frame)
	{
	case 31:
		pAFG->end();//frame=0;
		pAFG->change_stay = 0;

	    //pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	case 35:
		pAFG->end();//frame=0;
		pAFG->change_stay = 0;
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	case 9:
		if(moveState==STATE_STOP)
			pAFG->startFrameNum(30);//frame=30;
		break;
	case 19:
		if(moveState==STATE_STOP)
			pAFG->startFrameNum(33);//frame=33;
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	case 29:
		if(moveState==STATE_RUNNING)
			pAFG->startFrameNum(10);//frame=10;
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=0;
		break;
	}
}

void ArmyRole::dealMKick1()
{
	if(pAFG->frame==24)
	{
		pAFG->startFrameNum(54);//frame=54;
	}
}

void ArmyRole::dealMovementJump1()
{
	if (pAFG->frame ==31)
	{
		pAFG->frame=0;
		pAFG->change_stay = 0;
		//changeMatrix();
	}
	else if (pAFG->frame ==35)
	{
		pAFG->frame=0;
		pAFG->change_stay = 0;
		//changeMatrix();
	}
	else if (pAFG->frame ==9)
	{
		//changeMatrix();
		if(moveState==STATE_STOP)
			pAFG->frame=30;
		//else if(moveState==STATE_RUNNING)

	}
	else if (pAFG->frame ==19)
	{
		//changeMatrix();
		if(moveState==STATE_STOP)
			pAFG->frame=33;
	}
	else if (pAFG->frame ==29)
	{
		//changeMatrix();
		if(moveState==STATE_RUNNING)
			pAFG->frame=10;

	}
}

//set the hit data
void ArmyRole::hit()
{

}

//set be hitted data
void ArmyRole::behitted(int type, float hp)
{
	HP-=hp;

	turnToEnemy();

	if(HP<=0)
	{
		act(ID_FALLDOWN1);
		pAFG->change_stay = 1;
		//HP=10;
	}
	else
	{
		if(type==Frame::TYPE_HIT1)
		act(ID_BEHIT1);
		else if(type==Frame::TYPE_HIT2)
			act(ID_FALLDOWN1);
		pTransform->pTOmatrix->mMatrixQueue.back()[13]=floorHeight;
		timeBetweenDelay=0;
		pAFG->change_stay = 1;
	}

}

void ArmyRole::getEnviromentInfo()
{

	float * pos0=pTransform->pTOmatrix->mMatrixQueue.back();

	float * pos=aimRole->pTransform->pTOmatrix->mMatrixQueue.back();
	lengthToEnemy= (pos0[12]-pos[12])*(pos0[12]-pos[12])
			//+(pos0[13]-pos[13])*(pos0[13]-pos[13])
			+(pos0[14]-pos[14])*(pos0[14]-pos[14]);

}

void ArmyRole::nextMove()
{
	if(lengthToEnemy<30)
	{// 靠近

	}
	if(lengthToEnemy<10)
	{// 按攻击频率攻击

	}
}

void ArmyRole::getCloseToEnemy()
{

}

void ArmyRole::turnToEnemy()
{
	float * pos0=pTransform->pTOmatrix->mMatrixQueue.back();
	float * pos=aimRole->pTransform->pTOmatrix->mMatrixQueue.back();
	//
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

	if(abs(TouchTheta1+ThetaBody)<5)
		return;
	//LOGI("Theta: %f , %f, %f, %f", TouchTheta1,ThetaView,ThetaBody,pf[1]);
	//getMatrix().rotateM(pTransform->pTOmatrix->mMatrixQueue.back(),0,
	//		pf[0], pf[1],pf[2],pf[3]);
	//getMatrix().rotateM(pTransform->pTOmatrix->mMatrixQueue.back(),0,
	//		TouchTheta1+ThetaBody, 0, 1, 0);

	addRotateY(TouchTheta1+ThetaBody);
}

void ArmyRole::interactionEnemy(Role* pAR)
{

}

void ArmyRole::interactionSameKind(Role* pAR)
{

}
