#include"Integer.h"
#include<iostream>

#define BI_INT (0b11111111111111111111111111111111)
#define MAX_INT_EPOW	10

using namespace std;


int getE2(int i)
{
	int res = 1;
	for (int j = 1; j <= i; j++)
	{
		res *= 10;
	}
	return res;
}

int getBits(int i)
{
	i = abs(i);
	if (i > 99999)
	{
		if (i > 9999999)
		{
			if (i > 99999999)
			{
				if (i > 999999999)
					return 10;
				else
					return 9;
			}
			else
			{
				return 8;
			}
		}
		else
		{
			if (i > 999999)
			{
				return 7;
			}
			else
			{
				return 6;
			}
		}
	}
	else
	{
		if (i > 999)
		{
			if (i > 9999)
				return 5;
			else
				return 4;
		}
		else
		{

			if (i > 99)
				return 3;
			else
			{
				if (i > 9)
					return 2;
				else
					return 1;
			}
		}
	}
	/*
	if (i / 100000 == 0)
	{
		if (i / 1000 == 0)
		{
			if (i / 100 == 0)
			{
				if (i / 10 == 0)
					return 1;
				else
					return 2;
			}
			else
				return 3;
		}
		else if (i / 10000 == 0)
			return 4;
		else
			return 5;
	}
	else if (i / 10000000 == 0)
	{
		if (i / 1000000 == 0)
			return 6;
		else
			return 7;
	}
	else
	{
		if (i / 100000000 == 0)
			return 8;
		else if (i / 1000000000 == 0)
			return 9;
		else
			return 10;
	}*/
}

int getBits(unsigned int i)
{
	if (i > 99999)
	{
		if (i > 9999999)
		{
			if (i > 99999999)
			{
				if (i > 999999999)
					return 10;
				else
					return 9;
			}
			else
			{
				return 8;
			}
		}
		else
		{
			if (i > 999999)
			{
				return 7;
			}
			else
			{
				return 6;
			}
		}
	}
	else
	{
		if (i > 999)
		{
			if (i > 9999)
				return 5;
			else
				return 4;
		}
		else
		{

			if (i > 99)
				return 3;
			else
			{
				if (i > 9)
					return 2;
				else
					return 1;
			}
		}
	}
}


bool getanArithmeticOperator(char*ch)
{
	char get;
	cin >> get;

	//Ìø¹ı¿Õ°××Ö·û
	while (get == ' ' || get == '\t' || get == '\r' || get == '\n')
		cin >> get;

	if (get == '-' || get == '+' || get == '*' || get == '/' || get == '%'
		|| get == '^' || get == '<' || get == '>' || get == '!' || get == '$')
	{
		*ch = get;
	}
	else if (get == 'c' || get == 'C')
	{
		*ch = 'c';
	}
	else if (get == 'g')
	{
		cin >> get;
		if (get != 'c')
			return false;

		cin >> get;
		if (get != 'd')
			return false;

		*ch = 'g';
	}
	else if (get == 'l')
	{
		cin >> get;
		if (get != 'c')
			return false;

		cin >> get;
		if (get != 'm')
			return false;

		*ch = 'l';
	}
	else if (get == 'p' || get == 'P')
	{
		*ch = 'p';
	}
	else
		return false;

	return true;
}