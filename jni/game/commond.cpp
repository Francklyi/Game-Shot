#include "commond.h"
#include "TextureFactory.h"
#include "Screen2d.h"

void initEngine()
{
	//glsl=new GLSL();

	pRenderManager=new RenderManager();
	pTexFactory=new TextureFactory();
	pScreen2d=new Screen2d();
}
