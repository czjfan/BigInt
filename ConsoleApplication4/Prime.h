#pragma once

#include <fstream>
#include <iostream>
#include <math.h>

#include "Integer.h"
#include "BigInt.h"

#define MAX_UINT64				18446744073709551615
#define MAX_UINT32				4294967295
#define MAX_UINT8				255
#define MAX_INT					2147483647
#define PRIMECOUNT_IN_UINT32	203280221
#define PRIMECOUNT_IN_UINT8		40
#define PRIMECOUNT_IN_INT		105097565
#define MAX_INTSQRT				46341
#define I32PRIMECOUNT			110000000
#define PRIMESTORE				PRIMECOUNT_IN_INT

using namespace std;

enum PRIMELIST_STATE {
	PLS_EMPTY
	, PLS_LOADING
	, PLS_LOADED
	, PLS_BUILDING
	, PLS_BUILT
};

class Prime {
private:
	static PRIMELIST_STATE stateU32;
	static int nListLenU32;
	static unsigned int *primeU32;//质数数组
	static int nPrimesU32;//计数

public:
	Prime();

	static void ShowPrimeListU32(UINT32 delay);
	static int LoadPrimeU32();
	static int BuildU32();
	static int GetCount();
	static UINT32 GetNum(int);
	static bool isPrime(UINT32 num);
	//static int GetFactor(BigInt num);
};