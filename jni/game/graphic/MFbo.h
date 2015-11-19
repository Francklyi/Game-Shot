#ifndef _FBO_OBJECT_
#define _FBO_OBJECT_
#include <GLES2/gl2.h>

class MFbo{
public:
	GLuint fboHDR[1];

	GLuint fboTexture[1];

	GLuint RenderBfName[1];

	int width;

	int height;

};

#endif
