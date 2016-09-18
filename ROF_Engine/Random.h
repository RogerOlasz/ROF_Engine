#ifndef __RANDOM_H__
#define __RANDOM_H__

#include <stdint.h>

class Random
{
private:
	unsigned int seed;

public:
	float RandomFloat();
	int RandomInt(int min, int max);

	void SetSeed(unsigned int seed);

};
#endif // !__RANDOM_H__
