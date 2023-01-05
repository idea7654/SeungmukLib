#pragma once

#include <windows.h>
#include <math.h>

#define NUM_MAX_THREAD	24

#define W 32
#define R 32
#define M1 3
#define M2 24
#define M3 10

#define MAT0POS(t,v)	(v^(v>>t))
#define MAT0NEG(t,v)	(v^(v<<(-(t))))
#define Identity(v)		(v)

#define V0(n)		STATE[n][ state_i[n]                  ]
#define VM1(n)		STATE[n][(state_i[n]+M1) & 0x0000001fU]
#define VM2(n)		STATE[n][(state_i[n]+M2) & 0x0000001fU]
#define VM3(n)		STATE[n][(state_i[n]+M3) & 0x0000001fU]
#define VRm1(n)		STATE[n][(state_i[n]+31) & 0x0000001fU]

#define newV0(n)	STATE[n][(state_i[n]+31) & 0x0000001fU]
#define newV1(n)	STATE[n][ state_i[n]                  ]

#define FACT 2.32830643653869628906e-10


static unsigned int state_i[NUM_MAX_THREAD];
static unsigned int STATE[NUM_MAX_THREAD][R];


// main 시작할때 한번 불러줘라..
void Init_WELL_RAND();
// Core 부분 --> 0~1 사이 소수 리턴
double RandWELL(int N);

int randbtn(int minValue, int maxValue);

int randbtn(int T, int minValue, int maxValue);
int isEven();

double randbtn(double min, double max);


///// Gaussian Random ///////////////////////////////////////////////////////
double GaussianRandom(int num_thr, double mean, double stdev);

double GaussianFunc2D_Std(double X, double Y, double sigma);

double GaussianFunc2D_One(double X, double Y, double sigma);