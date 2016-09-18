#include "Random.h"

//Generate a random float
float Random::RandomFloat()
{
	seed ^= (seed << 5);
	seed ^= (seed >> 13);
	seed ^= (seed << 6);

	return seed * (1.0f / 4294967295.0f);
}

//Generate random number between min and max
int Random::RandomInt(int min, int max)
{
	seed ^= (seed << 5);
	seed ^= (seed >> 13);
	seed ^= (seed << 6);
	float rnd =  seed * (1.0f / 4294967295.0f);

	return rnd * (float)(max - min) + min;
}

void Random::SetSeed(unsigned int seed)
{
	this->seed = seed;
}