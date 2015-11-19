#pragma once

class TrigFuns
{
public:

	float *pfCos;
	float *pfSin;

	TrigFuns();



	float getCos(int degree);

	float getSin(int degree);

};

extern TrigFuns *pTrigFuns;
