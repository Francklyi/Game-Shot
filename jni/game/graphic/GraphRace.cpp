#include "GraphRace.h"

GraphRace::GraphRace():fileName(0),pgd(0)
{
	vpg=vector<Graph*>(0);
	vIZero=vector<int>(0);
}

GraphRace::~GraphRace()
{
	if(pgd) free(pgd);
	pgd=0;
	for(int i=0; i<vpg.size(); i++)
	{
		if(vpg[i])
			free(vpg[i]);
	}
	vpg.clear();
}

void GraphRace::setMovements()
{
	if (!pgd) return;
	//char aFile_run[]="run_test6_1.amn\0";//"walkman6.amn\0";

	char aFile1[]="enemy_run2_1.amn\0";//"jump_forward1_1.amn\0";//"jump_test5_1.amn\0";//"walkManNew1.amn\0";
	char aFile2[]="hit_hand_leg1.amn\0";//"hit_test4.amn\0";//"jump_hide2.amn\0";//"jump_test5_2.amn\0";
	char aFile3[]="behitted5_1.amn\0";
	char aFile4[]="hit_hand5_1.amn\0";
	char aFile5[]="falldownup1.amn\0";//"falldown1_1.amn\0";

	char aFile6[]="getup1_1.amn\0";

	//addMovement( ID_RUN, aFile_run);

	addMovement( ID_RUN2, aFile1,0, 0);


	int infoLen=1;
	int info0[]={
			45,Frame::TYPE_HIT1,
	};

	addMovement( ID_HIT2, aFile2, infoLen, info0);

	addMovement( ID_BEHIT1, aFile3,0, 0);


	infoLen=4;
	int info[]={
			9,Frame::TYPE_HIT1,
			24,Frame::TYPE_HIT1,
			38,Frame::TYPE_HIT1,
			53,Frame::TYPE_HIT1
	};

	addMovement( ID_HIT4, aFile4,infoLen, info);

	addMovement( ID_FALLDOWN1, aFile5, 0, 0);

	//addMovement( ID_GETUP1, aFile6);
}

void GraphRace::addMovement(int moveID,char *pfile,int infoLen, int *info)
{

	Movement *pM=new Movement(0,moveID,0,pfile,0);
	pgd->addMovement(pM);
	pM->HP=2;
	for(int i=0; i<infoLen;i++)
	{
		pM->getAnimation()
						  ->pFrames[info[2*i]].setInfo(info[2*i], info[2*i+1]);
	}

}

void GraphRace::add(Graph *pG){
	if(!pG) return;
	pG->setGraphRace(this);
	if (!pgd)
	{
		pG->load();

		pgd=pG->gp;
		setFileName(pG->fileName);

		pG->setOrder(vpg.size());
		vpg.push_back(pG);
	}
	else
	{
		pG->gp=pgd;

		if (vIZero.size()>0)
		{
			pG->setOrder(vIZero.back());
			vpg[vIZero.back()]=pG;
			vIZero.pop_back();
		}else
		{
			pG->setOrder(vpg.size());
			vpg.push_back(pG);
		}	
	}
}

int GraphRace::remove(Graph *pG){
	if(!pG) return 0;
	if (vpg.size()>0)
	{
		vIZero.push_back(pG->order);
		pG->state=2;
		//vpg[pG->order]=0;
	}
	if (vIZero.size()==vpg.size())
	{
		return -1;
	}
	return vpg.size();
}

void GraphRace::clear(){
	if (vpg.size()>0)
	{
		
	}
	vpg.clear();
}
