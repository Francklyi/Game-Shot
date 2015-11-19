#include "scene.h"
#include "fileDeal/getMtl.h"
#include "fileDeal/loadFile.h"
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <string.h>
#include "AEA.h"
#include "Entitylist.h"
//#include "fileDeal/loadFile.h"

Scene::Scene(char *fileName_,Map *map_)
{
 // LOGI("Scene0");
  LOGE("Scene begin");
	map=map_;
	fileName=fileName_;
	LOGI(fileName);
	next=map->getNextScene();
	pre=map->getPreScene();
	LOGE("Scene1");
	entityFactoryP=new EntityFactory();
	LOGE("Scene2");
	loadXml();
	LOGE("Scene Done1");
	loadData();
	//LOGI("Scene Done2");
}

void Scene::loadXml(){
	//chars=getCharsofFile(fileName);
	LOGI(fileName);
	//fileName=fileName_;
	next=map->getNextScene();
	pre=map->getPreScene();
	//LOGI("%d",next);
	xmlDocPtr doc = NULL;
	xmlNodePtr node = NULL;//roleClass
	xmlNodePtr node1 = NULL;//AEA
	xmlNodePtr node2 = NULL;//pos

	xmlKeepBlanksDefault(0);
	char *path1=combineChars(path0,fileName);
	LOGI(path1);
	doc = xmlParseFile(path1);

	if(doc == NULL)
	{
		return ;
	}

	//get root node
	node = xmlDocGetRootElement(doc);//Scene

	if(node == NULL)
	{
		xmlFreeDoc(doc);
		return ;
	}
	//char temp[]="my_scene1_7.ly";
	geoFile=strCopy((char*)node->properties->children->content);
	LOGI("scene:geoFile:%s",geoFile);
	//temp;
	//walk the tree
	node = node->xmlChildrenNode; //Entitylists
	int roleClassNum=atoi((char*)node->properties->children->content);
	//roleFactoryPtr->initRCPtrs(roleClassNum);
	node = node->xmlChildrenNode;  //Entitylist1
	LOGI("roleClassNum:%d",roleClassNum);
	for(int i=0;i<roleClassNum;++i)
	{

	    int roleNum=atoi((char*)node->properties->children->content);
	    char *xmlFile =strCopy((char*)node->properties->next->children->content);
	    LOGI("Entitylist,xmlFile:%s",xmlFile);
	    int id=atoi((char*)node->properties->next->next->children->content);

	    Entitylist *entitylistp_=new Entitylist(id,xmlFile);

	    entityFactoryP->addEntitylist(entitylistp_);

	    node1=node->xmlChildrenNode; //AEA1
	    LOGI("roleNum:%d",roleNum);
	    for(int j=0;j<roleNum;++j)
		{
			xmlAttrPtr attrPtr=node1->properties;
			int id=atoi((char*)attrPtr->children->content);
			//LOGI("id:%d",id);

			//attrPtr=attrPtr->next;
			//char *geoFile1=strCopy((char*)attrPtr->children->content);

			attrPtr=attrPtr->next;
			int grade=atoi((char*)attrPtr->children->content);
			//LOGI("grade:%d",grade);
			attrPtr=attrPtr->next;
			int bloodLimit=atoi((char*)attrPtr->children->content);

			attrPtr=attrPtr->next;
			int magicLimit=atoi((char*)attrPtr->children->content);

			attrPtr=attrPtr->next;
			int attack=atoi((char*)attrPtr->children->content);

			attrPtr=attrPtr->next;
			int defense=atoi((char*)attrPtr->children->content);

			//attrPtr=attrPtr->next;

			Attribute *roleAttrPtr=new Attribute(
					grade,
					bloodLimit,
					magicLimit,
					attack,
					defense
					);

			AEA *aea= new AEA(roleAttrPtr,id);

			node2=node1->xmlChildrenNode; //pos
			attrPtr=node2->properties;
			int x=atoi((char*)attrPtr->children->content);
			attrPtr=attrPtr->next;
			int y=atoi((char*)attrPtr->children->content);
			attrPtr=attrPtr->next;
			int z=atoi((char*)attrPtr->children->content);

			aea->setPos(x,y,z);

			node2=node2->next;
			attrPtr=node2->properties;

                        float x1=atof((char*)attrPtr->children->content);
                        attrPtr=attrPtr->next;
                        float y1=atof((char*)attrPtr->children->content);
                        attrPtr=attrPtr->next;
                        float z1=atof((char*)attrPtr->children->content);
                        btVector3 rotate(x1,y1,z1);
                        LOGI("rotate:%f,%f,%f",x1,y1,z1);
                        aea->setRotate(rotate);
//LOGI("Pos:%d,%d,%d",x,y,z);

			entitylistp_->registerRole(aea);
			//entitylistp_=entityFactoryP->getRoleClass(aea);
			node1=node1->next;
			LOGI("for2 done");
		}
		//Skills
		int movement_num=atoi((char*)node1->properties->children->content);

		entitylistp_->setNum_movements(movement_num);
		int *id_movements=(int *)malloc(sizeof(int)*movement_num);
		node1=node1->xmlChildrenNode; //movement1
		for(int k=0;k<movement_num;++k)
		  {
		    id_movements[k]=atoi((char*)node1->children->content);

		    node1=node1->next;
                    //char *movementFile=strCopy((char*)node1->properties->next->children->content);
                   // LOGI("geoFile:%s",movementFile);
                   // Movement *movement;
                   // entitylistp_->add_movement(movement);
		  }
		LOGI("movement_num:%d", movement_num);

		entitylistp_->setHavingMovementids(id_movements);
		node = node -> next;
		LOGI("id_movements[k]:%d", id_movements[0]);
	}

	//release resource
	xmlFreeDoc(doc);
	xmlCleanupParser();
	//roleFactory.init()

	//load entityFactoryP loadXml();
	//LOGI("EFLX");
	//entityFactoryP->loadXml();
}

bool Scene::loadData()
{
	LOGI("Scene::loadData:%s",geoFile);
	graph=new Graph();
	graph->setFileName(geoFile);
	graph->load();
}

bool Scene::loadConfiguration()
{

}

bool Scene::update()
{
	//LOGI("Scene::update 0");
  if(true||!glsl->isShadowMode()){
        glsl->chooseMatrix(GLSL::MMATRIX);
	glsl->pushMatrix();

	//glsl->scale(0.2f,0.2f,0.2f);
	graph->draw();
	//LOGI("Scene::update 1");
	glsl->popMatrix();
  }
	entityFactoryP->update();//记得改回来
}

void Scene::setData(){

}

void Scene::init(char *fileName_){
	//loadFile();
	next=map->getNextScene();
	pre=map->getPreScene();

}

void Scene::toNext()
{
	map->setScenePos(next);
	fileName=map->getScenePath();
	load();
}

void Scene::toPre()
{
	map->setScenePos(pre);
	fileName=map->getScenePath();
	load();
}

Scene::~Scene()
{
	LOGI("~Scene0");
	//delete SceneGeom;
	graph=0;
}
