#pragma once
#include "GraphData.h"
#include "Graph.h"
#include "Object.h"

#include <string>
using std::string;
/************************************************************************/
/* race of objects of Graph  with same GraphData     */
/************************************************************************/
class GraphRace : public Object
{
public:
	enum{ ID_RUN=1001,ID_RUN2,
		ID_HIT1=2001, ID_HIT2, ID_HIT3, ID_HIT4, ID_HIT5
		, ID_HIT6, ID_HIT7, ID_HIT8,
		ID_BEHIT1, ID_BEHIT2, ID_BEHIT3, ID_BEHIT4,
		ID_FALLDOWN1, ID_FALLDOWN2, ID_FALLDOWN3,
		ID_GETUP1,ID_GETUP2
	};

	GraphData *pgd;//��� ��ֵΪ0˵��δ��ʼ����
	vector<Graph *> vpg;//����GraphData��Graph���������
	vector<int> vIZero;//vpg��Ϊ0��Ԫ�ص�λ�õĶ���
	char *fileName;// ���ڶ�ȡģ���ļ��ͱ��ģ������
	string strFileName;

	GraphRace();
	~GraphRace();

	void setFileName(char *fileName_){
		fileName=fileName_;
		strFileName=string(fileName_);
	}

	void setMovements();
	void addMovement(int moveID,char *pfile, int infoLen, int *info);

	void add(Graph *);
	int remove(Graph *);
	void clear();
protected:
private:
};
