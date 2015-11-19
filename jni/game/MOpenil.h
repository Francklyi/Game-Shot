#ifndef _MOPENIL_H_
#define _MOPENIL_H_

#include <GLES2/gl2.h>
#include <IL/il.h>
#include <IL/ilu.h>

class MOpenil
{
public:
	int w; 
	int h;
	unsigned char *pixels;
	ILuint ID;
	ILuint format;
public:
	MOpenil();
	~MOpenil();
	
	void init();
	bool loadImage(char *fileName);
	void clearImage();

	GLuint loadTexture(char *fileName);

protected:
private:
};

MOpenil& getMOL();

//extern MOpenil *pMOL;

#endif //_MOPENIL_H_
