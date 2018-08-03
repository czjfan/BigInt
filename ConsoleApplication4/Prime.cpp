#include "Prime.h"

PRIMELIST_STATE Prime::stateU32 = PLS_EMPTY;
int Prime::nListLenU32 = 1;
UINT32* Prime::primeU32 = new UINT32[nListLenU32 + 1];
int Prime::nPrimesU32 = 0;

Prime::Prime()
{
	primeU32 = NULL;
}

void Prime::ShowPrimeListU32(UINT32 delay)
{
	if (!primeU32)
		return;
	if (nPrimesU32 >= 1)
	{
		cout << "max:" << primeU32[nPrimesU32] << " (" << nPrimesU32 << "th)" << endl;
		Sleep(1000);
	}
	for (UINT32 i = nPrimesU32; i >=1; i--)
	{
		cout << primeU32[i] << ',' << i << endl;
		Sleep(delay);
	}
}

int Prime::LoadPrimeU32()
{
	LARGE_INTEGER start = { 0 };
	LARGE_INTEGER end = { 0 };
	LARGE_INTEGER freq = { 1 };
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&start);

	ifstream in;
	in.open(".\\primeI32.txt", ios::_Nocreate | ios::binary);//读取文件
	if (!in.is_open())
	{
		cerr << "read prime file failed!" << endl;
		return 0;
	}
	in >> noskipws;
	in.seekg(0, ios::end);
	streamoff nFileSize = in.tellg();
	in.seekg(0, ios::beg);

	//存储列表更新
	nListLenU32 = PRIMECOUNT_IN_UINT32;
	SAFE_DELETELIST(primeU32);
	primeU32 = new UINT32[nListLenU32 + 1];
	if (primeU32 == NULL)//创建失败
	{
		stateU32 = PLS_EMPTY;
		nListLenU32 = 0;
		nPrimesU32 = 0;
		return 0;
	}
	stateU32 = PLS_LOADING;
	nPrimesU32 = 0;
	primeU32[++nPrimesU32] = 2;	//质数2特殊处理，唯一一个偶数质数

	UINT32 testOdd = 3;		//待测奇数
	byte cycle = 0;			//数据解析辅助

	//以待测数累加条件区分的第一个字节处理
	in >> cycle;
	for (int i = 0; i <= 7; i++, cycle >>= 1)
	{
		if (cycle & 1)//是质数
		{
			primeU32[++nPrimesU32] = testOdd;

			if (nPrimesU32 >= nListLenU32)//达到存储上限
				break;
		}

		//待测质数递增
		testOdd += 2;
		if (testOdd % 10 == 5 && testOdd != 5)
			testOdd += 2;
	}

	//剩余字节处理
	int mark = 0;
	while (true)
	{
		//读取一组数据
		in >> cycle;

		//到文件尾，退出循环
		if (in.eof())
			break;

		//解析一组数据(一个字节包含8个待测奇数信息)
		for (int i = 0; i <= 7; i++, cycle >>= 1)
		{
			if (cycle & 1)//是质数
			{
				primeU32[++nPrimesU32] = testOdd;

				if (nPrimesU32 >= nListLenU32)//达到存储上限
					break;
			}

			//待测质数递增
			testOdd += 2;
			if (testOdd % 10 == 5)
				testOdd += 2;
		}

		//达到存储上限，退出循环
		if (nPrimesU32 >= nListLenU32)
		{
			if ((streamoff)in.tellg() < nFileSize)
			{
				cout << "prime list is full" << endl;//TODO:增加判断条件
			}
			break;
		}

		//信息显示
		if (nPrimesU32 - mark >= 5000000)
		{
			mark = nPrimesU32;
			cout << "\r read primes:" << nPrimesU32
				<< "\tprime is:" << primeU32[nPrimesU32];
			cout.flush();
		}
	}
	cout << "\r total read primes:" << nPrimesU32
		<< "\tlast:" << primeU32[nPrimesU32] << endl;

	QueryPerformanceCounter(&end); 
	float period = ((end.QuadPart - start.QuadPart) / (float)freq.QuadPart);
	cout << "\ntime: ";
	OutputTime(period);
	cout << endl;

	stateU32 = PLS_LOADED;
	return nPrimesU32;
}

int Prime::BuildU32()
{
	ofstream out;
	out.open("E:\\primeI32.txt", ios::binary);
	if (!out.is_open())
		return -1;

	//存储列表更新
	if (primeU32)
		delete[] primeU32;
	nListLenU32 = PRIMECOUNT_IN_UINT32;	//存储列表长度
	primeU32 = new UINT32[nListLenU32 + 1];
	if (primeU32 == NULL)//创建失败
	{
		stateU32 = PLS_EMPTY;
		nListLenU32 = 0;
		nPrimesU32 = 0;
		return 0;
	}
	stateU32 = PLS_BUILDING;
	nPrimesU32 = 0;

	UINT32 prime = 3;				//待测质数
	UINT32 lastprime = 2;			//上一个待测质数，(用于判断待测质数溢出)
	UINT32 test = 1;				//测试数组下标
	UINT32 testedprime = 2;			//用于测试的质数
	UINT32 testrange = UINT_MAX;	//测试范围，最大待测数

	ifstream in;
	in.open("E:\\primeintcontinue.txt", ios::_Nocreate);//继续上次
	if (in.is_open())
	{
		//in.seekg(-32, ios::end);
	}
	else
	{
		primeU32[++nPrimesU32] = 2;//质数2特殊处理，唯一一个偶数质数
		//FileOut(out, 2);

		//为了在testedprime <= sqrtprime判断前提下，防止取下一用于测试的质数超出列表范围
		//列表空间>=2则预存2,3
		//列表空间>=3则预存2,3,5
		//但不增加nPrimesU32，仅用于测试
		//if (nListLenU32 >= 2)
		//	primeU32[2] = 3;
		//if (nListLenU32 >= 3)
		//	primeU32[3] = 5;
	}
	//此时列表已存一个质数2，nPrimesU32=1

	//循环测试
	/*
	【三个结束循环条件】
	1.testrange测试范围
	2.nListLenU32长度的列表存储溢出
	3.待测质数prime数据类型溢出
	*/
	byte cycle = 0;
	byte cycledata = 0;
	byte databuf[512] = { 0 };
	UINT16 buflen = 0;
	prime = 3;
	time_t start = GetTickCount(), end = 0;//计时

	databuf[0] = 247;
	out.write((char*)databuf, 1);
	prime = 21;
	primeU32[2] = 3;
	primeU32[3] = 5;
	primeU32[4] = 7;
	primeU32[5] = 11;
	primeU32[6] = 13;
	primeU32[7] = 17;
	primeU32[8] = 19;
	nPrimesU32 += 7;

	while (prime <= testrange)//在测试范围内测试
	{
		//达到存储上限，[放在循环开始，为了适应nListLenU32=1的情况]
		if (nPrimesU32 >= nListLenU32)
			break;

		for (cycle = 0; cycle <= 7; cycle++)
		{
			//测试是否是质数
			bool isprime = true;
			UINT32 sqrtprime = (UINT32)(sqrt((float)prime) + 1);//这个数平方保证大于prime，用于测试的质数小于它时循环测试
			test = 1;//从第一个质数开始测试
			testedprime = primeU32[test];
			for (; testedprime < sqrtprime/*&& test <= nPrimesU32*/;)
			{
				if (prime%testedprime == 0)//不是质数
				{
					isprime = false;
					break;
				}
				test++;

				testedprime = primeU32[test];
			}

			//是质数，设置当前位
			if (isprime)
			{
				//存储质数
				primeU32[++nPrimesU32] = prime;

				//输出一组数据
				//FileOut(out, prime);
				cycledata |= (byte)(1 << cycle);

				//信息显示
				if (nPrimesU32 % 500000 == 0)
				{
					end = GetTickCount();
					cout << nPrimesU32 << ":" << prime << " rate:" << nPrimesU32 / (float)(end - start)
						<< "\tprimepercent:" << primeU32[nPrimesU32] / (float)nPrimesU32 << endl;
				}

				//新增数字后，达到存储上限
				if (nPrimesU32 >= nListLenU32)
					break;
			}

			//待测质数递增
			lastprime = prime;
			prime += 2;
			if (prime % 10 == 5/* && prime != 5*/)
				prime += 2;
			//待测数溢出（数据类型溢出）
			if (lastprime >= prime)
				break;
		}

		//存入输出缓存（最后一字节为空则不输出）
		if (!((lastprime >= prime || nPrimesU32 >= nListLenU32) && cycledata == 0))
		{
			databuf[buflen++] = cycledata;

			//完成一组数据，重置数据转换器
			cycle = 0;
			cycledata = 0;
		}

		if (buflen >= 512)
		{
			out.write((char*)databuf, buflen);
			buflen = 0;
		}

		//达到存储上限
		if (nPrimesU32 >= nListLenU32)
			break;
		//待测数越界（数据类型越界）
		if (lastprime >= prime)
			break;
	}
	//输出剩余缓冲区内容
	if (buflen > 0)
		out.write((char*)databuf, buflen);

	end = GetTickCount();
	time_t totalsec = (end - start) / 1000;
	cout << "total build:" << nPrimesU32 << "primes\tlast:" << primeU32[nPrimesU32]
		<< endl
		<< " time:"
		<< totalsec / 60 << "m"
		<< totalsec % 60 << "s"
		<< endl;

	stateU32 = PLS_BUILT;
	return nPrimesU32;
}

int Prime::GetCount()
{
	return nPrimesU32;
}

UINT32 Prime::GetNum(int index)
{
	if (index <= nPrimesU32)
		return primeU32[index];
	else
		return 2;
}

bool Prime::isPrime(UINT32 number)
{
	//根据存储的列表查找
	/*if (primeU32)
	{
		for (UINT32 i = 1; i <= nPrimesU32; i++)
		{
			if (primeU32[i] == number)
				return true;
		}
	}*/

	/*if (number != 0 && (number & 1) == 0)//是非0偶数
		return false;*/
	if (number == 0 || number == 1)
		return false;

	UINT32 primefactor = 2;
	int primeindex = 1;
	for (primefactor = 2; getBits(primefactor) < (getBits(number) + 1) / 2 + 1 && primefactor < number;)//TODO &&后第二判断是否需要
	{
		if (primeindex <= Prime::GetCount())
			primefactor = Prime::GetNum(primeindex);

		if (number % primefactor == 0)
			return false;

		primeindex++;
		primefactor += 2;
		if (primefactor % 10 == 5 && primefactor != 5)
			primefactor += 2;
	}
	return true;
}

//int Prime::GetFactor(BigInt num)
//{
//	if (primeI64)//根据存储的数组查找
//	{
//		for (int i = 1; i <= nPrimesU32 && num <= (int)(primeI64[i] * primeI64[i]); i++)
//		{
//			if (num%(int)primeI64[i] == 0)
//				return primeI64[i];
//		}
//	}
//
//	int prime = primeI64[nPrimesU32];
//	for (; getBits(prime) <= num.GetBits() / 2 + 1 && prime < num &&prime <= MAX_INT; prime += 2)
//	{
//		if (num % prime == 0)
//			return prime;
//	}
//	return prime;
//}
