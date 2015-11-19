#include "RenderEntityGroup.h"
#include <GLES2/gl2.h>
#include "GLSL/GLSL.h"
#include "GLSL/matrix.h"
#include <string>
#include "log.h"

int RenderEntityGroup::drawCallTime=0;

RenderEntityGroup::RenderEntityGroup()
:num(0)
{
	num_v=0;
	num_vi=0;
	num_re=0.0f;
	vpREntity=vector<RenderEntity*>(10);
	vpREntity.clear();
	RenderEntity* pRE1=new RenderEntity();
	vpREntity.push_back(pRE1);
	//pvdata=new float[50000*9]; // 50000 vertex

	//pvi=new unsigned short[10000*3];//10000 triangles
}

void RenderEntityGroup::setGraphData(GraphData *gp_)
{
	gp=gp_;
}

void RenderEntityGroup::initREs(int num)
{

}

void RenderEntityGroup::addREntity(Graph *pGraph)
{
	//LOGI("adre 0: %d",num);
	if(gp->jointNump>0)
	if(vpREntity.size()<=num)
	{
		RenderEntity* pRE1=new RenderEntity();
		vpREntity.push_back(pRE1);
		pRE1->gp=pGraph->gp;
	}

	int intTemp=num*16;

	getMatrix().multiplyMM(&MVMatrixs[intTemp], 0,getGL().VMatrix, 0,
			pGraph->pTransform->pTOmatrix->mMatrixQueue.back(), 0);

	getMatrix().multiplyMM(&MVPMatrixs[intTemp], 0, getGL().PMatrix, 0, &MVMatrixs[intTemp], 0);
//LOGI("adre 1");
/*
	if(gp->pmtls[0]->Ns>0)//FUNS_LIGHT_PHONG
	{

		//LOGI("adre 2");
		for(int i=0;i<3;i++)
		{
			NormalMatrixs[num*9 + i*3]  =MVMatrixs[intTemp+i*4];
			NormalMatrixs[num*9 + i*3+1]=MVMatrixs[intTemp+i*4+1];
			NormalMatrixs[num*9 + i*3+2]=MVMatrixs[intTemp+i*4+2];
		}

		getMatrix().invertM3(&NormalMatrixs[num*9]);

		//getMatrix().setIdentityM(NormalMatrix,0);

		getMatrix().transposeM3(&NormalMatrixs[num*9]);
	}
*/

	//LOGI("adre 3");
	if(gp->jointNump==0 || gp->animations==0)
	{
		//vpREntity[num]->change_stay=0;
	}
	else
	{
		if (pGraph->pAFG->change_stay == 0) {
			vpREntity[num]->change_stay=1;
			vpREntity[num]->ubov= pGraph->pMoveNow->getAnimation()->ubo;
			vpREntity[num]->sizev=3;
			vpREntity[num]->stridev=0;
			vpREntity[num]->pointerv=(GLvoid *) ( pGraph->pAFG->frame *pGraph->gp->pointerNum*3*sizeof(float) );

		} else {
			vpREntity[num]->change_stay=1;
			vpREntity[num]->ubov= pGraph->pMoveNow->getAnimation()->ubo;
			vpREntity[num]->sizev=3;
			vpREntity[num]->stridev=0;
			vpREntity[num]->pointerv=(GLvoid *) ( pGraph->pAFG->frame *pGraph->gp->pointerNum*3*sizeof(float) );

		}
		//vpREntity[num]->change_stay=1;
		//vpREntity[num]->ubov= pGraph->pMoveNow->getAnimation()->ubo;
		//vpREntity[num]->sizev=3;
		//vpREntity[num]->stridev=0;
		//vpREntity[num]->pointerv=(GLvoid *) ( pGraph->pAFG->frame *pGraph->gp->pointerNum*3*sizeof(float) );
	}


	num++;
	//if(num>49)　num=49;
	//LOGI("adre 4");
}

void RenderEntityGroup::updateTransform(RenderEntity *pRE)
{
    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, 1, GL_FALSE, pRE->MVMatrix);

    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, 1, GL_FALSE, pRE->MVPMatrix);

	//glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, 1, GL_FALSE, pRE->NormalMatrix);
}

void RenderEntityGroup::organize(){ }

void RenderEntityGroup::clear()
{
	num=0;
	//num_v=0;
	//num_vi=0;
	//num_re=0.0f;
	//vpREntity.clear();
}

void RenderEntityGroup::renderObjects(){
	if(num==0) return;

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

	if(gp->jointNump==0 || gp->animations==0)
	{
		if(gp->pmtls[0]->Ns>0)
			glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(GLSL::FUNS_LIGHT_PHONG) );
		else
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

		int offset=0;
		int draw_count=0;
		for(int toDraw=num; toDraw>0; toDraw -= MAX_OBJS_BATCH )
		{
			draw_count = toDraw < MAX_OBJS_BATCH ? toDraw : MAX_OBJS_BATCH;

		    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, draw_count, GL_FALSE, &(MVMatrixs[offset*16]) );

		    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, draw_count, GL_FALSE, &(MVPMatrixs[offset*16]) );

			//glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, draw_count, GL_FALSE, &(NormalMatrixs[offset*9]) );


			glDrawElements(GL_TRIANGLES, gp->pmtls[0]->viL * draw_count ,GL_UNSIGNED_SHORT, 0);

			offset += draw_count;

			drawCallTime++;
		}

	}
	else
	{
/*
		glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(GLSL::FUNS_LIGHT_PHONG) );

		getGL().attribPointer(getGL().pGPNow->gvPositionHandle, vpREntity[0]->ubov, vpREntity[0]->sizev, vpREntity[0]->stridev, vpREntity[0]->pointerv );

		//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (12));

		//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (24));

		glBindBuffer(GL_ARRAY_BUFFER, gp->ubo[0]);

		glVertexAttribPointer(getGL().pGPNow->gvNormalHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (12));

		glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (24));

		glEnableVertexAttribArray(getGL().pGPNow->gvNormalHandle);
		glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);

	    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, 1, GL_FALSE, MVMatrixs );

	    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, 1, GL_FALSE, MVPMatrixs );

		glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, 1, GL_FALSE, NormalMatrixs );


		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->pmtls[0]->uboi[0]);
		glDrawElements(GL_TRIANGLES, gp->pmtls[0]->viL, GL_UNSIGNED_SHORT, 0);

		drawCallTime++;
		*/


		 		glUniform1f( getGL().pGPNow->funcTypeHandle,  (float)(GLSL::FUNS_LIGHT_PHONG) );

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, gp->batch_uboi[0]);

				glBindBuffer(GL_ARRAY_BUFFER, gp->batch_ubo[0]);

				glVertexAttribPointer(getGL().pGPNow->gvNormalHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (12));

				glVertexAttribPointer(getGL().pGPNow->gvTexCoordHandle, 3, GL_FLOAT, GL_FALSE, 9*sizeof(float), (GLvoid *) (24));

				glEnableVertexAttribArray(getGL().pGPNow->gvNormalHandle);
				glEnableVertexAttribArray(getGL().pGPNow->gvTexCoordHandle);


				getGL().attribPointer(getGL().pGPNow->gvPositionHandle, vpREntity[0]->ubov,
						vpREntity[0]->sizev, vpREntity[0]->stridev, vpREntity[0]->pointerv );

				//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (12));

				//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (24));

			    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, 1, GL_FALSE, MVMatrixs );

			    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, 1, GL_FALSE, MVPMatrixs );

				//glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, 1, GL_FALSE, &(NormalMatrixs[i*9]) );
			    drawCallTime++;
				glDrawElements(GL_TRIANGLES, gp->pmtls[0]->viL, GL_UNSIGNED_SHORT, 0);
				//LOGI("draw ani");
/*
				for(int i=0;i<num;i++)
				{

					getGL().attribPointer(getGL().pGPNow->gvPositionHandle, vpREntity[i]->ubov,
							vpREntity[i]->sizev, vpREntity[i]->stridev, vpREntity[i]->pointerv );

					//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (12));

					//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, gp->ubo[0],3, 9 * sizeof(float), (GLvoid *) (24));

				    glUniformMatrix4fv( getGL().pGPNow->MVMatrixHandle, 1, GL_FALSE, &(MVMatrixs[i*16]) );

				    glUniformMatrix4fv( getGL().pGPNow->MVPMatrixHandle, 1, GL_FALSE, &(MVPMatrixs[i*16]) );

					//glUniformMatrix3fv(getGL().pGPNow->NormalMatrixHandle, 1, GL_FALSE, &(NormalMatrixs[i*9]) );

					glDrawElements(GL_TRIANGLES, gp->pmtls[0]->viL, GL_UNSIGNED_SHORT, 0);
				}
*/
				//drawCallTime++;


	}
}

void RenderEntityGroup::render()
{
	renderObjects();
	clear();
}
