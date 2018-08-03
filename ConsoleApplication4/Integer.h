#pragma once

inline int getE(int i)
{
	int res = 1;
	for (int j = 1; j <= i; j++)
	{
		res *= 10;
	}
	return res;
}
int getE2(int i);
int getBits(int);
int getBits(unsigned int);

bool getanArithmeticOperator(char*);