#include "RenderManager.h"
#include "log.h"
#include "commond.h"
#include "Screen2d.h"
#include "SysTime.h"

RenderManager *pRenderManager=0;

RenderManager::RenderManager()
{
	//vpGraph=vector<Graph*>(100);
	//vpGraph.clear();
	aveLight=0;
	lumAdapt=0;
	pvpREGRender=new vector<RenderEntityGroup *>(10);
	pvpREGRender->clear();

	pvpREGNext=new vector<RenderEntityGroup *>(10);
	pvpREGNext->clear();

}

void RenderManager::addGraph(Graph* pGraph)
{
	//vpGraph.push_back(pGraph);
}

void RenderManager::clear()
{
	//vpGraph.clear();
	for(int i=0;i<pvpREGRender->size();i++)
	{
		pvpREGRender->at(i)->clear();
	}
	return;
}

void RenderManager::addRenderEntity(Graph *pGraph)
{
	pvpREGNext->at(pGraph->gp->id_REG)->addREntity(pGraph);
/*
	for(int i=0;i<pvpREGNext->size();i++)
	{
		if(pvpREGNext->at(i)->gp==pGraph->gp)
			pvpREGNext->at(i)->addREntity(pGraph);
	}
*/
	//pREG->addREntity(pRE);
}

void RenderManager::addREG(GraphData* gp)
{
	RenderEntityGroup *pREG=new RenderEntityGroup();
	pREG->setGraphData(gp);
	gp->id_REG=pvpREGRender->size();
	pvpREGRender->push_back(pREG);

	RenderEntityGroup *pREG1=new RenderEntityGroup();
	pREG1->setGraphData(gp);
	pvpREGNext->push_back(pREG1);
}

void RenderManager::changeFrame()
{
	vector<RenderEntityGroup *> *pvpREGTemp=pvpREGRender;
	pvpREGRender=pvpREGNext;
	pvpREGNext=pvpREGTemp;
	//for(int i=0;i<pvpREGNext->size();i++)
	//{
	//	pvpREGNext->at(i)->clear();
	//}
}

int num_1=0;

void RenderManager::getFboHDR(int w, int h, int internalformat,int texFormat, MFbo& fbo){
	//glGetIntegerv(GL_VIEWPORT, getGL().viewport);
	//LOGI("viewport:%d,%d,%d,%d",getGL().viewport[0],getGL().viewport[1],getGL().viewport[2],getGL().viewport[3]);

	// Depth texture. Slower than a depth buffer, but you can sample it later in your shader
	fbo.width=w;
	fbo.height=h;

	fbo.fboTexture[0] = 0;
	glGenTextures(1, fbo.fboTexture);
	glBindTexture(GL_TEXTURE_2D, fbo.fboTexture[0]);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);//GL_REPEAT
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);//GL_CLAMP_TO_EDGE

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR );

	glTexImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, GL_RGBA, texFormat ,
			0);
	//glGenerateMipmap(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, 0);

	getGL().shadowTexId = fbo.fboTexture[0];

	getGL().active_shadowTexId = getGL().getATextureID();

	LOGI("renderToTex 1");
	//create renderbuffer
	fbo.RenderBfName[0] = 0;
	glGenRenderbuffers(1, fbo.RenderBfName);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo.RenderBfName[0]);
	LOGI("renderToTex 2");//GL_RGB565
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, w, h);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	LOGI("renderToTex 3");
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	fbo.fboHDR[0] = 0;
	glGenFramebuffers(1, fbo.fboHDR);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo.fboHDR[0]);

	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, fbo.fboTexture[0], 0);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo.RenderBfName[0]);
	//glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	LOGI("renderToTex 4");

	// Always check that our framebuffer is ok
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOGI("renderToTex false");
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		return ;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	LOGI("renderToTex 5");
	return ;
}

//void RenderManager::initFBO(){

//}

void RenderManager::initFboHDR(int w, int h){

	getFboHDR(w, h, GL_RGBA, GL_UNSIGNED_BYTE,fbo1);
	getFboHDR(w/4, h/4, GL_RGBA, GL_UNSIGNED_BYTE,fbo1d4);
	getFboHDR(4,4, GL_RGBA, GL_UNSIGNED_BYTE,fbo32);

	/*
	//getFboHDR(32,32, GL_RGBA4, GL_UNSIGNED_BYTE,fbo32);
	fbo32.RenderBfName[0] = 0;
	glGenRenderbuffers(1, fbo32.RenderBfName);
	glBindRenderbuffer(GL_RENDERBUFFER, fbo32.RenderBfName[0]);
	LOGI("renderToTex 2");//GL_RGB565
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, 32, 32);
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	LOGI("renderToTex 3");
	// The framebuffer, which regroups 0, 1, or more textures, and 0 or 1 depth buffer.
	fbo32.fboHDR[0] = 0;
	glGenFramebuffers(1, fbo32.fboHDR);
	glBindFramebuffer(GL_FRAMEBUFFER, fbo32.fboHDR[0]);

	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, fbo32.RenderBfName[0]);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glDrawBuffer(GL_NONE); // No color buffer is drawn to.
	LOGI("renderToTex 4");
	*/
}

void RenderManager::render()
{
	//vpREGRender=vpREG1;
/*
	for(int i=0;i<vpGraph.size();i++)
	{
		vpGraph[i]->render();
	}
*/
	//LOGI("r0");

	//render to fbo
	getGL().setPGPNow(getGL().pGLSL_BASE);
	MFbo *pfbo=&fbo1;

	glBindFramebuffer(GL_FRAMEBUFFER, pfbo->fboHDR[0]);
	glViewport(0, 0, pfbo->width, pfbo->height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	getGL().shadowTexId = pfbo->fboTexture[0];
	//pLR->gp->pmtls[0]->mTextureId=fbo1d4.fboTexture[0];
	for(int i=0;i<pvpREGRender->size();i++)
	{
		pvpREGRender->at(i)->render();
	}

	glBindTexture(GL_TEXTURE_2D, pfbo->fboTexture[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	//render to fbo32
	getGL().setPGPNow(getGL().pGLSL_log);
	pGFullSquare->gp->pmtls[0]->mTextureId=fbo1.fboTexture[0];
	pfbo=&fbo32;
	glBindFramebuffer(GL_FRAMEBUFFER, pfbo->fboHDR[0]);
	glViewport(0, 0, pfbo->width, pfbo->height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	pGFullSquare->drawNoMovement();
	glBindTexture(GL_TEXTURE_2D, pfbo->fboTexture[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	//LOGI("r1");
	//doLogAverage();
	//LOGI("r2");
	//return;
	//glCopyTexImage2D (GLenum target, GLint level, GLenum internalformat, GLint x, GLint y, GLsizei width, GLsizei height, GLint border);

	//render to fb01d4
	getGL().setPGPNow(getGL().pGLSL_getHLight);
	pGFullSquare->gp->pmtls[0]->mTextureId=fbo1.fboTexture[0];
	pfbo=&fbo1d4;
	glBindFramebuffer(GL_FRAMEBUFFER, pfbo->fboHDR[0]);
	glViewport(0, 0, pfbo->width, pfbo->height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//lumAdapt += (aveLight - lumAdapt) * ( 1 - pow( 0.4f, 30 * pSysTime->frameInterval ) );
	lumAdapt=150.0f;
	glUniform1i( getGL().pGLSL_getHLight->imgWHandle,  pfbo->width );
	glUniform1i( getGL().pGLSL_getHLight->imgHHandle,  pfbo->height );
	glUniform1f( getGL().pGLSL_getHLight->aveHandle,  lumAdapt/255.0f );


    glActiveTexture( GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,fbo32.fboTexture[0]);
    glUniform1i(getGL().pGLSL_getHLight->tex2Handle, 1);

	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	pGFullSquare->drawNoMovement();
	glBindTexture(GL_TEXTURE_2D, pfbo->fboTexture[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	// make gaosi effect
	doGaosi(pfbo);

	//render to screen
	getGL().setPGPNow(getGL().pGLSL_BLOOM);//getGL().pGLSL_BASE

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glViewport(0, 0, getGL().viewport[2],getGL().viewport[3]);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	  //glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(GLSL::FUNS_LIGHT_EASY) );
	pGFullSquare->gp->pmtls[0]->mTextureId=fbo1.fboTexture[0];

	//pvpREGNext->at(pGFullSquare->gp->id_REG)->render();
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[14]=-1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[0]=1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[5]=1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[10]=0.9f;

    glActiveTexture( GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D,pfbo->fboTexture[0]);
    glUniform1i(getGL().pGLSL_BLOOM->tex2Handle, 1);

	pGFullSquare->drawNoMovement();


	//LOGI("r3");
/*
	for(int i=0;i<pvpREGRender->size();i++)
	{
		pvpREGRender->at(i)->render();
	}

	//LOGI("drawCall 0: %d",RenderEntityGroup::drawCallTime);
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();
	//pGFullSquare->useTexture=true;
	//pGFullSquare->drawNoMovement();
	pGFullSquare->draw();
	for(int i=0;i<pvpREGRender->size();i++)
	{
		pvpREGRender->at(i)->render();
	}

	GraphData* gp=pGFullSquare->gp;


	pMatrix->multiplyMM(pGFullSquare->pf32, 0, getGL().PMatrix, 0, getGL().VMatrix, 0);


    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, 1, false, getGL().VMatrix );

    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, 1, false, pGFullSquare->pf32 );

	//getGL().material->draw();
    getGL().material->drawNoMaterial();
	if (gp->pmtls[0]->mTextureId>0)
	{
		getGL().setHasTex(1.0f);

		glActiveTexture( GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, gp->pmtls[0]->mTextureId);

		glUniform1i(getGL().pGPNow->tex1Handle, 0);
	}
	else
	{
		getGL().setHasTex(0.0f);
	}


	glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(GLSL::FUNS_LIGHT_EASY) );


	//getGL().attribPointer(getGL().pGPNow->gvPositionHandle, gp->batch_ubo[0], 3, 9*sizeof(float), 0 );//gp->ubo[0]
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, gp->batch_ubo[0], 3, 9*sizeof(float), (GLvoid *) (12));
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, gp->batch_ubo[0], 3, 9*sizeof(float), (GLvoid *) (24));

	glBindBuffer(GL_ARRAY_BUFFER, gp->batch_ubo[0]);

	glVertexAttribPointer(getGL().pGPNow->gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), 0);

	glVertexAttribPointer(getGL().pGPNow->gvNormalHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (12));

	glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (24));

	glEnableVertexAttribArray(getGL().pGPNow->gvPositionHandle);
	glEnableVertexAttribArray(getGL().pGPNow->gvNormalHandle);
	glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->batch_uboi[0]);


	//glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, draw_count, GL_FALSE, &(NormalMatrixs[offset*9]) );

	glDrawElements(GL_TRIANGLES, gp->pmtls[0]->viL  ,GL_UNSIGNED_SHORT, 0);
*/


	//LOGI("drawCall 1: %d",RenderEntityGroup::drawCallTime);
	RenderEntityGroup::drawCallTime=0;
	//clear();
	//pREG->render();
}

void RenderManager::renderObjects(){
	getGL().material->drawNoMaterial();

	for(int i=0;i<pvpREGRender->size();i++)
	{
		pvpREGRender->at(i)->render();
	}

	num_1++;
	if(num_1<80)
	{
		pGame->pLine->drawMults();
	}else if(num_1>100)
	{
		num_1=0;
		//pGame->pLine->drawMults();
	}


	// draw shadow
	if(pGame->isRunning){
	pGame->pShadow->pTransform->pTOmatrix->mMatrixQueue.back()[12]=
			pGame->pLR->pTransform->pTOmatrix->mMatrixQueue.back()[12];//0;//pGame->pLR->floorHeight+0.8f
	pGame->pShadow->pTransform->pTOmatrix->mMatrixQueue.back()[13]=
			pGame->pLR->sceneBelowID==-1?pGame->pLR->floorHeight+1.0f : pGame->vpScene[pGame->pLR->sceneBelowID]->pPScene->h+0.3f;//camera1->m_View.y
	pGame->pShadow->pTransform->pTOmatrix->mMatrixQueue.back()[14]=
			pGame->pLR->pTransform->pTOmatrix->mMatrixQueue.back()[14];//0;//


	GraphData* gp=pGame->pShadow->gp;

	getGL().material->setAmbient(gp->pmtls[0]->Ka);

	getGL().material->setDiffuse(gp->pmtls[0]->Kd);
	getGL().material->setSpecular(gp->pmtls[0]->Ks);
	//LOGI("ks: %f,%f,%f",Mp->Ks[0],Mp->Ks[1],Mp->Ks[2]);
	getGL().material->setShininess(gp->pmtls[0]->illum);

	getGL().material->draw();

	if (gp->pmtls[0]->mTextureId>0)
	{
		getGL().setHasTex(1.0f);

		glActiveTexture( GL_TEXTURE0);

		glBindTexture(GL_TEXTURE_2D, gp->pmtls[0]->mTextureId);

		glUniform1i(getGL().pGPNow->tex1Handle, 0);
	}
	else
	{
		getGL().setHasTex(0.0f);
	}


	glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(GLSL::FUNS_LIGHT_EASY) );


	//getGL().attribPointer(getGL().pGPNow->gvPositionHandle, gp->batch_ubo[0], 3, 9*sizeof(float), 0 );//gp->ubo[0]
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, gp->batch_ubo[0], 3, 9*sizeof(float), (GLvoid *) (12));
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, gp->batch_ubo[0], 3, 9*sizeof(float), (GLvoid *) (24));

	glBindBuffer(GL_ARRAY_BUFFER, gp->batch_ubo[0]);

	glVertexAttribPointer(getGL().pGPNow->gvPositionHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), 0);

	glVertexAttribPointer(getGL().pGPNow->gvNormalHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (12));

	glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (24));

	glEnableVertexAttribArray(getGL().pGPNow->gvPositionHandle);
	glEnableVertexAttribArray(getGL().pGPNow->gvNormalHandle);
	glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->batch_uboi[0]);


	pGame->pShadow->updateTransform();

	//glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, draw_count, GL_FALSE, &(NormalMatrixs[offset*9]) );

	glEnable(GL_BLEND);
	glBlendFunc(GL_ZERO, GL_ONE_MINUS_SRC_COLOR);

	glDrawElements(GL_TRIANGLES, gp->pmtls[0]->viL  ,GL_UNSIGNED_SHORT, 0);

	glDisable(GL_BLEND);

}
}

void RenderManager::doLogAverage(){
	int num1=32*32*4;
	unsigned char *pixels=new unsigned char[num1];
	glReadPixels(0, 0, 32, 32, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

	aveLight=0;
	for(int i=0;i<num1;i+=4)
		aveLight+=pixels[i];
	aveLight=aveLight/(float)(32*32);
	//LOGI("%d,%d,%d,%d",pixels[i*4],pixels[(i+1)*4], pixels[(i+2)*4],pixels[(i+3)*4]);

	//LOGI("ave1:%f",aveLight);
}

void RenderManager::doGaosi(MFbo *pfbo){

	//render to fb01d4
	getGL().setPGPNow(getGL().pGLSL_gaosi_x);
	pGFullSquare->gp->pmtls[0]->mTextureId=pfbo->fboTexture[0];
	pfbo=&fbo1d4;
	glBindFramebuffer(GL_FRAMEBUFFER, pfbo->fboHDR[0]);
	glViewport(0, 0, pfbo->width, pfbo->height);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1i( getGL().pGLSL_gaosi_x->imgWHandle,  pfbo->width );


	//pvpREGNext->at(pGFullSquare->gp->id_REG)->render();

	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[14]=-1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[0]=1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[5]=1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[10]=0.9f;
	pGFullSquare->drawNoMovement();
	glBindTexture(GL_TEXTURE_2D, pfbo->fboTexture[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

	//render to fb01d4
	getGL().setPGPNow(getGL().pGLSL_gaosi_y);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glUniform1i( getGL().pGLSL_gaosi_y->imgHHandle,  pfbo->height );

	//pvpREGNext->at(pGFullSquare->gp->id_REG)->render();

	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[14]=-1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[0]=1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[5]=1.0f;
	//pGFullSquare->pTransform->pTOmatrix->mMatrixQueue.back()[10]=0.9f;
	pGFullSquare->drawNoMovement();
	glBindTexture(GL_TEXTURE_2D, pfbo->fboTexture[0]);
	glGenerateMipmap(GL_TEXTURE_2D);

}

void RenderManager::initFullScreenSquare()
{
	//pREFullScreenSquare=new RenderEntity();
	pGFullSquare=new Graph();
	char fileName[]="square.ly";
	pGFullSquare->setFileName(fileName);
	pGFullSquare->load();

	float *pvs=pGFullSquare->gp->vp;

	pScreen2d->getPos(0.0f, getGL().viewport[3], pvs );
	pScreen2d->getPos(getGL().viewport[2], getGL().viewport[3],&(pvs[9*1]) );
	pScreen2d->getPos(0.0f, 0.0f,  &(pvs[9*2]) );
	pScreen2d->getPos(getGL().viewport[2], 0.0f, &(pvs[9*3]) );

	LOGI("pvs:%f,%f,%f",pvs[0],pvs[1],pvs[2]);
	LOGI("pvs:%f,%f,%f",pvs[9],pvs[10],pvs[11]);

	pGFullSquare->gp->setBatchs(1);
	addREG(pGFullSquare->gp);

/*
	float *pvs=new float[4*9];
	pScreen2d->getPos(0.0f, getGL().viewport[3], pvs );
	pScreen2d->getPos(getGL().viewport[2], getGL().viewport[3], &(pvs[9*1]) );
	pScreen2d->getPos(getGL().viewport[2], 0.0f, &(pvs[9*2]) );
	pScreen2d->getPos(0.0f, 0.0f, &(pvs[9*3]) );

	pvs[0]=-100.0f;pvs[1]=50.0f;pvs[2]=50.0f;
	pvs[9*1+0]=100.0f;pvs[9*1+1]=50.0f;pvs[9*1+2]=-50.0f;
	pvs[9*2+0]=100.0f;pvs[9*2+1]=-60.0f;pvs[9*2+2]=50.0f;
	pvs[9*3+0]=-100.0f;pvs[9*3+1]=-60.0f;pvs[9*3+2]=-50.0f;

	pvs[3]=0.0f;pvs[4]=0.0f;pvs[5]=1.0f;
	pvs[9*1+3]=0.0f;pvs[9*1+4]=0.0f;pvs[9*1+5]=1.0f;
	pvs[9*2+3]=0.0f;pvs[9*2+4]=0.0f;pvs[9*2+5]=1.0f;
	pvs[9*3+3]=0.0f;pvs[9*3+4]=0.0f;pvs[9*3+5]=1.0f;

	pvs[6]=0.0f;pvs[7]=0.0f;pvs[8]=1.0f;
	pvs[9*1+6]=1.0f;pvs[9*1+7]=0.0f;pvs[9*1+8]=1.0f;
	pvs[9*2+6]=1.0f;pvs[9*2+7]=1.0f;pvs[9*2+8]=1.0f;
	pvs[9*3+6]=0.0f;pvs[9*3+7]=1.0f;pvs[9*3+8]=1.0f;

	unsigned short *vip=new unsigned short[6];
	vip[0]=0;vip[1]=1;vip[2]=2;
	vip[3]=2;vip[4]=1;vip[5]=3;
*/

	//pGFullSquare->gp=new GraphData();
	//pGFullSquare->gp->init(pvs,&(pvs[3]),&(pvs[6]), vip, 6, 0, 1);


}
