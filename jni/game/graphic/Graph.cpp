#include "Graph.h"
//#include "getMtl.h"
#include "log.h"
//#define GL_GLEXT_PROTOTYPES
#include "fileDeal/loadFile.h"
#include <EGL/egl.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
//#include "GLES/gl.h"
#include "MOpenil.h"
#include <math.h>
#include "GraphRace.h"
#include "RenderManager.h"
//#include "RenderEntityGroup.h"

//PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES;
//PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES;
//PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES;
///PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES;

//PFNGLGENVERTEXARRAYSOESPROC glGenVertexArraysOES = (PFNGLGENVERTEXARRAYSOESPROC)eglGetProcAddress ( "glGenVertexArraysOES" );
//PFNGLBINDVERTEXARRAYOESPROC glBindVertexArrayOES = (PFNGLBINDVERTEXARRAYOESPROC)eglGetProcAddress ( "glBindVertexArrayOES" );
//PFNGLDELETEVERTEXARRAYSOESPROC glDeleteVertexArraysOES = (PFNGLDELETEVERTEXARRAYSOESPROC)eglGetProcAddress ( "glDeleteVertexArraysOES" );
//PFNGLISVERTEXARRAYOESPROC glIsVertexArrayOES = (PFNGLISVERTEXARRAYOESPROC)eglGetProcAddress ( "glIsVertexArrayOES" );


#define BUFFER_OFFSET(offset) ((GLvoid *) NULL + offset)

//GLuint Graph::uboLast=0;
//GLuint Graph::uboiLast=0;
//GLSL *Graph::glsl=0;
//vector<float*> Entity::aniEntityp;
//int Graph::TextureId=0;
bool Graph::useTexture=true;

//int Graph::frameNum=0;
//float *Graph::floats1 =new float[4];// (float*) malloc(4*sizeof(float));
//FourNumber Graph::fourNumber1 = FourNumber(0, 0, 0);

//float *Graph::pf32 = new float[32];// (float*) malloc( 32*sizeof(float) );

Graph::Graph():
changeFunc(0),isChoosed(false),gp(0),pParserLy(new ParserLy())
,state(0),moveState(STATE_STOP),lockMovement(false),changeArgu(0)
,floorHeight(0)
,pT(0)
,pMShape(0)
//,pEP(NULL)
,roleType(0)
,moveCode(-1)
,pMoveNow(0)
,hasChange(false)
,changeRotateY(0)
,pFrameNow(0)
,bodyHeight(0)
,moveType( FIXED)
,followType(FOLLOWTYPE_YES)
,Entity(){
	glslFunctype=0;
	centre=Vector3();
	floats1[0]=floats1[1]=floats1[2]=0.0f;
	floats1[3]=1.0f;
	NTextureId=0;
	vpAFGs=vector<AnimForGraph *>(0);
	//pREntity=new RenderEntity();

	pMovepath=0;

	//vMovement=vector<Movement*>(0);
	//GLuint arrays[2];
	//glGenVertexArraysOES(2, arrays);
	//LOGI("arrays: %d, %d",arrays[0],arrays[1]);
	//glDeleteVertexArraysOES(2,arrays);
}

Graph::Graph(char *fileName1)
{
	Graph();
	fileName=fileName1;
}

void Graph::loadModel(GraphData *gp,char * fileNamei)
{
	// LOGI("Graph1");
	//jiliye3_1.ly//"my_scene1_7.ly"
	char folder[]="";
	char * filgpath=fileName;
	char *str=new char[50];
	pParserLy->setFileName(filgpath);
	pParserLy->setPGraphData(gp);
	pParserLy->parse();
}

void Graph::updatePhysic()
{
	//if(pEP) pEP->update();
}

void Graph::setShape()
{
	if(pMShape) return;
	pMShape=new MShape();
	pMShape->setGraph(this);
}

void Graph::setRoleType(int type_)
{
	roleType=type_;
}

int Graph::getRoleType()
{
	return roleType;
}

void Graph::load(){
	//LOGI("Graph::load:%s",fileName);
	gp=GraphData::getInstance(fileName);//(GraphData *)malloc(sizeof(GraphData)*sizeof(float));
	//gp->setFileName(fileName);
	//gp->load();
	//pParserLy->setFileName(fileName);
	//pParserLy->setPGraphData(gp);
	//pParserLy->parse();

	//loadModel(gp , fileName);
	//m_log1("jointNum: %d",gp->jointNump);


}

void Graph::loadAnimation(GraphData *ep,char * fileName){
        LOGI("loadAnimation 0");
        if(fileName){
        char folder[]="./sdcard/animationFile/";
        char * filePath=getLoadFile().combineChars(folder,fileName);
        LOGI("aniFile:%s",filePath);
        char * chars=getLoadFile().getCharsofFile(filePath);
        gp->animations=new Animation();//(Animation*)malloc(sizeof(*(gp->animations)));
        gp->animations->frameNum=*(int*)chars;
        //LOGI("frameNum %d\n", *(ep->animations->frameNum));
        chars+=sizeof(float);
        gp->animations->frameTime=(float*)chars;
        //LOGI("frameTime %f\n", *(ep->animations->frameTime));
        chars+=sizeof(float)*(gp->animations->frameNum);
        gp->animations->jointNum=*(int*)chars;
        //LOGI("jointNum %d\n", *(ep->animations->jointNum));
        chars+=sizeof(float);
        gp->animations->joints=new Joint[gp->animations->jointNum];
        //(Joint*)malloc(sizeof(Joint)*(*(gp->animations->jointNum)));

        for(int i=0;i<ep->animations->jointNum;i++){
                gp->animations->joints[i].sn=i;
                gp->animations->joints[i].posf[0]=(*((float*)chars));
                chars+=sizeof(float);
                gp->animations->joints[i].posf[1]=(*((float*)chars));
                chars+=sizeof(float);
                gp->animations->joints[i].posf[2]=(*((float*)chars));
                chars+=sizeof(float);
        //  	LOGI("posf[0] %f\n", ep->animations->joints[i].posf[0]);
        //      LOGI("posf[1] %f\n", ep->animations->joints[i].posf[1]);
        //      LOGI("posf[2] %f\n", ep->animations->joints[i].posf[2]);
                gp->animations->joints[i].Matrix=(float*)chars;
                chars+=16*sizeof(float)*(gp->animations->frameNum);
        }
        //ep->animations->jointMatrixs=(float*)chars;
        gp->animations->frame=0;//0
        gp->animations->direction=1;
        pAFG->change_stay=1;
        }
        LOGI("loadAnimation done");
}

void Graph::reset()
{
	state=0;
	if(pMovepath)
	{
		//LOGI("vMPs:%d",pMovepath->vMPs.size());
		if(pMovepath->vMPs.size()>0)
		pMovepath->vMPs.clear();
		delete pMovepath;
		pMovepath=0;
	}
	getMatrix().setIdentityM(matrixOrg,0);
	//getMatrix().arrayCopy(matrixOrg,0,
	//		pTransform->pTOmatrix->mMatrixQueue.back(), 0,
	//					16);
}

void Graph::action(float theta){
	/*
	if(pAFG->frame==0)	return;

    fourNumber1 = FourNumber(0, 0, 0);
    fourNumber1.productMatrix(floats1,(&(pMoveNow->pA->joints[0].Matrix[16* (pAFG->frame)])));
    fourNumber1 = FourNumber(
  		  floats1[0]- (pMoveNow->pA->joints[0].posf[0]),
  		  floats1[1]- (pMoveNow->pA->joints[0].posf[1]),
  		  floats1[2]- (pMoveNow->pA->joints[0].posf[2]));
    */
}

void Graph::setFuncType(int funcType_)
{
  getGL().setFuncType(funcType_);
}

void Graph::addMove(int code_,char *pfile,int type)
{
	Movement *pM=new Movement(0,code_,0,pfile,type);
	gp->addMovement(pM);
	gp->vMovements.back()->HP=2;
}

void Graph::act(int code_)
{
	//setAction(vMovement[0]->getAnimation());
	moveCode=code_;
	//return;

	for(int i=0; i<gp->vMovements.size(); i++)
	{
		if(code_==gp->vMovements[i]->code)
		{
			//if(pG&&gp&&pAFG)
				//pAFG->frame=0;
			//setAction(gp->vMovements[i]);
			//LOGI("code: %d",code_);
			pMoveNow=gp->vMovements[i];
			gp->animations=pMoveNow->pA;
			pAFG=vpAFGs[i];
			pAFG->change_stay = 1;
			pAFG->startFrameNum(0);
			//pAFG->frame=0;

			//pFrameNow=gp->animations->pFrames[pAFG->frame];
			//moveState=STATE_STOP;
		}

	}
}

void Graph::setAction(Movement* pMoveNow_)
{
	pMoveNow=pMoveNow_;

	//pAFG=pMoveNow->pAFG;
	// moving = true;
}

void Graph::setMaterial(Mtl *Mp)
{
  getGL().material->setAmbient(Mp->Ka);

  getGL().material->setDiffuse(Mp->Kd);
  getGL().material->setSpecular(Mp->Ks);
  //LOGI("ks: %f,%f,%f",Mp->Ks[0],Mp->Ks[1],Mp->Ks[2]);
  getGL().material->setShininess(Mp->illum);
  //LOGI("Ns: %f",Mp->Ns);
}

void Graph::setChange(float *pf)
{
	if(changeArgu) free(changeArgu);
	if(pAFG->frame==0)
	{
		pAFG->change_stay = 1;
		pAFG->startFrameNum(1);
		//pAFG->frame=1;
	}
	changeArgu=pf;
	changeMatrix();
}

void Graph::addRotateY(float theta)
{
	hasChange=true;
	changeRotateY=theta;
}

void Graph::setRotateY(float theta)
{
	//if(pAFG->frame==0)
	//{
		//pAFG->change_stay = 1;
		//pAFG->frame=1;
	//}

	//float * pos=pTransform->pTOmatrix->mMatrixQueue.back();
	//LOGI("0");
	getMatrix().setRotateM(pf32, 0, (int)theta, 0, 1, 0);

	//pos=pf32;
	//LOGI("rotate: %d", (int)theta);
	getMatrix().multiplyMM(pTransform->pTOmatrix->mMatrixQueue.back(), 0, pTransform->pTOmatrix->mMatrixQueue.back(), 0, pf32, 0);
	//LOGI("2");
	//getMatrix().arrayCopy(pf32, 16, pTransform->pTOmatrix->mMatrixQueue.back(), 0, 16);

	//pos=pTransform->pTOmatrix->mMatrixQueue.back();
//LOGI("3");
}

void Graph::changeMatrix()
{
	if(!changeArgu) return;
	if(pAFG->frame==0)
	{
		pAFG->change_stay = 1;
		//pAFG->frame=1;
		pAFG->startFrameNum(1);
	}

	if(changeArgu[0]==1.0)//translate
		getMatrix().translateM(pTransform->pTOmatrix->mMatrixQueue.back(),0, changeArgu[1],changeArgu[2],changeArgu[3]);
	else
	if(changeArgu[0]==2.0)//rotate
		getMatrix().rotateM(pTransform->pTOmatrix->mMatrixQueue.back(),0, changeArgu[1], changeArgu[2],changeArgu[3],changeArgu[4]);

	free(changeArgu);
	changeArgu=0;
}

float * Graph::getActionVp()
{
	{
		//get the translate matrix for each frame
		float *floats11 =&(pMoveNow->pA->joints[0].Matrix_1[16* ( pAFG->frame )]);// new float[16];//(float*) malloc(16 * sizeof(float));
		//getMatrix().arrayCopy(&(go->animations->joints[0].Matrix_1[16* (pAFG->frame)]), 0,floats11,0,  16);

		//translate the translate matrix of entity
		getMatrix().translateM(pTransform->pTOmatrix->mMatrixQueue.back(),0,floats11[4],floats11[5],floats11[6]);
		//LOGI("sizeof(float)56: %f, %f , %f",floats11[sizeof(float)],floats11[5],floats11[6]);
		//bool isInSky=(floats11[1] - pMoveNow->pA->joints[0].posf[1])>5;

		//getMatrix().translateM(pTransform->pTOmatrix->mMatrixQueue.back(),0,0,floorHeight,0);
		//pTransform->pTOmatrix->mMatrixQueue.back()[13]=floats11[1]+floorHeight;//floorHeight;//+
		//LOGI("h1 : %f",pTransform->pTOmatrix->mMatrixQueue.back()[13]);
		getGL().chooseMatrix(GLSL::MMATRIX);
		getGL().setIdentity();
		//getGL().translate(0,floorHeight,0);
		Entity::draw();
		getGL().glslUpdate();
		//pAFG->frame += 1;

	}

	return 0;
}

bool Graph::isPointerScreen()
{
	floats1[0]=0;
	floats1[1]=9;
	floats1[2]=0;
	floats1[3]=1;
	getMatrix().multiplyMM(pf32, 0, getGL().VMatrix, 0, pTransform->pTOmatrix->mMatrixQueue.back(), 0);//getGL().MMatrix
	getMatrix().multiplyMM(pf32, 16, getGL().PMatrix, 0, pf32, 0 );
	getMatrix().multiplyMV(pf32,0,pf32,16, floats1, 0);
	if(fabs(pf32[0]/pf32[3])>1 || fabs(pf32[1]/pf32[3])>1 || pf32[3]<0 )
		return false;
	else
		return true;
}

void Graph::drawMovement()
{
	updateTransform();
	if (pAFG->change_stay == 0) {

		if(!isPointerScreen())
		{
			//LOGI("dis0:%s",fileStr.c_str());
			return;
		}
//if(uboLast!=gp->ubo[0])
		getGL().attribPointer(getGL().pGPNow->gvPositionHandle, gp->ubo[0],3, 9*sizeof(float), 0);

		//pREntity->setData_v(getGL().pGPNow->gvPositionHandle, gp->ubo[0],3, 8*sizeof(float), 0);

	} else {

		getGL().attribPointer(getGL().pGPNow->gvPositionHandle, pMoveNow->pA->ubo,3, 0, (GLvoid *) ( pAFG->frame *gp->pointerNum*3*sizeof(float) ) );//gp->animations->ubos[pAFG->frame]

		//pREntity->setData_v(getGL().pGPNow->gvPositionHandle, pMoveNow->pA->ubo,3, 0, (GLvoid *) ( pAFG->frame *gp->pointerNum*3*sizeof(float) ) );
		//pAFG->frame += 1;
		if ( pAFG->frame > ( pMoveNow->pA->frameNum-1 )&&(pAFG->frame!=pAFG->lastFrame) )//|| pAFG->frame < 0
		{
			//frameNum++;
			pAFG->end();
			//pAFG->frame=0;
			//pMoveNow->pA->direction *= -1;
			pAFG->change_stay = 0;
			moveState=STATE_STOP;
		}

		if(!isPointerScreen())
		{
			return;
		}

		//LOGI("h1: %f,h2: %f",(pMoveNow->pA->joints[0].Matrix[16* (pAFG->frame)+7]),
		//		pTransform->pTOmatrix->mMatrixQueue.back()[13] );
	}
	//if(uboLast!=gp->ubo[0])
	getGL().attribPointer(getGL().pGPNow->gvNormalHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (12));
	//if(uboLast!=gp->ubo[0])
	getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (24));

	//pREntity->setData_vn(getGL().pGPNow->gvNormalHandle, gp->ubo[0],3, 8 * sizeof(float), (GLvoid *) (12));

	//pREntity->setData_vt(getGL().pGPNow->gvTexCoordHandle, gp->ubo[0],2, 8 * sizeof(float), (GLvoid *) (24));

	//setMaterial(gp->pmtls[0]);
	//getGL().material->draw();

    if (useTexture && gp->pmtls[0]->mTextureId>0)
      {
        getGL().setHasTex(1.0f);

        glActiveTexture(  GL_TEXTURE0);//gp->pmtls[0]->active_texture_id +

        glBindTexture(GL_TEXTURE_2D,gp->pmtls[0]->mTextureId);
        //LOGE("gp->pmtls[i]->mTextureId %d\n", gp->pmtls[i]->mTextureId);
        //int loc = glGetUniformLocation(getGL().getProgramHandle(), "Tex1");
        //if( loc >= 0 )gp->pmtls[0]->active_texture_id
        glUniform1i(getGL().pGPNow->tex1Handle, 0);
      }
    else
      {
        getGL().setHasTex(0.0f);
      }

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->pmtls[0]->uboi[0]);
	glDrawElements(GL_TRIANGLES, gp->pmtls[0]->viL, GL_UNSIGNED_SHORT, 0);
	//LOGI("d2");
}

void Graph::drawNoMovement(){

	updateTransform();

   getGL().attribPointer(getGL().pGPNow->gvPositionHandle, gp->batch_ubo[0],3, 9*sizeof(float), 0 );//gp->ubo[0]
   getGL().attribPointer(getGL().pGPNow->gvNormalHandle, gp->batch_ubo[0],3, 9*sizeof(float), (GLvoid *) (12));
   getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, gp->batch_ubo[0],3, 9*sizeof(float), (GLvoid *) (24));
   //int i=0;
   //for(int i;i<(gp->numofMtl);i++){
       if(gp->pmtls[0]->viL>0){
           //setMaterial(gp->pmtls[0]);
           //getGL().material->draw();

            if (useTexture && gp->pmtls[0]->mTextureId>0)
              {
                getGL().setHasTex(1.0f);

                glActiveTexture( GL_TEXTURE0);
//gp->pmtls[i]->active_texture_id +
                glBindTexture(GL_TEXTURE_2D,gp->pmtls[0]->mTextureId);
                //LOGE("gp->pmtls[i]->mTextureId %d\n", gp->pmtls[i]->mTextureId);
                //int loc = glGetUniformLocation(getGL().getProgramHandle(), "Tex1");
                //if( loc >= 0 )gp->pmtls[i]->active_texture_id
                glUniform1i(getGL().pGPNow->tex1Handle, 0);
              }
            else
              {
                getGL().setHasTex(0.0f);
              }
           // if(TextureId) glBindTexture(GL_TEXTURE_2D,TextureId);
             // LOGI("gp->pmtls[i]->vip %d\n", gp->pmtls[i]->vip[gp->pmtls[i]->viL-1]);
             // glDrawElements(GL_TRIANGLES,gp->pmtls[i]->viL,GL_UNSIGNED_SHORT,gp->pmtls[i]->vip);
            //if(uboiLast!=gp->pmtls[i]->uboi[0])
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->batch_uboi[0]);
            //uboiLast=gp->pmtls[0]->uboi[0];
            glDrawElements(GL_TRIANGLES,gp->pmtls[0]->viL,GL_UNSIGNED_SHORT,0);

        	//pREntity->uboi=gp->pmtls[0]->uboi[0];
        	//pREntity->viL=gp->pmtls[0]->viL;

           }
   //}
}

void Graph::makeTransform()
{

	if(gp->jointNump>0 && gp->animations){

		//getGL().chooseMatrix(GLSL::MMATRIX);
		//getGL().setIdentity();
		//LOGI("makeTransform ");
		if (pAFG->change_stay == 0) {
			//pAFG->change_stay = 1;
			//getGL().chooseMatrix(GLSL::MMATRIX);
			//getGL().setIdentity();

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
				//LOGI("nextPos 0: %f, %f, %f ", nextPos[0],nextPos[1],nextPos[2]);
				calVelocity();
				if(pPScene->isInside(nowPos,5.0f))
				{
					floorHeight=pPScene->h;
					pPScene->calculate(nowPos, nextPos,5.0f);
				}
				else
				{
					floorHeight=-100;
					//nextPos[1]-=1;
				}

				//if(abs(nowPos[1]-floorHeight)>3)
				//	posState=POSSTATE_SKY;
				//else
				//	posState=POSSTATE_GROUND;



				//LOGI("nextPos : %f, %f, %f ", nextPos[0],nextPos[1],nextPos[2]);

				pTransform->pTOmatrix->mMatrixQueue.back()[12]=nextPos[0];
				pTransform->pTOmatrix->mMatrixQueue.back()[13]=nextPos[1];//-bodyHeight
				pTransform->pTOmatrix->mMatrixQueue.back()[14]=nextPos[2];

			}

		}

	}

}

void Graph::setPhyScene(PhyScene *pPScene_)
{
	pPScene=pPScene_;
}

void Graph::updateAfterTransform()
{

}

void Graph::updateAfterDraw()
{

}

void Graph::draw(){

	//LOGI("draw0");
	//setFuncType(getGL().FUNS_NORMAL);


	//LOGI("draw1");
	makeTransform();

	updateAfterTransform();

	//pREntity->gp=gp;

	//pREntity->Mmatrix=pTransform->pTOmatrix->mMatrixQueue.back();
	//LOGI("draw2");
	if(gp->jointNump>0 && gp->animations){
		//LOGI("draw3");
		if (pAFG->change_stay == 0) {

			if(!isPointerScreen())
			{
				//LOGI("dis0:%s",fileStr.c_str());
				return;
			}

			//getGL().attribPointer(getGL().pGPNow->gvPositionHandle, gp->ubo[0],3, 9*sizeof(float), 0);

		} else {

			//getGL().attribPointer(getGL().pGPNow->gvPositionHandle, pMoveNow->pA->ubo,3, 0, (GLvoid *) ( pAFG->frame *gp->pointerNum*3*sizeof(float) ) );//gp->animations->ubos[pAFG->frame]

			if ( pAFG->frame > ( pMoveNow->pA->frameNum-1 )&&(pAFG->frame!=pAFG->lastFrame) )//|| pAFG->frame < 0
			{

				pAFG->end();
				pAFG->change_stay = 0;
				moveState=STATE_STOP;
			}

			if(!isPointerScreen())
			{
				return;
			}

		}

	}
	//LOGI("draw4");
	//((RenderEntityGroup*)pREG)->addREntity(this);
	pRenderManager->addRenderEntity(this);

	return;
	//Entity::draw();

	//getGL().glslUpdate();

	//LOGI("a1");
	//pRenderManager->addGraph(this);
/*
	if(gp->jointNump>0 && gp->animations){
		pRenderManager->addGraph(this);
	}
	else
	{
		   pREntity->pv=gp->vp;
		   pREntity->num_vertex=gp->pointerNum;

		   pREntity->num_vi=gp->pmtls[0]->viL;
		   pREntity->pvi=gp->pmtls[0]->vip;

		   pREntity->Mmatrix=pTransform->pTOmatrix->mMatrixQueue.back();

		   pRenderManager->addRenderEntity(pREntity);
	}
*/

	//LOGI("a2");




	//glBindTexture(GL_TEXTURE_2D,0);
}

void Graph::setGlslFuncType(int functype_){
	glslFunctype=functype_;
}

void Graph::render()
{
	glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(glslFunctype) );
	if(gp->jointNump>0 && gp->animations){

		drawMovement();

	}
	else
	{
		drawNoMovement();
	}
	updateAfterDraw();
}

void Graph::updateTransform()
{
	//multiplyMVMatrix();

	getMatrix().multiplyMM(pf32, 16,getGL().VMatrix, 0,
			pTransform->pTOmatrix->mMatrixQueue.back(), 0);

	//multiplyMPMatrix();

	getMatrix().multiplyMM(pf32, 0, getGL().PMatrix, 0, pf32, 16);


    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, 1, false, &(pf32[16]) );

    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, 1, false, pf32 );

  //
/*
	for(int i=0;i<3;i++)
	{
		pf32[i*3]=pf32[16+i*4];
		pf32[i*3+1]=pf32[16+i*4+1];
		pf32[i*3+2]=pf32[16+i*4+2];
	}

	getMatrix().invertM3(pf32);

	//getMatrix().setIdentityM(NormalMatrix,0);

	getMatrix().transposeM3(pf32);

	glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, 1, false, pf32);
	*/
}

//float *Graph::pfTemp4=new float[4];

void Graph::calVelocity()
{


	float *floats11 =&(pMoveNow->pA->joints[0].Matrix_1[16* ( pAFG->frame )]);// new float[16];//(float*) malloc(16 * sizeof(float));

	nextPos[0]=floats11[4];// used as temp
	nextPos[1]=floats11[5];
	nextPos[2]=floats11[6];

	//if(false)
	for(int i=1;i<(pAFG->frame - pAFG->lastFrame);i++)
	{
		floats11 =&(pMoveNow->pA->joints[0].Matrix_1[16* ( pAFG->lastFrame+i )]);
		nextPos[0]+=floats11[4];
		nextPos[1]+=floats11[5];
		nextPos[2]+=floats11[6];
	}
	//LOGI("2");
	//LOGI("floats11 : %f, %f, %f ", floats11[4],floats11[5],floats11[6]);
	//LOGI("***new****");

	//bodyHeight=pMoveNow->pA->joints[0].Matrix[7]
	//           -pMoveNow->pA->joints[0].Matrix_1[16* (pAFG->frame)+7];

	//bodyHeight=bodyHeight>0?floorHeight:0;
	floats11 =pTransform->pTOmatrix->mMatrixQueue.back();

	//getMatrix().translateM( pf32,0, pTransform->pTOmatrix->mMatrixQueue.back(),0,nextPos[0],nextPos[1],nextPos[2]);
	// translate x y z

	nowPos[0] = floats11[0] * nextPos[0] + floats11[4] * nextPos[1] + floats11[8] * nextPos[2] +floats11[12];
	nowPos[1] = floats11[1] * nextPos[0] + floats11[5] * nextPos[1] + floats11[9] * nextPos[2] +floats11[13];
	nowPos[2] = floats11[2] * nextPos[0] + floats11[6] * nextPos[1] + floats11[10] * nextPos[2] +floats11[14];

	nextPos[0]=nowPos[0];
	nextPos[1]=nowPos[1];//+bodyHeight
	nextPos[2]=nowPos[2];

	nowPos[0]=floats11[12];
	nowPos[1]=floats11[13];//+bodyHeight
	nowPos[2]=floats11[14];

	//vel[0]=nextPos[0]-nowPos[0];
	//vel[1]=nextPos[1]-nowPos[1];
	//vel[2]=nextPos[2]-nowPos[2];

}

GLuint Graph::setNTexture(char *NTextureFile)
{
  //GLuint texName11;
  //  LOGI("setTextureB:%d", 155);
  char folder[]="./sdcard/gamepics/";//"./sdcard/textureFiles/";
  char *filgpath=getLoadFile().combineChars(folder,NTextureFile);
  getMOL().loadImage(filgpath);
  delete[] filgpath;
  //char * chars;
  //chars=getCharsofFile(filgpath);
  //texName11=setTexture(&chars);
  NTextureId=setTexture(getMOL().pixels,getMOL().w,getMOL().h,getMOL().format);

  return NTextureId;
}

GLuint Graph::setTexture(BYTE *pixels,int w, int h,unsigned int format)
{
        GLuint texName;
        glGenTextures(1, &texName);
        glBindTexture(GL_TEXTURE_2D, texName);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        // glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

        //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        //glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );
        //glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, format, w, h, 0, format, GL_UNSIGNED_BYTE,
              pixels);
        //glGenerateMipmap(GL_TEXTURE_2D);
        //glGenerateMipmap (GL_TEXTURE_2D);
        return texName;
}

void Graph::setVpAFG()
{
	if(vpAFGs.size()!=0) return;//means one object only need set this once
	vpAFGs=vector<AnimForGraph *>(gp->vMovements.size());
	for(int i=0;i<vpAFGs.size();i++)
	{
		vpAFGs[i]=new AnimForGraph();
		vpAFGs[i]->setMovement(gp->vMovements[i]);
	}
}

void Graph::setGraphRace(Object * pGraphRace_)
{
	pGraphRace=pGraphRace_;
}

void Graph::removeFromRace()
{
	//LOGI("removeFromRace");
	((GraphRace*)pGraphRace)->remove(this);
}

void Graph::setMoveType(int type)
{
	moveType=type;
}

void Graph::addMovepoint(Movepoint *pMovepoint)
{
	if(pMovepath==0)
		pMovepath=new Movepath();
	pMovepoint->setGraph(this);
	pMovepath->addMovepoint(pMovepoint);

}

void Graph::initMovepath()
{
	pMovepath->initMovepath();
}

Graph::~Graph(){

}
