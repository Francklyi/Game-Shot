#include "roadpart.h"

RoadPart::RoadPart()
{

}

RoadPart::RoadPart(char *fileName_ ):
fileName(fileName)
{
  this->load();
}

RoadPart::~RoadPart()
{

}

void RoadPart::setFile(char *fileName_)
{
  fileName = fileName_;
}

bool RoadPart::load()
{
 // LOGI("RoadPart::load");
  //char geols[]="manModel2-1.ly";//"jiliye3_1.ly";
  graph=new Graph();
  graph->setFileName(fileName);
  graph->load();
  return true;
}

bool RoadPart::update()
{
  this->draw();
}

void RoadPart::draw()
{
  glsl->pushMatrix();

  graph->draw();

  glsl->popMatrix();
}
/*
void RoadPart::regester(Roads *roads)
{
  this->id = roads->addRoadPart(this);
}
*/
