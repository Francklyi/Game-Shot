#ifndef _LOADFILE_H_
#define _LOADFILE_H_

#include <stdio.h>
#include <stdlib.h>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>


class LoadFile
{
public:

	static char *lyfile;
	static char *tex;
	static char *layout;
	static char *ani;
	static char *ani2;

	char *pagPath;
	char *lyPath;
	char *texPath;
	char *layoutPath;
	char *aniPath;
	char *ani2Path;

	char *loadFile_buffer1;

	int loadFile_bufferLen1;

	char *loadFile_buffer2;

	int loadFile_bufferLen2;

	char *loadFile_buffer3;

	int loadFile_bufferLen3;

	char *loadFile_buffer4;

	int loadFile_bufferLen4;

	AAssetManager* mAssetManager;

	LoadFile();

	char * getCharsofFile(char *FileName);

	//char * combineChars(char * A,char *B);

	char * combineChars(const char * A,const char *B);

	char * strCopy(char *str);

	int getInt(char **cp);

	char *getDataOfLyoutFile(char *FileName);

	char *getDataOfLyFile(char *FileName);

	char *getDataOfAniFile(char *FileName);

	char *getDataOfAniFile2(char *FileName);

	char *getDataOfPicFile(char *FileName, int *length);

	void clear();
};

LoadFile& getLoadFile();

//extern LoadFile *pLoadFile;

#endif  //_LOADFILE_H_
