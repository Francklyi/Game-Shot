#pragma once
#include "Parser.h"
#define BYTE unsigned char

class ParserLy:public Parser
{
public:
	ParserLy();
	~ParserLy();
	void getMtls(char **cp);

	void getOneMtl(char **cp, Mtl *pMtl);

	GLuint setTexture(char **cp);

	GLuint setTexture(BYTE *pixels,int w, int h,unsigned int format);

	GLuint setTextureB(char *map_Kd);

	int getNewmtlNum(char **cp) {
		return (int) *((*cp)++);
	}

	virtual void parse();

	void update();
};

extern ParserLy *pParserLy0;
