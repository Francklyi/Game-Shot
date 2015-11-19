#include "loadFile.h"
#include "log.h"

char lyfile1[]="my/lyfile/";// my/lyfile/

char tex1[]="my/gamepics/";

char layout1[]="my/layout/";

char ani1[]="my/ani/";

char ani21[]="my/ani2/";


char *LoadFile::lyfile=lyfile1;

char *LoadFile::tex=tex1;

char *LoadFile::layout=layout1;

char *LoadFile::ani=ani1;

char *LoadFile::ani2=ani21;

LoadFile::LoadFile()
{
	loadFile_buffer1=0;

	loadFile_bufferLen1=0;

	loadFile_buffer2=0;

	loadFile_bufferLen2=0;

	loadFile_buffer3=0;

	loadFile_bufferLen3=0;

	loadFile_buffer4=0;

	loadFile_bufferLen4=0;
}

LoadFile& getLoadFile(){
	static LoadFile loadFile;
	return loadFile;
}

char *LoadFile::getCharsofFile(char *FileName){
	FILE * fp;
	if((fp=fopen(FileName,"rb"))==NULL) {
	    LOGE("can't find file:%s",FileName);
		return 0;
	}
	fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    const long longBytes=ftell(fp);// longBytes�����ļ��ĳ���
	char * cp=new char[longBytes];
	fseek(fp,0,SEEK_SET);
	fread(cp,1,longBytes,fp);
	fclose(fp);
    return cp;
}

char *LoadFile::getDataOfLyoutFile(char *FileName)
{
	//char * filgpath=combineChars(layoutPath,FileName);

	//return getCharsofFile(filgpath);
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/layout");
	/*获取文件名并打开*/
	char *path=combineChars(layout,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}
	/*获取文件大小*/
	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen1)
	{
		if(loadFile_bufferLen1>0)
			delete[] loadFile_buffer1;
		loadFile_bufferLen1=bufferSize;

		loadFile_buffer1=new char[loadFile_bufferLen1+1];
	}
	loadFile_buffer1[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer1, bufferSize);
	/*关闭文件*/
	AAsset_close(asset);

	return loadFile_buffer1;

	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfLyFile(char *FileName)
{
	//char * filgpath=combineChars(lyPath,FileName);

	//return getCharsofFile(filgpath);
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/lyfile");
	/*获取文件名并打开*/
	char *lyPath=combineChars(lyfile,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager,lyPath,AASSET_MODE_UNKNOWN);

	LOGI("LyFile: %s",FileName);
	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}
	/*获取文件大小*/
	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen2)
	{
		if(loadFile_bufferLen2>0)
			delete[] loadFile_buffer2;
		loadFile_bufferLen2=bufferSize;

		loadFile_buffer2=new char[loadFile_bufferLen2+1];
	}
	loadFile_buffer2[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer2, bufferSize);
	/*关闭文件*/
	AAsset_close(asset);

	return loadFile_buffer2;



	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfAniFile(char *FileName)
{
	//char * filgpath=combineChars(aniPath,FileName);

	//return getCharsofFile(filgpath);
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/ani");
	/*获取文件名并打开*/
	//jboolean iscopy;
	char *path=combineChars(ani,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}
	/*获取文件大小*/
	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen3)
	{
		if(loadFile_bufferLen3>0)
			delete[] loadFile_buffer3;
		loadFile_bufferLen3=bufferSize;

		loadFile_buffer3=new char[loadFile_bufferLen3+1];
	}
	loadFile_buffer3[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer3, bufferSize);

	/*关闭文件*/
	AAsset_close(asset);

	return loadFile_buffer3;


	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfAniFile2(char *FileName)
{
	//char * filgpath=combineChars(ani2Path,FileName);

	//return getCharsofFile(filgpath);
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/ani");
	/*获取文件名并打开*/
	//jboolean iscopy;
	char *path=combineChars(ani2,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}
	/*获取文件大小*/

	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen4)
	{
		if(loadFile_bufferLen4>0)
			delete[] loadFile_buffer4;
		loadFile_bufferLen4=bufferSize;

		loadFile_buffer4=new char[loadFile_bufferLen4+1];
	}
	loadFile_buffer4[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer4, bufferSize);
	/*关闭文件*/
	AAsset_close(asset);
	return loadFile_buffer4;

	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfPicFile(char *FileName, int *length)
{
	/*
	char * filgpath=combineChars(texPath,FileName);

	FILE * fp;
	if((fp=fopen(filgpath,"rb"))==NULL) {
	    LOGE("can't find file:%s",filgpath);
		return 0;
	}
	fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    const long longBytes=ftell(fp);
    (*length)=longBytes;
	char * cp=new char[longBytes];
	fseek(fp,0,SEEK_SET);
	fread(cp,1,longBytes,fp);
	fclose(fp);
    return cp;
*/
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/gamepics");
	/*获取文件名并打开*/

	char *path=combineChars(tex,FileName);

	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}
	/*获取文件大小*/
	(*length) = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
/*
	char *buffer=new char[(*length)+1];
	buffer[(*length)]=0;
	int numBytesRead = AAsset_read(asset, buffer, (*length));
	//关闭文件
	AAsset_close(asset);

	return buffer;

*/
	off_t bufferSize =(*length);// AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen4)
	{
		if(loadFile_bufferLen4>0)
			delete[] loadFile_buffer4;
		loadFile_bufferLen4=bufferSize;

		loadFile_buffer4=new char[loadFile_bufferLen4+1];
	}
	loadFile_buffer4[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer4, bufferSize);
	/*关闭文件*/
	AAsset_close(asset);
	return loadFile_buffer4;
	//AAssetDir_close(assetDir);
}

/*
char * LoadFile::combineChars(char * A,char *B){
	int n=0,m=0;
	while((*(A+n))!='\n') n++;
	while((*(B+m))!='\n') m++;
	char *newAB=(char*)malloc(m+n);
	n=0;

	while(A[n]!='\n'){
		newAB[n]=A[n];
		n++;
	}
	//LOGI("B %d %d \n",m ,B[m]);
	m=0;
	while(B[m]!='\n'){
		newAB[n]=B[m];
		n++;
		m++;
	}
	return newAB;
}
*/
char * LoadFile::combineChars(const char * A,const char *B){
	int n=strlen(A),m=strlen(B);
	char *newAB=new char[sizeof(char)*(m+n+1)];
	for(int i=0;i<n;++i)
	{
		newAB[i]=A[i];
	}
	for(int j=0;j<m;++j)
	{
		newAB[n+j]=B[j];
	}
	newAB[n+m]=0;
	return newAB;
}

char * LoadFile::strCopy(char *str_)
{
	char *str=new char[strlen(str_)];
	strcpy(str,str_);
	return str;
}

int LoadFile::getInt(char **cp){
	int i=0;
	i+=*((*cp)++);
	i+=(*((*cp)++))*128;
	i+=(*((*cp)++))*16384;
	i+=(*((*cp)++))*2097152;
	return i;
}

void LoadFile::clear()
{
	return;
	if(loadFile_buffer1)
	{
		delete[] loadFile_buffer1;
		loadFile_buffer1=0;
		loadFile_bufferLen1=0;
	}
	if(loadFile_buffer2)
	{
		delete[] loadFile_buffer2;
		loadFile_buffer2=0;
		loadFile_bufferLen2=0;
	}
	if(loadFile_buffer3)
	{
		delete[] loadFile_buffer3;
		loadFile_buffer3=0;
		loadFile_bufferLen3=0;
	}
	if(loadFile_buffer4)
	{
		delete[] loadFile_buffer4;
		loadFile_buffer4=0;
		loadFile_bufferLen4=0;
	}
}

LoadFile *pLoadFile=new LoadFile();




/*

char *LoadFile::getCharsofFile(char *FileName){
	FILE * fp;
	if((fp=fopen(FileName,"rb"))==NULL) {
	    LOGE("can't find file:%s",FileName);
		return 0;
	}
	fseek(fp,0,SEEK_SET);
    fseek(fp,0,SEEK_END);
    const long longBytes=ftell(fp);// longBytes�����ļ��ĳ���
	char * cp=new char[longBytes];
	fseek(fp,0,SEEK_SET);
	fread(cp,1,longBytes,fp);
	fclose(fp);
    return cp;
}

char *LoadFile::getDataOfLyoutFile(char *FileName)
{
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/layout");

	char *path=combineChars(layout,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}

	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen1)
	{
		if(loadFile_bufferLen1>0)
			delete[] loadFile_buffer1;
		loadFile_bufferLen1=bufferSize;

		loadFile_buffer1=new char[loadFile_bufferLen1+1];
	}
	loadFile_buffer1[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer1, bufferSize);

	AAsset_close(asset);

	return loadFile_buffer1;

	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfLyFile(char *FileName)
{
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/lyfile");

	char *lyPath=combineChars(lyfile,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager,lyPath,AASSET_MODE_UNKNOWN);

	LOGI("LyFile: %s",FileName);
	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}

	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen2)
	{
		if(loadFile_bufferLen2>0)
			delete[] loadFile_buffer2;
		loadFile_bufferLen2=bufferSize;

		loadFile_buffer2=new char[loadFile_bufferLen2+1];
	}
	loadFile_buffer2[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer2, bufferSize);

	AAsset_close(asset);

	return loadFile_buffer2;



	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfAniFile(char *FileName)
{
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/ani");

	//jboolean iscopy;
	char *path=combineChars(ani,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}

	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen3)
	{
		if(loadFile_bufferLen3>0)
			delete[] loadFile_buffer3;
		loadFile_bufferLen3=bufferSize;

		loadFile_buffer3=new char[loadFile_bufferLen3+1];
	}
	loadFile_buffer3[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer3, bufferSize);


	AAsset_close(asset);

	return loadFile_buffer3;


	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfAniFile2(char *FileName)
{
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/ani");

	//jboolean iscopy;
	char *path=combineChars(ani2,FileName);
	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}


	off_t bufferSize = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen4)
	{
		if(loadFile_bufferLen4>0)
			delete[] loadFile_buffer4;
		loadFile_bufferLen4=bufferSize;

		loadFile_buffer4=new char[loadFile_bufferLen4+1];
	}
	loadFile_buffer4[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer4, bufferSize);

	AAsset_close(asset);
	return loadFile_buffer4;

	//AAssetDir_close(assetDir);
}

char *LoadFile::getDataOfPicFile(char *FileName, int *length)
{
	//AAssetDir* assetDir = AAssetManager_openDir(mAssetManager,"my/gamepics");


	char *path=combineChars(tex,FileName);

	AAsset* asset = AAssetManager_open(mAssetManager, path,AASSET_MODE_UNKNOWN);

	if(asset==NULL)
	{
		LOGI(" %s","asset==NULL");
		return NULL;
	}

	(*length) = AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);

	off_t bufferSize =(*length);// AAsset_getLength(asset);
	//LOGI("file size : %d\n",bufferSize);
	if(bufferSize>loadFile_bufferLen4)
	{
		if(loadFile_bufferLen4>0)
			delete[] loadFile_buffer4;
		loadFile_bufferLen4=bufferSize;

		loadFile_buffer4=new char[loadFile_bufferLen4+1];
	}
	loadFile_buffer4[bufferSize]=0;
	int numBytesRead = AAsset_read(asset, loadFile_buffer4, bufferSize);

	AAsset_close(asset);
	return loadFile_buffer4;
	//AAssetDir_close(assetDir);
}

*/
