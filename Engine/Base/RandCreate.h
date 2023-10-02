#pragma once
#include<stdio.h>
#include<random>

class RandCreate
{
private:
	
	
	using mt19937_64 = std::mt19937_64;
	using random_device = std::random_device;

public:
	void Ins()
	{
		mt.seed(rnd());
	}
	int getRandInt(int i, int j)
	{
		std::uniform_int_distribution<> intRand(i, j);
		return intRand(mt);
	}
	double getRandDouble(double i, double j)
	{
		std::uniform_real_distribution<> floatRand(i, j);
		return floatRand(mt);
	}

	float getRandFloat(float i, float j)
	{
		std::uniform_real_distribution<> floatRand(i, j);
		return (float)floatRand(mt);
	}

	static int sGetRandInt(int i, int j)
	{
		mt19937_64 mt;
		random_device rnd;
		mt.seed(rnd());
		std::uniform_int_distribution<> intRand(i, j);
		return intRand(mt);
	}
	static double sGetRandFloat(double i, double j)
	{
		mt19937_64 mt;
		random_device rnd;
		mt.seed(rnd());
		mt.seed(rnd());
		std::uniform_real_distribution<> floatRand(i, j);
		return floatRand(mt);
	}
private:

	mt19937_64 mt;
	random_device rnd;
};