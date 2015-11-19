#include "GraphRaceGroup.h"
#include <string>
using std::string;

GraphRaceGroup::GraphRaceGroup()
{
	vpGR=vector<GraphRace*>(0);
}

GraphRaceGroup::~GraphRaceGroup()
{
	for(int i=0; i<vpGR.size(); i++)
	{
		if(vpGR[i])
			free(vpGR[i]);
	}
	vpGR.clear();
}

void GraphRaceGroup::add(Graph* pg)
{
	for(int i=0; i<vpGR.size(); i++)
	{
		if (vpGR[i]->strFileName==pg->fileStr)
		{
			//文件名相同，即为同类
			vpGR[i]->add(pg);
			return;
		}
	}
	GraphRace* pgr=new GraphRace();
	pgr->add(pg);
	vpGR.push_back(pgr);
}

void GraphRaceGroup::remove(Graph* pg)
{
	for(int i=0; i<vpGR.size(); i++)
	{
		if (vpGR[i]->strFileName==pg->fileStr)
		{
			//文件名相同，即为同类
			
			if(vpGR[i]->remove(pg)==-1) 
				vpGR.erase(vpGR.begin()+i);
			return;
		}
	}
}

void GraphRaceGroup::clear()
{
	for(int i=0; i<vpGR.size(); i++)
	{
		vpGR[i]->clear();
	}
	vpGR.clear();
}
