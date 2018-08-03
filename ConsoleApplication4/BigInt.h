#pragma once

#include "Integer.h"

#include <string>
#include <iostream>
#include <iomanip>
#include <windows.h>
#include <mutex>

#include "Prime.h"
#include "misc.h"
#include "char.h"

#define SAFE_DELETELIST(P)		{if (P) { delete[] (P); (P) = NULL; }}
#define CAPACITY_INIT			2

#define INDEX_BEGIN				1
#define WHOLE_INDEX_BEGIN		0

#define MAX_BLOCK				 999999999
#define INT_BASE				1000000000
#define BLOCK_BASE				 100000000
#define BLOCK_WIDTH				9
#define SQR_BLOCK_FLOOR			31622//����999999999(MAX_BLOCK)

#define MAX_2BLOCK				 999999999999999999
#define INT_2BASE				1000000000000000000

#define MAX_CONSTRUCT_INDEX		250000000

#define iszero					(index == 1 && num[1] == 0)

#define SPACES_IN_TAB			8

//display format
#define SPACES_FOR_SIGN			4
#define RIGHT_SPACES			1
#define CHARS_PER_ROW_CONSOLE	80
#define MAX_CMDLEN				32
#define NOSCNT_THRE				3

//console
#define CONSOLE_BACKCOLOR_OFF	4
#define CONSOLE_FRONTCOLOR_OFF	0
#define CONSOLE_BLACK			0x00
#define CONSOLE_BLUED			0x01
#define CONSOLE_GREEND			0x02
#define CONSOLE_LAKED			0x03
#define CONSOLE_REDD			0x04
#define CONSOLE_PURPLED			0x05
#define CONSOLE_YELLOWD			0x06
#define CONSOLE_GREY			0x07
#define CONSOLE_GREYD			0x08
#define CONSOLE_BLUE			0x09
#define CONSOLE_GREEN			0x0A
#define CONSOLE_LAKE			0x0B
#define CONSOLE_RED				0x0C
#define CONSOLE_PURPLE			0x0D
#define CONSOLE_YELLOW			0x0E
#define CONSOLE_WHITE			0x0F
#define CONSOLE_TEXTCOLOR(CB, CF)	(((CB)<<CONSOLE_BACKCOLOR_OFF) | ((CF)<<CONSOLE_FRONTCOLOR_OFF))
#define CTX_WX(CF)				(CONSOLE_TEXTCOLOR(CONSOLE_WHITE, (CF)))

using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::istream;
using std::ostream;

#define INIT_WITH_INDEX(I)	{index=(I);cap = index + 1;\
							SAFE_DELETELIST(num);\
							num = new int[cap];\
							num[0] = 0;}
#define INIT_WITH_PINDEX(P, I)	{if (P) {(P)->index=(I);(P)->cap = (P)->index + 1;\
							SAFE_DELETELIST((P)->num);\
							(P)->num = new int[(P)->cap];\
							(P)->num[0] = 0;}}

//TODO��������Ĵ���ŵ���̬�����У����Ա���������þ�̬����
//calc(BigInt *dest, BigInt *src, BigInt *obj)�죬����
//BigInt temp = *this, calc(BigInt *dest, BigInt *obj)��
//�����Ƽ�ʹ�þ�̬����
class BigInt{
private:
	//
	//number
	//
	int cap;
	int index;				//����
	int* num;				//��������

	bool positive;			//����λ������Ϊ0���Ǹ���Ϊ1

	CGrowableArray <BigInt> factors;	//����������

	char *arr;				//�ַ�����ʽ��ʾ

	//
	//param
	//
	static bool bCheck;					//�Ƿ������ں���ִ��ǰcheck
	static bool bStep;					//ִ�г�ʱ����㣬�Ƿ�����м���
	static unsigned int ostream_width;	//���BigInt���, =0��ʾ���涨���
	static bool bLeftAlign;				//���ʱ�Ƿ�������
	static bool bComma;					//���ʱ�Ƿ�ʹ�ö��ŷָ���ÿһ�ף�
	static bool bScnt;					//�Ƿ��ѧ���������

	//
	//function
	//
	inline void Extends(int ext = 1);
	inline void Cuts(int cut = 1);

public:
	//
	//destructor
	//
	~BigInt()
	{
		delete[] num;
	}

	//
	//constructor
	//
	template <typename T>BigInt(T);
	

	BigInt();
	BigInt(const char &);
	BigInt(const UINT8 &);
	BigInt(const short &);
	BigInt(const UINT16 &);
	BigInt(const int &);
	BigInt(const UINT32 &);
	BigInt(const long long &);
	BigInt(const UINT64 &);

	BigInt(bool unused, int INDEX);	//ָ����������BigInt
	BigInt(const char*);			//�ַ�����ʽ����

	//
	//copy constructor
	//
	BigInt(const BigInt &);

	//
	//param
	//
	void init_param();
	/*
	bCheck = true;
	bStep = false;

	ostream_width = 0;
	bLeftAlign = true;
	bComma = false;
	*/

	static void ostream_param_reset();

	static void set_check(bool check);
	static bool get_check();
	static UINT32 get_ostream_width();
	static void set_ostream_width(UINT32 width);
	static bool get_ostream_align();
	static void set_ostream_align(bool align);
	static bool get_ostream_comma();
	static void set_ostream_comma(bool sep);
	static bool get_step_display();
	static void set_step_display(bool label);
	static bool get_scnt();
	static void set_scnt(bool s);

	//
	//error
	//
	inline int Check(string source = "")const;	//���BigInt�Ƿ����쳣״̬
	bool SetCheck(bool aim);//����bCheck

	//
	//member
	//
	bool GetSign();
	bool SetSign(bool);
	int GetIndex();
	int* GetNum();

	inline void ReverseSign();
	inline bool isZero() const;
	inline bool isNotZero() const;
	inline bool isOne() const;
	inline bool isEven() const;

	//
	//function
	//
	void Display(bool newline = true);				//cout�ˣ�����ʽ���BigInt
	void DisplayInfo();								//���BigInt��Ϣ

	static BigInt abs(const BigInt& bi);	//ȡ����ֵ����ʱ��������޸�ԭ��
	static BigInt abs(const BigInt* bi);	//ȡ����ֵ����ʱ��������޸�ԭ��
	BigInt& abs();							//ԭ��ȡ����ֵ
	int GetBits()const;						//��ȡ10����λ��
	friend int getBits(const BigInt&);		//��ȡ��λ��
	bool LowestBitIs(byte);
	int GetFirstIndex();					//��ͽ�int
	inline bool SetZero();					//��Ϊ0�������Ƿ��޸�ԭ��
	int ShiftLeft9();						//����һ��(9λ),����ʵ����λ��
	int ShiftRight9();						//����һ��(9λ),����ʵ����λ��	
	int ShiftLeft(int = 1);					//���ƣ�����ʵ����λλ��
	int ShiftRight(int = 1);				//���ƣ�����ʵ����λλ��
	BigInt SubInt_rear(int);				//ȡָ��λ�������BigInt����λȡ��not advanced, should use shift
	BigInt SubInt_front(int);				//ȡָ��λ�������BigInt����λȡ��
	int CreateByCharArray_decimal(char*);	//����ʵ��ʹ��λ��
	char* GetCharArray();

	int Decomposition();
	void ShowDCPList();

	//
	//relation operator
	//
	//const ����Ϊ��ʹfriend������const BigInt����
	bool operator ==(const BigInt&)const;
	bool operator ==(const char&)const;
	bool operator ==(const UINT8&)const;
	bool operator ==(const short&)const;
	bool operator ==(const UINT16&)const;
	bool operator ==(const int&)const;
	bool operator ==(const UINT32&)const;
	bool operator ==(const long long&)const;
	bool operator ==(const UINT64&)const;

	friend bool operator ==(const char&, const BigInt&);
	friend bool operator ==(const UINT8&, const BigInt&);
	friend bool operator ==(const short&, const BigInt&);
	friend bool operator ==(const UINT16&, const BigInt&);
	friend bool operator ==(const int&, const BigInt&);
	friend bool operator ==(const UINT32&, const BigInt&);
	friend bool operator ==(const long long&, const BigInt&);
	friend bool operator ==(const UINT64&, const BigInt&);

	bool operator !=(const BigInt&)const;
	bool operator !=(const char&)const;
	bool operator !=(const UINT8&)const;
	bool operator !=(const short&)const;
	bool operator !=(const UINT16&)const;
	bool operator !=(const int&)const;
	bool operator !=(const UINT32&)const;
	bool operator !=(const long long&)const;
	bool operator !=(const UINT64&)const;

	friend bool operator !=(const char&, const BigInt&);
	friend bool operator !=(const UINT8&, const BigInt&);
	friend bool operator !=(const short&, const BigInt&);
	friend bool operator !=(const UINT16&, const BigInt&);
	friend bool operator !=(const int&, const BigInt&);
	friend bool operator !=(const UINT32&, const BigInt&);
	friend bool operator !=(const long long&, const BigInt&);
	friend bool operator !=(const UINT64&, const BigInt&);

	bool operator <(const BigInt&)const;
	bool operator <(const char&)const;
	bool operator <(const UINT8&)const;
	bool operator <(const short&)const;
	bool operator <(const UINT16&)const;
	bool operator <(const int&)const;
	bool operator <(const UINT32&)const;
	bool operator <(const long long&)const;
	bool operator <(const UINT64&)const;

	friend bool operator <(const char&, const BigInt&);
	friend bool operator <(const UINT8&, const BigInt&);
	friend bool operator <(const short&, const BigInt&);
	friend bool operator <(const UINT16&, const BigInt&);
	friend bool operator <(const int&, const BigInt&);
	friend bool operator <(const UINT32&, const BigInt&);
	friend bool operator <(const long long&, const BigInt&);
	friend bool operator <(const UINT64&, const BigInt&);

	bool operator >(const BigInt&)const;
	bool operator >(const char&)const;
	bool operator >(const UINT8&)const;
	bool operator >(const short&)const;
	bool operator >(const UINT16&)const;
	bool operator >(const int&)const;
	bool operator >(const UINT32&)const;
	bool operator >(const long long&)const;
	bool operator >(const UINT64&)const;

	friend bool operator >(const char&, const BigInt&);
	friend bool operator >(const UINT8&, const BigInt&);
	friend bool operator >(const short&, const BigInt&);
	friend bool operator >(const UINT16&, const BigInt&);
	friend bool operator >(const int&, const BigInt&);
	friend bool operator >(const UINT32&, const BigInt&);
	friend bool operator >(const long long&, const BigInt&);
	friend bool operator >(const UINT64&, const BigInt&);

	bool operator <=(const BigInt&)const;
	bool operator <=(const char&)const;
	bool operator <=(const UINT8&)const;
	bool operator <=(const short&)const;
	bool operator <=(const UINT16&)const;
	bool operator <=(const int&)const;
	bool operator <=(const UINT32&)const;
	bool operator <=(const long long&)const;
	bool operator <=(const UINT64&)const;

	friend bool operator <=(const char&, const BigInt&);
	friend bool operator <=(const UINT8&, const BigInt&);
	friend bool operator <=(const short&, const BigInt&);
	friend bool operator <=(const UINT16&, const BigInt&);
	friend bool operator <=(const int&, const BigInt&);
	friend bool operator <=(const UINT32&, const BigInt&);
	friend bool operator <=(const long long&, const BigInt&);
	friend bool operator <=(const UINT64&, const BigInt&);

	bool operator >=(const BigInt&)const;
	bool operator >=(const char&)const;
	bool operator >=(const UINT8&)const;
	bool operator >=(const short&)const;
	bool operator >=(const UINT16&)const;
	bool operator >=(const int&)const;
	bool operator >=(const UINT32&)const;
	bool operator >=(const long long&)const;
	bool operator >=(const UINT64&)const;

	friend bool operator >=(const char&, const BigInt&);
	friend bool operator >=(const UINT8&, const BigInt&);
	friend bool operator >=(const short&, const BigInt&);
	friend bool operator >=(const UINT16&, const BigInt&);
	friend bool operator >=(const int&, const BigInt&);
	friend bool operator >=(const UINT32&, const BigInt&);
	friend bool operator >=(const long long&, const BigInt&);
	friend bool operator >=(const UINT64&, const BigInt&);

	//
	//no logical operator
	//

	//
	//single operator
	//
	BigInt operator + ();
	BigInt operator - ();
	BigInt* operator & ();
	//BigInt& operator * ();

	//
	//calculation operator
	//
	//���к����ڲ����ö�Ӧ�������㺯��
	//����Ԫ������'/'��'%'�ڶ�Ӧ�����㺯������Ҫ��λ�������������Ͷ�Ӧ
	//'+','*'����Ԫ�����ڶ������������㣬�������ã���һ���������������
	BigInt operator + (const BigInt&);
	BigInt operator + (const char&);
	BigInt operator + (const UINT8&);
	BigInt operator + (const short&);
	BigInt operator + (const UINT16&);
	BigInt operator + (const int&);
	BigInt operator + (const UINT32&);
	BigInt operator + (const long long&);
	BigInt operator + (const UINT64&);

	friend BigInt operator +(const char&, BigInt);
	friend BigInt operator +(const UINT8&, BigInt);
	friend BigInt operator +(const short&, BigInt);
	friend BigInt operator +(const UINT16&, BigInt);
	friend BigInt operator +(const int&, BigInt);
	friend BigInt operator +(const UINT32&, BigInt);
	friend BigInt operator +(const long long&, BigInt);
	friend BigInt operator +(const UINT64&, BigInt);

	BigInt operator - (const BigInt&);
	BigInt operator - (const char&);
	BigInt operator - (const UINT8&);
	BigInt operator - (const short&);
	BigInt operator - (const UINT16&);
	BigInt operator - (const int&);
	BigInt operator - (const UINT32&);
	BigInt operator - (const long long&);
	BigInt operator - (const UINT64&);

	friend BigInt operator -(const char&, const BigInt&);
	friend BigInt operator -(const UINT8&, const BigInt&);
	friend BigInt operator -(const short&, const BigInt&);
	friend BigInt operator -(const UINT16&, const BigInt&);
	friend BigInt operator -(const int&, const BigInt&);
	friend BigInt operator -(const UINT32&, const BigInt&);
	friend BigInt operator -(const long long&, const BigInt&);
	friend BigInt operator -(const UINT64&, const BigInt&);

	BigInt operator * (const BigInt&);
	BigInt operator * (const char&);
	BigInt operator * (const UINT8&);
	BigInt operator * (const short&);
	BigInt operator * (const UINT16&);
	BigInt operator * (const int&);
	BigInt operator * (const UINT32&);
	BigInt operator * (const long long&);
	BigInt operator * (const UINT64&);

	friend BigInt operator *(const char&, BigInt);
	friend BigInt operator *(const UINT8&, BigInt);
	friend BigInt operator *(const short&, BigInt);
	friend BigInt operator *(const UINT16&, BigInt);
	friend BigInt operator *(const int&, BigInt);
	friend BigInt operator *(const UINT32&, BigInt);
	friend BigInt operator *(const long long&, BigInt);
	friend BigInt operator *(const UINT64&, BigInt);

	BigInt operator / (BigInt);
	BigInt operator / (const char&);
	BigInt operator / (const UINT8&);
	BigInt operator / (const short&);
	BigInt operator / (const UINT16&);
	BigInt operator / (const int&);
	BigInt operator / (const UINT32&);
	BigInt operator / (const long long&);
	BigInt operator / (const UINT64&);

	friend BigInt operator /(const char&, const BigInt&);
	friend BigInt operator /(const UINT8&, const BigInt&);
	friend BigInt operator /(const short&, const BigInt&);
	friend BigInt operator /(const UINT16&, const BigInt&);
	friend BigInt operator /(const int&, const BigInt&);
	friend BigInt operator /(const UINT32&, const BigInt&);
	friend BigInt operator /(const long long&, const BigInt&);
	friend BigInt operator /(const UINT64&, const BigInt&);

	BigInt operator % (BigInt);
	BigInt operator % (const char&);
	BigInt operator % (const UINT8&);
	BigInt operator % (const short&);
	BigInt operator % (const UINT16&);
	BigInt operator % (const int&);
	BigInt operator % (const UINT32&);
	BigInt operator % (const long long&);
	BigInt operator % (const UINT64&);

	friend BigInt operator %(const char&, const BigInt&);
	friend BigInt operator %(const UINT8&, const BigInt&);
	friend BigInt operator %(const short&, const BigInt&);
	friend BigInt operator %(const UINT16&, const BigInt&);
	friend BigInt operator %(const int&, const BigInt&);
	friend BigInt operator %(const UINT32&, const BigInt&);
	friend BigInt operator %(const long long&, const BigInt&);
	friend BigInt operator %(const UINT64&, const BigInt&);

	BigInt power(const BigInt&);
	BigInt power(const char&);
	BigInt power(const UINT8&);
	BigInt power(const short&);
	BigInt power(const UINT16&);
	BigInt power(const int&);
	BigInt power(const UINT32&);
	BigInt power(const long long&);
	BigInt power(const UINT64&);

	friend BigInt power(const char&, const BigInt&);
	friend BigInt power(const UINT8&, const BigInt&);
	friend BigInt power(const short&, const BigInt&);
	friend BigInt power(const UINT16&, const BigInt&);
	friend BigInt power(const int&, const BigInt&);
	friend BigInt power(const UINT32&, const BigInt&);
	friend BigInt power(const long long&, const BigInt&);
	friend BigInt power(const UINT64&, const BigInt&);

	BigInt selfFactorial();

	friend BigInt factorial(BigInt);
	friend BigInt factorial(const char&);
	friend BigInt factorial(const UINT8&);
	friend BigInt factorial(const short&);
	friend BigInt factorial(const UINT16&);
	friend BigInt factorial(const int&);
	friend BigInt factorial(const UINT32&);
	friend BigInt factorial(const long long&);
	friend BigInt factorial(const UINT64&);

	BigInt Permutation(const BigInt&);
	BigInt Permutation(const char&);
	BigInt Permutation(const UINT8&);
	BigInt Permutation(const short&);
	BigInt Permutation(const UINT16&);
	BigInt Permutation(const int&);
	BigInt Permutation(const UINT32&);
	BigInt Permutation(const long long&);
	BigInt Permutation(const UINT64&);

	friend BigInt Permutation(const char&, const BigInt&);
	friend BigInt Permutation(const UINT8&, const BigInt&);
	friend BigInt Permutation(const short&, const BigInt&);
	friend BigInt Permutation(const UINT16&, const BigInt&);
	friend BigInt Permutation(const int&, const BigInt&);
	friend BigInt Permutation(const UINT32&, const BigInt&);
	friend BigInt Permutation(const long long&, const BigInt&);
	friend BigInt Permutation(const UINT64&, const BigInt&);

	BigInt Combination(const BigInt&);
	BigInt Combination(const char&);
	BigInt Combination(const UINT8&);
	BigInt Combination(const short&);
	BigInt Combination(const UINT16&);
	BigInt Combination(const int&);
	BigInt Combination(const UINT32&);
	BigInt Combination(const long long&);
	BigInt Combination(const UINT64&);

	friend BigInt Combination(const char&, const BigInt&);
	friend BigInt Combination(const UINT8&, const BigInt&);
	friend BigInt Combination(const short&, const BigInt&);
	friend BigInt Combination(const UINT16&, const BigInt&);
	friend BigInt Combination(const int&, const BigInt&);
	friend BigInt Combination(const UINT32&, const BigInt&);
	friend BigInt Combination(const long long&, const BigInt&);
	friend BigInt Combination(const UINT64&, const BigInt&);

	//
	//self increase/decrease operator
	//
	BigInt& operator++();
	BigInt operator++(int);//����
	BigInt& operator--();
	BigInt operator--(int);


	static BigInt *Add(BigInt*, const BigInt*);
	static BigInt *Assign(BigInt*, const BigInt*);//1/100+time
	static BigInt *Add(BigInt*, const BigInt*, const BigInt*);//1/40time
	static BigInt *Mul(BigInt*, const BigInt*, const BigInt*);//0.9xtime
	static BigInt *Pow(BigInt*, const BigInt*, const BigInt*);
	//��ͨ�������ͻ�����BigInt, ���ò�����ͬ���ŵ�BigInt��������
	BigInt& operator += (const BigInt&);
	BigInt& operator += (const char&);
	BigInt& operator += (const UINT8&);
	BigInt& operator += (const short&);
	BigInt& operator += (const UINT16&);
	BigInt& operator += (const int&);
	BigInt& operator += (const UINT32&);
	BigInt& operator += (const long long&);
	BigInt& operator += (const UINT64&);

	BigInt& operator -= (const BigInt&);
	BigInt& operator -= (const char&);
	BigInt& operator -= (const UINT8&);
	BigInt& operator -= (const short&);
	BigInt& operator -= (const UINT16&);
	BigInt& operator -= (const int&);
	BigInt& operator -= (const UINT32&);
	BigInt& operator -= (const long long&);
	BigInt& operator -= (const UINT64&);

	BigInt& operator *= (const BigInt&);
	BigInt& operator *= (const char&);
	BigInt& operator *= (const UINT8&);
	BigInt& operator *= (const short&);
	BigInt& operator *= (const UINT16&);
	BigInt& operator *= (const int&);
	BigInt& operator *= (const UINT32&);
	BigInt& operator *= (const long long&);
	BigInt& operator *= (const UINT64&);
	//������ȡ����Ҫ�Բ�����������������λ����Ҫ��ʱ����
	BigInt& operator /= (BigInt);
	BigInt& operator /= (const char&);
	BigInt& operator /= (const UINT8&);
	BigInt& operator /= (const short&);
	BigInt& operator /= (const UINT16&);
	BigInt& operator /= (const int&);
	BigInt& operator /= (const UINT32&);
	BigInt& operator /= (const long long&);
	BigInt& operator /= (const UINT64&);

	BigInt& operator %= (BigInt);
	BigInt& operator %= (const char&);
	BigInt& operator %= (const UINT8&);
	BigInt& operator %= (const short&);
	BigInt& operator %= (const UINT16&);
	BigInt& operator %= (const int&);
	BigInt& operator %= (const UINT32&);
	BigInt& operator %= (const long long&);
	BigInt& operator %= (const UINT64&);
	//����4�ֺ���������Ҳ��Ҫ�޸�
	BigInt& oldselfpower(BigInt);
	BigInt& selfpower(BigInt);
	BigInt& selfpower(const char&);
	BigInt& selfpower(const UINT8&);
	BigInt& selfpower(const short&);
	BigInt& selfpower(const UINT16&);
	BigInt& selfpower(const int&);
	BigInt& selfpower(const UINT32&);
	BigInt& selfpower(const long long&);
	BigInt& selfpower(const UINT64&);

	BigInt& selfPermutation(BigInt);
	BigInt& selfPermutation(const char&);
	BigInt& selfPermutation(const UINT8&);
	BigInt& selfPermutation(const short&);
	BigInt& selfPermutation(const UINT16&);
	BigInt& selfPermutation(const int&);
	BigInt& selfPermutation(const UINT32&);
	BigInt& selfPermutation(const long long&);
	BigInt& selfPermutation(const UINT64&);

	BigInt& selfCombination(BigInt);
	BigInt& selfCombination(const char&);
	BigInt& selfCombination(const UINT8&);
	BigInt& selfCombination(const short&);
	BigInt& selfCombination(const UINT16&);
	BigInt& selfCombination(const int&);
	BigInt& selfCombination(const UINT32&);
	BigInt& selfCombination(const long long&);
	BigInt& selfCombination(const UINT64&);

	BigInt& selfGCD(BigInt);
	BigInt GCD(BigInt);
	BigInt EGCD(BigInt, BigInt&, BigInt&);
	BigInt& selfLCM(BigInt);
	BigInt MulInv();//�˷���Ԫ

	//
	//bit calculation operator
	//

	//
	//memory operator
	//

	//special operator
	//assignment operator
	//
	//�з�������������Ҫȡ����ֵ
	inline BigInt& operator = (const BigInt &);
	inline BigInt& operator = (char);
	inline BigInt& operator = (const UINT8 &);
	inline BigInt& operator = (short);
	inline BigInt& operator = (const UINT16 &);
	inline BigInt& operator = (int);
	inline BigInt& operator = (const UINT32 &);
	inline BigInt& operator = (long long);
	inline BigInt& operator = (const UINT64 &);

	//
	//stream operator
	//
	friend ostream& operator<< (ostream& str, BigInt& bi);
	friend istream& operator>> (istream& str, BigInt& bi);
};

void OutputTime(double time);

#define VALOF_STR3(S)		((S)[0] + ((S)[1]<<8) + (DWORD)((S)[2]<<16))
#define STR3_EQL(S1, S2)	(VALOF_STR3(S1) == VALOF_STR3(S2))
class BICalc {
private:
	BigInt lBig;
	BigInt rBig;
	BigInt ansBig;
	char answer[4];

	BigInt bx, by, bz;

	static const byte nOprSize = 4;
	char opr[nOprSize + 1];
	char singleOpr;
	static string singleOprList;
	static string remainOprList;
	CGrowableArray <string> oprList;

	LARGE_INTEGER frequency;
	LARGE_INTEGER starttime;
	LARGE_INTEGER endtime;

	HANDLE hConsole;
public:
	BICalc();
	inline void SetHConsole(HANDLE hCon)
	{
		hConsole = hCon;
	}
	inline void Clear()
	{
		lBig.SetZero();
		rBig.SetZero();
		memset(opr, 0, (nOprSize + 1) * sizeof(char));
	}
	bool GetLOperand();
	bool GetOperator();
	bool GetROperand();
	void Echo();
	void Calc();
	void Output();
};
