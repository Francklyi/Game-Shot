#include "TextureFactory.h"

TextureFactory::TextureFactory()
{
	vMTexture=vector<MTexture*>(0);
}

GLuint TextureFactory::getTexID(char *fileName)
{
	string str(fileName);
	for(int i=0;i<vMTexture.size();i++)
	{
		if(vMTexture[i]->strFileName==str)
			return vMTexture[i]->mTextureId;
	}
	MTexture *pMT=new MTexture();
	pMT->setFileName(fileName);
	pMT->load();
	vMTexture.push_back(pMT);
	return pMT->mTextureId;
}

TextureFactory *pTexFactory=0;
