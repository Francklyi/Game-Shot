#pragma once
#include "MTexture.h"
#include <vector>

using std::vector;

class TextureFactory
{
public:
	vector<MTexture*> vMTexture;

public:
	TextureFactory();
	GLuint getTexID(char *fileName);

};


extern TextureFactory *pTexFactory;
