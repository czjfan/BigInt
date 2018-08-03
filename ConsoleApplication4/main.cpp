#include <iostream>
#include <windows.h> 
#include <stdlib.h>  
#include "BigInt.h"
#include "Prime.h"
#include "char.h"

using namespace std;

void TypeDesc()
{
	char c;
	cout << "char:\t\tbytes:" << sizeof(char) 
		<< "\t\tmin:" << "-128" << '(' << (c |= INT_MAX) << ')' << endl;
}

bool GetCMD(char *cmd)
{
	char getch = 0;
	cin >> getch;

	while (invalidchar(getch) || spacechar(getch))//�����Ƿ��ַ�
	{
		cin >> getch;
	}

	if (numberhead(getch))//�����ֲ���
	{
		cin.putback(getch);
		cmd[0] = '\0';
		return false;
	}

	int i = 0;
	while (identifierchar(getch) && i < MAX_CMDLEN)
	{
		cmd[i] = getch;
		cin >> getch;
		i++;
	}
	cin.putback(getch);
	cmd[i] = '\0';

	if (_stricmp(cmd, "ans") == 0)
	{
		cin.putback('s');
		cin.putback('n');
		cin.putback('a');
		return false;
	}
	else
		for (; getch != '\n'; cin >> getch);//���������ַ�

	return true;
}

void ExecuteCMD(char *cmd)
{
	if (!cmd)
	{
		cerr << "in ExecuteCMD: no cmd" << endl;
		return;
	}

	if (_stricmp(cmd, "scnt") == 0)
	{
		BigInt::set_scnt(true);
		cout << "in scientific notation.." << endl << endl;
	}
	else if (_stricmp(cmd, "reset scnt") == 0)
	{
		BigInt::set_scnt(false);
		cout << "reset scientific notation.." << endl << endl;
	}
	else if (_stricmp(cmd, "exit") == 0)
	{
		cout << "exit.." << endl << endl;
		exit(0);
	}
	else if (_stricmp(cmd, "typedesc") == 0)
	{
		TypeDesc();
	}
	else
	{
		cout << "unknown command" << endl;
	}
}

int main()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);//����̨���
	SetConsoleTitle(L"Calc");

	LARGE_INTEGER frequency = { 0 };
	LARGE_INTEGER pcStart = { 0 };
	LARGE_INTEGER pcEnd = { 0 };
	QueryPerformanceFrequency(&frequency);

	//���ڴ�С
	//system("mode con cols=80 lines=25");
	//��Ļ��������С
	COORD bufSize = { 80, 1024 };
	SetConsoleScreenBufferSize(hConsole, bufSize);
	/*SMALL_RECT rc = { 600, 100, 80 - 1, 25 - 1 };
	SetConsoleWindowInfo(hConsole, true, &rc);*/
	system("color f0");//TODO:�Ƿ��������Ӧ��ͳһ��ɫ��ÿ������̨�򿪶���

	//����
	char order[MAX_CMDLEN + 1];
	char getchar = '\0';

	cin >> resetiosflags(ios::skipws);

	BigInt bi(10);
	BigInt ai(2);
#define CYCLE_CAL
#ifdef CYCLE_CAL
	//����
	int period, period2;
	double total_old = 0, total_new = 0;
	//ѭ���ƽ�ͬһ����ƽ��ʱ���㷨
	for (int i = 1;; i++)
	{
		const int cycle = 16000;
		bi = 10;
		int s = clock();//��ʼ��ʱ
		for (int j = 0; j < cycle; j++)
		{
			BigInt::Mul(&bi, &bi, &ai);
		}
		int e = clock();

#define two_at_a_time
#ifdef two_at_a_time
		bi = 10;
		clock_t s2 = clock();//��ʼ��ʱ
		for (int j = 0; j < cycle; j++)
		{
			bi*=ai;
		}
		clock_t e2 = clock();

		period2 = e2 - s2;
		total_new += period2;
#endif
		period = e - s;
		total_old += period;

		cout << "\r" << "time_old:" <<setw(6)<< setprecision(6) << total_old / i << "    ";
		cout << '\t';
		cout << "time_new:" << setw(6) << setprecision(6) << total_new / i << "    ";
	}
#endif


	//�����ַ�����ʼ��
	/*char* ch = new char[12];
	strcpy(ch, "67666887");

	bi.CreateByCharArray_decimal(ch);
	bi.Display();*/

	BICalc calc;
	calc.SetHConsole(hConsole);
	//������
	SetConsoleTextAttribute(hConsole, 0x00F3);
	cout << "2017-08-16 version.." << endl << endl;
	SetConsoleTextAttribute(hConsole, 0x00F8);
	cout << "valid operator: +, -, *, /, ^, %, !, <, >, p, c, $" << endl;

	/*Prime::BuildU32();
	system("pause");
	return 0;*/
	Prime::LoadPrimeU32();
	//Prime::ShowPrimeListU32(1000);

	BigInt::set_step_display(false);//ȡ��������ʾ
	BigInt::set_ostream_comma(true);//���ö��ŷָ�
	while (true)
	{
		calc.Clear();
		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_GREY));
		cout << "input :" << endl;

		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_BLACK));
		bool bCMD = GetCMD(order);
		if (bCMD)//ִ������
		{
			SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_GREYD));
			if(order)
				ExecuteCMD(order);
			continue;
		}

		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_BLACK));

		// ���������
		calc.GetLOperand();
		QueryPerformanceCounter(&pcStart);

		if (!calc.GetOperator())
		{
			cerr << "fail to get operator" << endl << endl;

			cin >> getchar;
			for (; getchar != '\n'; cin >> getchar);//���������ַ�

			continue;
		}

		// ���Ҳ�����
		calc.GetROperand();

		cin >> getchar;
		for (; getchar != '\n'; cin >> getchar);//���������ַ�

		// �������
		calc.Echo();
	
		// ����
		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_GREYD));
		calc.Calc();

		// ������
		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_BLACK));
		calc.Output();

		/*QueryPerformanceCounter(&pcEnd);
		float period = (pcEnd.QuadPart - pcStart.QuadPart) / (float)frequency.QuadPart;
		OutputTime(period);
		cout << endl;*/
	}

	system("pause");
	return 0;
}