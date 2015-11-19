#include "LeadHP.h"
#include "Screen2d.h"

LeadHP::LeadHP()
{
	pColor[0]=1.0f;
	pColor[1]=0.0f;
	pColor[2]=0.0f;

	index[0]=0; index[1]=2; index[2]=1;
	index[3]=0; index[4]=3; index[5]=2;

}

void LeadHP::init()
{
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY
	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();
	getGL().chooseMatrix(GLSL::VMATRIX);
	getGL().setIdentity();

	float pf4[4];
	//getGL().glslUpdate();

	pScreen2d->getPos(10, 10, vertics);
	pScreen2d->getPos(getGL().viewport[2]*0.5f, 10, &(vertics[3]));
	//pMatrix->screenPto3dP(10, 10, vertics);
	//pMatrix->screenPto3dP(getGL().viewport[2]*0.5f, 10, &(vertics[3]));

	interval[0]=vertics[3]-vertics[0];
	interval[1]=vertics[4]-vertics[1];
	interval[2]=vertics[5]-vertics[2];

	pScreen2d->getPos(getGL().viewport[2]*0.5f, getGL().viewport[3]*0.05f, &(vertics[6]));
	//pMatrix->screenPto3dP(getGL().viewport[2]*0.5f, getGL().viewport[3]*0.05f, &(vertics[6]));
	//pMatrix->screenPto3dP(10, 30, &(vertics[9]));

	vertics[9] =vertics[6]-interval[0];
	vertics[10]=vertics[7]-interval[1];
	vertics[11]=vertics[8]-interval[2];
	//p1=Vector3(pf4[0],pf4[1],pf4[2]);




	//p2=Vector3(pf4[0],pf4[1],pf4[2]);//pf4[0],pf4[1],pf4[2]
	//LOGI("2:%f,%f,%f",vec2.x,vec2.y,vec2.z);

}

void LeadHP::setPLR(LeadRole* pLR_)
{
	pLR=pLR_;
}

void LeadHP::draw()
{
	glDisable (GL_CULL_FACE);
	getGL().setFuncType(GLSL::FUNS_LIGHT_EASY);//FUNS_LIGHT_PHONG FUNS_LIGHT_EASY

	getGL().chooseMatrix(GLSL::MMATRIX);
	getGL().setIdentity();

	getGL().material->setAmbient(pColor);

	getGL().material->setDiffuse(pColor);
	getGL().material->setSpecular(pColor);

	getGL().material->draw();

    getGL().setHasTex(0.0f);

    glBindTexture(GL_TEXTURE_2D, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);

	vertics[3]=vertics[0]+interval[0];
	vertics[4]=vertics[1]+interval[1];
	vertics[5]=vertics[2]+interval[2];

	vertics[6] =vertics[9]+interval[0];
	vertics[7]=vertics[10]+interval[1];
	vertics[8]=vertics[11]+interval[2];

	glLineWidth(2.125f);
	getGL().glslUpdate();
	//glDrawElements(GL_LINE_LOOP,6,GL_UNSIGNED_SHORT,index);
	glDrawArrays(GL_LINE_LOOP,0,4);

	if(pLR->HP>0)
		HPPercentage=pLR->HP/pLR->HPAll;
	else
		HPPercentage=0;

	vertics[3]=vertics[0]+interval[0]*HPPercentage;
	vertics[4]=vertics[1]+interval[1]*HPPercentage;
	vertics[5]=vertics[2]+interval[2]*HPPercentage;

	vertics[6] =vertics[9]+interval[0]*HPPercentage;
	vertics[7]=vertics[10]+interval[1]*HPPercentage;
	vertics[8]=vertics[11]+interval[2]*HPPercentage;
	//pMid=p1+(p2-p1)*pLR->HP/pLR->HPAll;

	//glLineWidth(20.125f);



	glBindBuffer(GL_ARRAY_BUFFER, 0);
	getGL().attribPointer(getGL().pGPNow->gvPositionHandle, 0,3, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvNormalHandle, 0, 0, vertics);
	//getGL().attribPointer(getGL().pGPNow->gvTexCoordHandle, 0, 0, vertics);

	//getGL().glslUpdate();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_SHORT,index);

	glEnable (GL_CULL_FACE);
	//line.draw(&p1, &pMid);
}
