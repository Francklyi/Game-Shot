#pragma once
#include "graphic/Graph.h"
#include <vector>
#include "RenderEntity.h"
#include "RenderEntityGroup.h"
#include <GLES2/gl2.h>

#include "graphic/MFbo.h"

using std::vector;

class RenderManager
{
public:
	vector<Graph*> vpGraph;

	vector<RenderEntity*> vpRE;

	vector<RenderEntityGroup *> *pvpREGRender;

	vector<RenderEntityGroup *> *pvpREGNext;

	vector<RenderEntityGroup *> *pvpREG1;

	vector<RenderEntityGroup *> *pvpREG2;

	GLuint fboHDRFull;

	//GLuint RenderBfName;

	GLuint fboTextureFull;

	GLuint fboHDR1d4;

	GLuint fboTexture1d4;

	GLuint fboHDR1d8;

	GLuint fboTexture1d8;

	MFbo fbo1;

	MFbo fbo1d4;

	MFbo fbo1d8;

	MFbo fbo32;

	RenderEntity *pREFullScreenSquare;

	Graph *pGFullSquare;

	float aveLight;
	float lumAdapt;

public:
	RenderManager();

	void addGraph(Graph* pGraph);

	void addRenderEntity(Graph *pGraph);

	void clear();

	void render();

	void renderObjects();

	void addREG(GraphData* gp);

	void changeFrame();

	void getFboHDR(int w, int h, int internalformat, int texFormat, MFbo &fbo);

	void renderToFBO();

	void initFullScreenSquare();

	void initFboHDR(int w, int h);

	void doGaosi(MFbo *pfbo);

	void doLogAverage();

};

extern RenderManager *pRenderManager;
