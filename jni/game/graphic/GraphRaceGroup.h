#pragma once
#include "GraphRace.h"
#include "Graph.h"

class GraphRaceGroup
{
public:
	vector<GraphRace*> vpGR;

	GraphRaceGroup();
	~GraphRaceGroup();
	void add(Graph*);
	void remove(Graph*);
	void clear();
protected:
private:
};