#include "GraphData.h"
#include "fileDeal/loadFile.h"
#include "log.h"
#include "fourNumber.h"
#include "parser/ParserLy.h"

vector<GraphData*> GraphData::vGDAll=vector<GraphData*>(0);

GraphData::GraphData():strFile(""),animations(0),id_REG(0),dataType(DATATYPE_0){
	//vAnims=vector<Animation *>(0);
	//glGenBuffers(1, ubo_ani);
	vMovements=vector<Movement*>(0);
	//if(vGDAll==null)
	//	vGDAll=vector<GraphData*>(0);
}

GraphData::~GraphData() {
	if(fileData) delete fileData;
	/*
	if(vp) delete vp;
	if(vnp) delete vnp;
	if(vtp) delete vtp;
	if(vp_) delete vp_;
	if(weightsp) delete weightsp;
	*/
}

GraphData* GraphData::getInstance(char *fileName1){
	GraphData *pGd=GraphData::isLoad(fileName1);
	if(pGd!=NULL)
		return pGd;
	LOGI("create new GraphData");
	pGd=new GraphData();//(GraphData *)malloc(sizeof(GraphData)*sizeof(float));
	pGd->setFileName(fileName1);
	pGd->load();
	vGDAll.push_back(pGd);
	return pGd;
}

void GraphData::setFileName(char *fileName1)
{
	fileName=fileName1;
	strFile=string(fileName);
}

void GraphData::load() {
	if(strFile.length()>2)
	{

	}

	ParserLy *pParserLy=new ParserLy();
	pParserLy->setFileName(fileName);
	pParserLy->setPGraphData(this);
	pParserLy->parse();
	if(jointNump>0){
		//loadAnimation(gp ,&(fileNamea[0]));
		//
		//由于使用计算好的新的动作文件，所以不用vp_了
		vp_=new float[150* pointerNum*3 ];//*sizeof(float), 最大150帧
	}
}

GraphData* GraphData::isLoad(char *fileName){
	string fileName1(fileName);
	for(int i=0;i<vGDAll.size();i++){
		if(vGDAll[i]->strFile==fileName1)
			return vGDAll[i];
	}
	return NULL;
}

/**
 *
 *
 * @param vp_
 * @param vnp_
 * @param vtp_
 * @param vip_
 * @param viLength
 * @param textureID
 * @param batchNum
 */

void GraphData::init(float *vp_,float *vnp_,float *vtp_, unsigned short *vip_, int viLength, GLuint textureID, int batchNum){
	vp=vp_;
	vnp=vnp_;
	vtp=vtp_;

	pmtls = (Mtl **) malloc(1 * sizeof(Mtl*));
	pmtls[0] = new Mtl();
	pmtls[0]->vip=vip_;
	pmtls[0]->viL=viLength;
	pmtls[0]->mTextureId=textureID;
	setBatchs(batchNum);
}


void GraphData::draw() {

}

void GraphData::setAttribPointer() {
	if (useDataModel == GraphData::USE_ONE_UBO) {//婵炶揪缍�▔鏇㈠极閵堝棛鈻旈柨鐔剁矙閸ゅ绻涢敐澶嬫暠閻㈩垱鎹囬幆鍐礆椤＄豹fer闁诲海鏁搁、濠囨寘閿燂拷			{
		getGL().attribPointer(getGL().gvPositionHandle, ubo[0],3, 8 * 4, 0);
		getGL().attribPointer(getGL().gvNormalHandle, ubo[0],3, 8 * 4,
				(GLvoid *) (12));
		getGL().attribPointer(getGL().gvTexCoordHandle, ubo[0],2, 8 * 4,
				(GLvoid *) (24));
	} else if (useDataModel == GraphData::USE_UBOS) {//婵炶揪缍�▔鏇㈠极閵堝棗绶炴慨姗嗗亰閸ゅ鏌涘Δ浣圭閻庢凹鍙冮幆鍐礆椤＄豹fer闁诲海鏁搁、濠囨寘閿燂拷			{
		//getGL().attribPointer(getGL().gvPositionHandle, ubos[0], 0, 0);
		//getGL().attribPointer(getGL().gvNormalHandle, ubos[1], 0, 0);
		//getGL().attribPointer(getGL().gvTexCoordHandle, ubos[2], 0, 0);
	} else if (useDataModel == GraphData::USE_ARRAY) //婵炶揪缍�▔鏇㈠极閵堝鏋侀柣妤�墕缁侊箓鏌℃担鐟邦棗缂侇喖绉归弫宥囦沪閼测晝鎲归梺鐑╂噰閺嗗澆ffer
			{
		getGL().attribPointer(getGL().gvPositionHandle, 0,3, 0, vp);
		getGL().attribPointer(getGL().gvNormalHandle, 0,3, 0, vnp);
		getGL().attribPointer(getGL().gvTexCoordHandle, 0,2, 0, vtp);
	}
}

void GraphData::addMovement(Movement* pM)
{
	if(pM->pA) return;

	pM->pA=loadAnimation(pM->fileName);
	//setAniBuffers2(pM);
	setAniBuffers(pM->pA);
	vMovements.push_back(pM);
	//writeCache(pM);
}

Animation *GraphData::loadAnimation(char * fileName)
{

	//char folder[]="./sdcard/animationFile/";
	//char * filePath=combineChars(aniPath,fileName);
	//LOGI("aniFile:%s",filePath);
	char * chars=getLoadFile().getDataOfAniFile(fileName);//getCharsofFile(filePath);
	//LOGI("loadAnimation:%s",fileName);
	Animation *pA=new Animation();//(Animation*)malloc(sizeof(*pA));
	//pA->id=code;
	pA->frameNum=*(int*)chars;
	//LOGI("frameNum %d\n", pA->frameNum);
	chars+=4;
	pA->frameTime=new float[pA->frameNum];
	for(int i=0;i<pA->frameNum;i++)
	{
		pA->frameTime[i]=*(float*)chars;
		chars+=4;
	}

	//LOGI("frameTime %f\n", *(ep->pA->frameTime));
	//chars+=4*(pA->frameNum);
	pA->jointNum=*(int*)chars;
	//LOGI("jointNum %d\n", *(ep->pA->jointNum));
	chars+=4;
	pA->joints=new Joint[pA->jointNum];

	pA->joints[0].sn=0;
	pA->joints[0].posf[0]=(*((float*)chars));
	chars+=4;
	pA->joints[0].posf[1]=(*((float*)chars));
	chars+=4;
	pA->joints[0].posf[2]=(*((float*)chars));
	chars+=4;

	pA->joints[0].Matrix=new float[16*(pA->frameNum)];
	for(int i=0;i<16*(pA->frameNum);i++)
	{
		pA->joints[0].Matrix[i]=*(float*)chars;
		chars+=4;
	}
	//chars+=16*4*(pA->frameNum);

	pA->joints[0].Matrix_1=new float[16*(pA->frameNum)];
	for(int i=0;i<16*(pA->frameNum);i++)
	{
		pA->joints[0].Matrix_1[i]=*(float*)chars;
		chars+=4;
	}
	//pA->joints[0].Matrix_1=(float*)chars;
	//chars+=16*4*(pA->frameNum);

	for(int i=1;i<(pA->jointNum);i++){
		pA->joints[i].sn=i;
		pA->joints[i].posf[0]=(*((float*)chars));
		chars+=4;
		pA->joints[i].posf[1]=(*((float*)chars));
		chars+=4;
		pA->joints[i].posf[2]=(*((float*)chars));
		chars+=4;
		//pA->joints[i].Matrix=(float*)chars;
		//chars+=16*4*(pA->frameNum);
		pA->joints[i].Matrix=new float[16*(pA->frameNum)];
		for(int j=0;j<16*(pA->frameNum);j++)
		{
			pA->joints[i].Matrix[j]=*(float*)chars;
			chars+=4;
		}
	}
	//ep->pA->jointMatrixs=(float*)chars;
	pA->frame=0;
	pA->direction=1;
	pA->change_stay=0;
	//delete[] chars;
	//LOGI("loadAnimation done");
	return pA;
}

bool GraphData::writeCache(Movement* pM)
{
  FILE * fp;
    if((fp=fopen(getLoadFile().combineChars("/sdcard/",pM->fileName),"w+"))==NULL) {
            return false;
    }

    fwrite(vp_, sizeof(float), (pM->pA->frameNum)*(pointerNum)*3, fp);

    fflush(fp);

    fclose(fp);
    return true;
}

void GraphData::setAniBuffers(Animation *pA_)
{
	pA_->pFrames=new Frame[pA_->frameNum];

	//LOGI( "frameNum: %d", pA_->frameNum );

	float floats[3];
	int n=0;
	for(int i=0;i<pA_->frameNum; i++)
	{
		pA_->pFrames[i].setInfo(i,Frame::TYPE_NORMAL );

		//float *floats = (float*) malloc(3 * sizeof(float));
		FourNumber fourNumber = FourNumber(0, 0, 0);
		float w = 0;
		float wAll=0;
		for (int k = 0; k < pointerNum ; k++) {
			n=k+i*pointerNum;
			vp_[n * 3] = 0;
			vp_[n * 3 + 1] = 0;
			vp_[n * 3 + 2] = 0;
			w = 0;
			//wAll=0;
			for (int j = 0; j < (pA_->jointNum); j++) {
				w = weightsp[(pA_->jointNum) * k + j];

				if ( w != 0.0f) {
						//LOGI("%d: %f",k,w);
					//wAll+=w;
					fourNumber = FourNumber(
							(vp + k * 9)[0]- (pA_->joints[j].posf[0]),
							(vp + k * 9)[1]- (pA_->joints[j].posf[1]),
							(vp + k * 9)[2]- (pA_->joints[j].posf[2]));

					fourNumber.productMatrix(floats,
							(&(pA_->joints[j].Matrix[16* i])));

					vp_[n * 3] += floats[0] * w;
					vp_[n * 3 + 1] += floats[1] * w;
					vp_[n * 3 + 2] += floats[2] * w;
				}

			}
			//LOGI("%d: %f,%f,%f",n,vp_[n*3],vp_[n*3+1],vp_[n*3+2]);
			//pGD->vp_[k * 3] = pGD->vp_[k * 3] / wAll;//- (pGD->animations->joints[0].posf[0])
			//pGD->vp_[k * 3 + 1] = pGD->vp_[k * 3 + 1] / wAll;//- (pGD->animations->joints[0].posf[1])
			//pGD->vp_[k * 3 + 2] = pGD->vp_[k * 3 + 2] / wAll;//- (pGD->animations->joints[0].posf[2])

		}
		//free(floats);
	}

	glGenBuffers(1, &(pA_->ubo) );
	glBindBuffer(GL_ARRAY_BUFFER, pA_->ubo);
	glBufferData(GL_ARRAY_BUFFER,(pA_->frameNum)*(pointerNum)*3*sizeof(float), vp_,
			GL_STATIC_DRAW);
	//glBufferSubData(GL_ARRAY_BUFFER,0,(pA_->frameNum)*(pointerNum)*3*sizeof(float), vp_);

}

void GraphData::setAniBuffers2(Movement* pM)
{
	pM->pA->pFrames=new Frame[pM->pA->frameNum];
	//(Frame *)malloc(pM->pA->frameNum*sizeof(Frame));

	//LOGI( "ani: %s", combineChars("./sdcard/",pM->fileName) );
	glGenBuffers(1, &(pM->pA->ubo) );

	for(int i=0;i<pM->pA->frameNum; i++)
	{
		pM->pA->pFrames[i].setInfo(i,Frame::TYPE_NORMAL );

	}
	char * chars=getLoadFile().getDataOfAniFile2(pM->fileName);
	//getCharsofFile(combineChars("./sdcard/",pM->fileName));//getCharsofFile(filePath);

	glBindBuffer(GL_ARRAY_BUFFER, pM->pA->ubo);
	glBufferData(GL_ARRAY_BUFFER,(pM->pA->frameNum)*(pointerNum)*3*sizeof(float), chars,
			GL_STATIC_DRAW);
	//delete[] chars;
	//chars=0;
}

void GraphData::updateMove(Animation *pA_,int frame)
{
	float floats[3];
	int n=0;
	for(int i=0;i<pA_->frameNum; i++)
	{
		//float *floats = (float*) malloc(3 * sizeof(float));
		FourNumber fourNumber = FourNumber(0, 0, 0);
		float w = 0;
		float wAll=0;
		for (int k = 0; k < pointerNum ; k++) {
			n=k;
			vp_[n * 3] = 0;
			vp_[n * 3 + 1] = 0;
			vp_[n * 3 + 2] = 0;
			w = 0;
			//wAll=0;
			for (int j = 0; j < (pA_->jointNum); j++) {
				w = weightsp[(pA_->jointNum) * k + j];

				if ( w != 0.0f) {
						//LOGI("%d: %f",k,w);
					//wAll+=w;
					fourNumber = FourNumber(
							(vp + k * 9)[0]- (pA_->joints[j].posf[0]),
							(vp + k * 9)[1]- (pA_->joints[j].posf[1]),
							(vp + k * 9)[2]- (pA_->joints[j].posf[2]));

					fourNumber.productMatrix(floats,
							(&(pA_->joints[j].Matrix[16* frame])));

					vp_[n * 3] += floats[0] * w;
					vp_[n * 3 + 1] += floats[1] * w;
					vp_[n * 3 + 2] += floats[2] * w;
				}

			}
			//LOGI("%d: %f,%f,%f",n,vp_[n*3],vp_[n*3+1],vp_[n*3+2]);
			//pGD->vp_[k * 3] = pGD->vp_[k * 3] / wAll;//- (pGD->animations->joints[0].posf[0])
			//pGD->vp_[k * 3 + 1] = pGD->vp_[k * 3 + 1] / wAll;//- (pGD->animations->joints[0].posf[1])
			//pGD->vp_[k * 3 + 2] = pGD->vp_[k * 3 + 2] / wAll;//- (pGD->animations->joints[0].posf[2])

		}
		//free(floats);
	}

	glGenBuffers(1, &(pA_->ubo) );
	glBindBuffer(GL_ARRAY_BUFFER, pA_->ubo);
	glBufferData(GL_ARRAY_BUFFER,(pA_->frameNum)*(pointerNum)*3*sizeof(float), vp_,
			GL_STATIC_DRAW);
	//glBindBuffer(GL_ARRAY_BUFFER, gp->batch_ubo[0]);
	//glBufferSubData(GL_ARRAY_BUFFER,9*sizeof(float),(pointerNum)*3*sizeof(float), vp_);

}

void GraphData::getFramePointer(float *pf3, Animation *pA, int frame,int num)
{
	float w = 0;
	float floats[3]={0,0,0};
	FourNumber fourNumber = FourNumber(0, 0, 0);
	//wAll=0;
	for (int j = 0; j < (pA->jointNum); j++) {
		w = (*(weightsp + ((pA->jointNum) * num + j)));
		if ( w != 0.0f) {

			//wAll+=w;
			fourNumber = FourNumber(
					(vp + num * 8)[0]- (pA->joints[j].posf[0]),
					(vp + num * 8)[1]- (pA->joints[j].posf[1]),
					(vp + num * 8)[2]- (pA->joints[j].posf[2]));

			fourNumber.productMatrix(floats,
					(&(pA->joints[j].Matrix[16* frame])));

			pf3[0] += floats[0] * w;
			pf3[1] += floats[1] * w;
			pf3[2] += floats[2] * w;
		}

	}
}

void GraphData::setBatchs(int num)
{
	if(num==1)
	{
		glGenBuffers(1, batch_ubo);
		glBindBuffer(GL_ARRAY_BUFFER, batch_ubo[0]);
		glBufferData(GL_ARRAY_BUFFER,
				pointerNum * 9 * sizeof(float) , vp,
			GL_STATIC_DRAW);


		glGenBuffers(1, batch_uboi);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch_uboi[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
			pmtls[0]->viL * sizeof(unsigned short), pmtls[0]->vip,
			GL_STATIC_DRAW);

	}else if(num>1)
	{

		batch_v=new float[pointerNum * 9 * num];
		batch_vi=new unsigned short[pmtls[0]->viL*num];

		//LOGI("batch: %d, %d",pointerNum * 9 * num,pmtls[0]->viL*num);
		int iTemp=0;
		int viTemp=0;
		unsigned short * pvi=pmtls[0]->vip;

		for(int i=0;i<num;i++)
		{
			iTemp=i * pointerNum * 9;
			memcpy( &batch_v[ iTemp ], vp, sizeof(float)*pointerNum * 9);
			for(int j=0;j<pointerNum;j++)
			{
				batch_v[ iTemp+ j*9+ 8]=(float)i;
			}

			iTemp=i * pointerNum;
			viTemp=pmtls[0]->viL * i;
			for(int j=0;j<pmtls[0]->viL;j++)
			{
				batch_vi[ viTemp + j ] =(short)( pvi[j]+iTemp );
			}

		}

		glGenBuffers(1, batch_ubo);
		glBindBuffer(GL_ARRAY_BUFFER, batch_ubo[0]);
		glBufferData(GL_ARRAY_BUFFER,
				pointerNum * 9 * num * sizeof(float) , batch_v,
				GL_STATIC_DRAW);


		glGenBuffers(1, batch_uboi);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, batch_uboi[0]);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,
				pmtls[0]->viL * num * sizeof(unsigned short), batch_vi,
				GL_STATIC_DRAW);

		delete[] batch_v;
		batch_v=0;
		delete[] batch_vi;
		batch_vi=0;

	}
}
