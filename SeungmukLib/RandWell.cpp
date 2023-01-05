#include "RandWell.h"
#include "pch.h"

void Init_WELL_RAND()
{
	for (int i = 0; i < NUM_MAX_THREAD; i++)
	{
		state_i[i] = 0;

		for (int j = 0; j < R; j++)
		{
			unsigned int curTime = timeGetTime();
			STATE[i][j] = curTime + ((curTime + (unsigned int)rand()) % ((unsigned int)rand() + 1));
		}
	}
}

double RandWELL(int N)
{
	static unsigned int z0, z1, z2;

	z0 = VRm1(N);
	z1 = Identity(V0(N)) ^ MAT0POS(8, VM1(N));
	z2 = MAT0NEG(-19, VM2(N)) ^ MAT0NEG(-14, VM3(N));

	newV1(N) = z1 ^ z2;
	newV0(N) = MAT0NEG(-11, z0) ^ MAT0NEG(-7, z1) ^ MAT0NEG(-13, z2);

	state_i[N] = (state_i[N] + 31) & 0x0000001fU;

	return (double)STATE[N][state_i[N]] * FACT;
}

int randbtn(int minValue, int maxValue)
{
	double wellrand = RandWELL(0);
	return (int)(wellrand * ((double)(maxValue - minValue) + 0.99)) + minValue;
}

int randbtn(int T, int minValue, int maxValue)
{
	double wellrand = RandWELL(T);
	return (int)(wellrand * ((double)(maxValue - minValue) + 0.99)) + minValue;
}

int isEven()
{
	int randomValue = randbtn(0, 1);
	return randomValue == 1 ? 1 : -1;
}


double randbtn(double min, double max)
{
	double res = RandWELL(0) * (max - min) + min;

	if (res > max)	res = max;
	if (res < min)	res = min;

	return res;
}

double GaussianRandom(int num_thr, double mean, double stdev)
{
	double v1, v2, s, temp;

	do
	{
		v1 = 2.0 * RandWELL(num_thr) - 1.0;      // -1.0 ~ 1.0
		v2 = 2.0 * RandWELL(num_thr) - 1.0;      // -1.0 ~ 1.0 
		s = v1 * v1 + v2 * v2;
	} while (s >= 1 || s == 0);

	s = sqrt((-2.0 * log(s)) / s);

	temp = v1 * s;
	temp = (stdev * temp) + mean;

	return temp;
}

double GaussianFunc2D_Std(double X, double Y, double sigma)
{
	return 0.5 * exp(-0.5 * (X * X + Y * Y) / (sigma * sigma)) / (3.141592 * sigma * sigma);
}

double GaussianFunc2D_One(double X, double Y, double sigma)
{
	return exp(-0.5 * (X * X + Y * Y) / (sigma * sigma));
}