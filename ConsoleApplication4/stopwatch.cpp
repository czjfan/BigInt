#include"stopwatch.h"

#include<iostream>
#include<iomanip>//����time.h


using namespace std;

stopwatch::stopwatch()
{
	period = 0;
	start = -1;
}

void stopwatch::go()//��ʼ��ʱ
{
	start = clock();
}

long stopwatch::newone()//���¿�ʼ��ʱ
{
	if (start == -1)
		return 0;

	end = clock();
	period = end - start;

	start = end;
	return period;
}

long stopwatch::newone_show()
{
	if (start == -1)
		return 0;

	end = clock();
	period = end - start;

	start = end;

	stdshow();
	return period;
}

long stopwatch::cut()
{
	if (start == -1)
		return 0;

	end = clock();
	period = end - start;
	return period;
}

long stopwatch::cut_show()
{
	if (start == -1)
		return 0;

	end = clock();
	period = end - start;

	stdshow();
	return period;
}


long stopwatch::cut_show(ofstream &fout)
{
	if (start == -1)
		return 0;

	end = clock();
	period = end - start;

	double PERIOD = (double)period / CLOCKS_PER_SEC;

	fout << setprecision(6) << PERIOD << "(sec)";
	file_show(fout);

	return period;
}


bool stopwatch::stdshow()
{
	if (start == -1)
		return false;

	double PERIOD = (double)period / CLOCKS_PER_SEC;

	cout << setprecision(6) << PERIOD << "(sec)";
	return true;
}

bool stopwatch::file_show(ofstream &fout)
{
	if (start == -1)
		return false;

	double PERIOD = (double)period / CLOCKS_PER_SEC;

	fout << setprecision(6) << PERIOD << "(sec)";
	return true;
}

/*void stopwatch::Go()
{
	start = clock();
}*/