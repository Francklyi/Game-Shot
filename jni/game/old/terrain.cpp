#include "terrain.h"
#include "fileDeal/loadFile.h"
#include "log.h"

Terrain::Terrain(char *fileName1)
{
	fileName=fileName1;
	//cellWidth=1.0f;
}

Terrain::~Terrain()
{
	//if(fileName) delete fileName;
	fileName=0;
	if(heightArray) free(heightArray);
	heightArray=0;
}

void Terrain::init(){
	//cellWidth=1;
	loadRawFile(fileName);
}

bool Terrain::loadRawFile(char *fileName1){
	char * charData=pLoadFile->getCharsofFile(fileName1);
	if(!charData) return false;
	width=*(int *)charData;
	charData+=4;
	height=*(int *)charData;

	charData+=4;
	cellWidth=*(float *)charData;
	charData+=4;
	LOGI("loadRawFile %d , %d, %f\n", width,height,cellWidth);
	//heightArray=(float *)malloc(width*height*sizeof(float)*4);
	heightArray=(float *)charData;

	X0=-cellWidth * (width-1)/2;
	Y0=-cellWidth * (height-1)/2;
	//LOGI("XY0 %f , %f\n", X0,Y0);
	return true;
}

float Terrain::getHeight(float x, float y){
	if(!heightArray) return 0;
	//int xx=x;//%width;
	//int yy=y;//%height;
	float x_ = x - X0;
	float y_ = y - Y0;
	int xi=x_/cellWidth;
	int yi=y_/cellWidth;
	//LOGI("h : %f , %f, %d , %d",x_,y_,xi,yi);
	float h00=heightArray[yi*width+xi];//heightArray[xi*width+yi]
	float h01=heightArray[yi*width+xi+1];
	float h10=heightArray[(yi+1)*width+xi];
	float h11=heightArray[(yi+1)*width+xi+1];
	//LOGI("h : %f , %f, %f , %f",h00,h01,h10,h11);
	float tx=(x_-xi*cellWidth)/cellWidth;
	float ty=(y_-yi*cellWidth)/cellWidth;

	float txty=tx * ty;
	float h = h00*(1.0f- ty -tx +txty)
			+ h01 * (tx- txty)+h11* txty
			+ h10 * (ty - txty);
	//if(xyz[2]>100||xyz[2]<-100) xyz[2]=0;
	//LOGE("xy:: %f, %f\n",xy[0] , xy[1]);
	return h;
}

int Terrain::getSquaretype(float x, float y)
{
	if(!heightArray) return 0;
	float x_ = x - X0;
	float y_ = y - Y0;
	int xi=x_/cellWidth;
	int yi=y_/cellWidth;

	float h00=heightArray[yi*width+xi];
	float h01=heightArray[yi*width+xi+1];
	float h10=heightArray[(yi+1)*width+xi];
	float h11=heightArray[(yi+1)*width+xi+1];

	if(h00==h01&&h00==h10&&h00==h11)
		return SQUARE_LOAD;
	else
		return SQUARE_TABLE;


	float Limit=0.1;
	float temp=0;
	temp=h00<=h01? h00: h01;
	if(h10<=temp) temp=h10;
	if(h11<=temp) temp=h11;

	if(h00-temp<Limit&&h01-temp<Limit&&h10-temp<Limit&&h11-temp<Limit)
		return SQUARE_LOAD;
	else
		return SQUARE_TABLE;

	float dx1=x_-xi*cellWidth;
	float dy1=y_-yi*cellWidth;

	float dx2=cellWidth-dx1;
	float dy2=cellWidth-dy1;

	return SQUARE_LOAD;
}

float *Terrain::getNearLine(float x, float y)
{
	if(!heightArray) return 0;
	float x_ = x - X0;
	float y_ = y - Y0;
	int xi=x_/cellWidth;
	int yi=y_/cellWidth;

	float dx1=x_-xi*cellWidth;
	float dy1=y_-yi*cellWidth;

	float dx2=cellWidth-dx1;
	float dy2=cellWidth-dy1;

	float temp=0;
	temp=dx1<=dy1? dx1: dy1;
	if(dx2<=temp) temp=dx2;

	float *pLine=new float(2);
	if(dy2<=temp)
	{
		/*
		pLine[0]=xi*cellWidth+ X0;
		pLine[1]=(yi+1)*cellWidth+ Y0;
		pLine[2]=(xi+1)*cellWidth+ X0;
		pLine[3]=(yi+1)*cellWidth+ Y0;
		*/
		pLine[0]=1;
		pLine[1]=0 ;
	}
	else
		if(dx1<=temp)
		{
			pLine[0]=0;
			pLine[1]=1;
		}
		else
			if(dy1<=temp)
			{
				pLine[0]=1;
				pLine[1]=0;
			}
			else
				if(dx2<=temp)
				{
					pLine[0]=0;
					pLine[1]=1;
				}
	return pLine;
}

float * Terrain::getXY(float xl , float yl ,float xn , float yn){
	return 0;
	/*
	float *xyz=(float *)malloc(3*sizeof(float));
	if(!heightArray) return 0;
	float Xl = xl/cellWidth, Yl = yl/cellWidth;
	float Xn = xn/cellWidth, Yn = yn/cellWidth;
	Xl+=5;
	Yl+=3;
	Xn+=5;
	Yn+=3;
	//LOGE("XY:%f , %f \n", Xl , Yl);
	int col0=int(Xn);
	int row0=int(Yl);
	int col1=col0+1;
	int row1=row0+1;

	if(col0<1.0||col1>width-1) xn=xl;
	else{
	//��ȡ��Ԫ��4���ǵĸ߶�
	float h00=(float)getHeight(col0,row0);
	float h01=(float)getHeight(col1,row0);
	float h10=(float)getHeight(col0,row1);
	float h11=(float)getHeight(col1,row1);
	//LOGE("h:: %f ,%f ,%f ,%f\n",h00,h01,h10,h11);
	if(h00<-100||h01<-100||h10<-100||h11<-100) {
		xn=xl;

	}

	}
	col0=int(Xl);
	row0=int(Yn);
	col1=col0+1;
	row1=row0+1;

	if(row0<1.0||row1>height-1) yn=yl;
	else{
		//��ȡ��Ԫ��4���ǵĸ߶�
		float h00=(float)getHeight(col0,row0);
		float h01=(float)getHeight(col1,row0);
		float h10=(float)getHeight(col0,row1);
		float h11=(float)getHeight(col1,row1);
		if(h00<-100||h01<-100||h10<-100||h11<-100) yn=yl;

	}
	xyz[0]=xn;
	xyz[1]=yn;

	col0=int(Xn);
	row0=int(Yn);
	col1=col0+1;
	row1=row0+1;
	//��ȡ��Ԫ��4���ǵĸ߶�
	float h00=(float)getHeight(col0,row0);
	float h01=(float)getHeight(col1,row0);
	float h10=(float)getHeight(col0,row1);
	float h11=(float)getHeight(col1,row1);

	float tx=Xn-int(Xn);
	float ty=Yn-int(Yn);

	float txty=tx * ty;
	xyz[2] = h00*(1.0f- ty -tx +txty)
			+ h01 * (tx- txty)+h11* txty
			+ h10 * (ty - txty);
	if(xyz[2]>100||xyz[2]<-100) xyz[2]=0;
	//LOGE("xy:: %f, %f\n",xy[0] , xy[1]);
	return xyz;
	*/
}
