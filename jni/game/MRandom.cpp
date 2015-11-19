#include "MRandom.h"

#include <ctime>
#include <cstdlib>

double getRandom(int seed,double start, double end)
{
	srand(seed);
	return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

double getRandom(double start, double end)
{
	srand(time(0));
	return start+(end-start)*rand()/(RAND_MAX + 1.0);
}

