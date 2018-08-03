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
	in.open(".\\primeI32.txt", ios::_Nocreate | ios::binary);//��ȡ�ļ�
	if (!in.is_open())
	{
		cerr << "read prime file failed!" << endl;
		return 0;
	}
	in >> noskipws;
	in.seekg(0, ios::end);
	streamoff nFileSize = in.tellg();
	in.seekg(0, ios::beg);

	//�洢�б����
	nListLenU32 = PRIMECOUNT_IN_UINT32;
	SAFE_DELETELIST(primeU32);
	primeU32 = new UINT32[nListLenU32 + 1];
	if (primeU32 == NULL)//����ʧ��
	{
		stateU32 = PLS_EMPTY;
		nListLenU32 = 0;
		nPrimesU32 = 0;
		return 0;
	}
	stateU32 = PLS_LOADING;
	nPrimesU32 = 0;
	primeU32[++nPrimesU32] = 2;	//����2���⴦��Ψһһ��ż������

	UINT32 testOdd = 3;		//��������
	byte cycle = 0;			//���ݽ�������

	//�Դ������ۼ��������ֵĵ�һ���ֽڴ���
	in >> cycle;
	for (int i = 0; i <= 7; i++, cycle >>= 1)
	{
		if (cycle & 1)//������
		{
			primeU32[++nPrimesU32] = testOdd;

			if (nPrimesU32 >= nListLenU32)//�ﵽ�洢����
				break;
		}

		//������������
		testOdd += 2;
		if (testOdd % 10 == 5 && testOdd != 5)
			testOdd += 2;
	}

	//ʣ���ֽڴ���
	int mark = 0;
	while (true)
	{
		//��ȡһ������
		in >> cycle;

		//���ļ�β���˳�ѭ��
		if (in.eof())
			break;

		//����һ������(һ���ֽڰ���8������������Ϣ)
		for (int i = 0; i <= 7; i++, cycle >>= 1)
		{
			if (cycle & 1)//������
			{
				primeU32[++nPrimesU32] = testOdd;

				if (nPrimesU32 >= nListLenU32)//�ﵽ�洢����
					break;
			}

			//������������
			testOdd += 2;
			if (testOdd % 10 == 5)
				testOdd += 2;
		}

		//�ﵽ�洢���ޣ��˳�ѭ��
		if (nPrimesU32 >= nListLenU32)
		{
			if ((streamoff)in.tellg() < nFileSize)
			{
				cout << "prime list is full" << endl;//TODO:�����ж�����
			}
			break;
		}

		//��Ϣ��ʾ
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

	//�洢�б����
	if (primeU32)
		delete[] primeU32;
	nListLenU32 = PRIMECOUNT_IN_UINT32;	//�洢�б���
	primeU32 = new UINT32[nListLenU32 + 1];
	if (primeU32 == NULL)//����ʧ��
	{
		stateU32 = PLS_EMPTY;
		nListLenU32 = 0;
		nPrimesU32 = 0;
		return 0;
	}
	stateU32 = PLS_BUILDING;
	nPrimesU32 = 0;

	UINT32 prime = 3;				//��������
	UINT32 lastprime = 2;			//��һ������������(�����жϴ����������)
	UINT32 test = 1;				//���������±�
	UINT32 testedprime = 2;			//���ڲ��Ե�����
	UINT32 testrange = UINT_MAX;	//���Է�Χ����������

	ifstream in;
	in.open("E:\\primeintcontinue.txt", ios::_Nocreate);//�����ϴ�
	if (in.is_open())
	{
		//in.seekg(-32, ios::end);
	}
	else
	{
		primeU32[++nPrimesU32] = 2;//����2���⴦��Ψһһ��ż������
		//FileOut(out, 2);

		//Ϊ����testedprime <= sqrtprime�ж�ǰ���£���ֹȡ��һ���ڲ��Ե����������б�Χ
		//�б�ռ�>=2��Ԥ��2,3
		//�б�ռ�>=3��Ԥ��2,3,5
		//��������nPrimesU32�������ڲ���
		//if (nListLenU32 >= 2)
		//	primeU32[2] = 3;
		//if (nListLenU32 >= 3)
		//	primeU32[3] = 5;
	}
	//��ʱ�б��Ѵ�һ������2��nPrimesU32=1

	//ѭ������
	/*
	����������ѭ��������
	1.testrange���Է�Χ
	2.nListLenU32���ȵ��б�洢���
	3.��������prime�����������
	*/
	byte cycle = 0;
	byte cycledata = 0;
	byte databuf[512] = { 0 };
	UINT16 buflen = 0;
	prime = 3;
	time_t start = GetTickCount(), end = 0;//��ʱ

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

	while (prime <= testrange)//�ڲ��Է�Χ�ڲ���
	{
		//�ﵽ�洢���ޣ�[����ѭ����ʼ��Ϊ����ӦnListLenU32=1�����]
		if (nPrimesU32 >= nListLenU32)
			break;

		for (cycle = 0; cycle <= 7; cycle++)
		{
			//�����Ƿ�������
			bool isprime = true;
			UINT32 sqrtprime = (UINT32)(sqrt((float)prime) + 1);//�����ƽ����֤����prime�����ڲ��Ե�����С����ʱѭ������
			test = 1;//�ӵ�һ��������ʼ����
			testedprime = primeU32[test];
			for (; testedprime < sqrtprime/*&& test <= nPrimesU32*/;)
			{
				if (prime%testedprime == 0)//��������
				{
					isprime = false;
					break;
				}
				test++;

				testedprime = primeU32[test];
			}

			//�����������õ�ǰλ
			if (isprime)
			{
				//�洢����
				primeU32[++nPrimesU32] = prime;

				//���һ������
				//FileOut(out, prime);
				cycledata |= (byte)(1 << cycle);

				//��Ϣ��ʾ
				if (nPrimesU32 % 500000 == 0)
				{
					end = GetTickCount();
					cout << nPrimesU32 << ":" << prime << " rate:" << nPrimesU32 / (float)(end - start)
						<< "\tprimepercent:" << primeU32[nPrimesU32] / (float)nPrimesU32 << endl;
				}

				//�������ֺ󣬴ﵽ�洢����
				if (nPrimesU32 >= nListLenU32)
					break;
			}

			//������������
			lastprime = prime;
			prime += 2;
			if (prime % 10 == 5/* && prime != 5*/)
				prime += 2;
			//������������������������
			if (lastprime >= prime)
				break;
		}

		//����������棨���һ�ֽ�Ϊ���������
		if (!((lastprime >= prime || nPrimesU32 >= nListLenU32) && cycledata == 0))
		{
			databuf[buflen++] = cycledata;

			//���һ�����ݣ���������ת����
			cycle = 0;
			cycledata = 0;
		}

		if (buflen >= 512)
		{
			out.write((char*)databuf, buflen);
			buflen = 0;
		}

		//�ﵽ�洢����
		if (nPrimesU32 >= nListLenU32)
			break;
		//������Խ�磨��������Խ�磩
		if (lastprime >= prime)
			break;
	}
	//���ʣ�໺��������
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
	//���ݴ洢���б����
	/*if (primeU32)
	{
		for (UINT32 i = 1; i <= nPrimesU32; i++)
		{
			if (primeU32[i] == number)
				return true;
		}
	}*/

	/*if (number != 0 && (number & 1) == 0)//�Ƿ�0ż��
		return false;*/
	if (number == 0 || number == 1)
		return false;

	UINT32 primefactor = 2;
	int primeindex = 1;
	for (primefactor = 2; getBits(primefactor) < (getBits(number) + 1) / 2 + 1 && primefactor < number;)//TODO &&��ڶ��ж��Ƿ���Ҫ
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
//	if (primeI64)//���ݴ洢���������
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
