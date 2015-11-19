#pragma once
#include "graphic/GraphData.h"

class Parser
{
public:
	char *pfileName;
	GraphData *gp;

	Parser();
	~Parser();

	void setFileName(char *pfileName_){ pfileName=pfileName_;}
	void setPGraphData(GraphData *gp_){ gp = gp_;}

	virtual void parse(){}

};
