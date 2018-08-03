#include"BigInt.h"

//
//static member variable
//
bool BigInt::bCheck = false;
bool BigInt::bStep = false;
unsigned int BigInt::ostream_width = 0;
bool BigInt::bLeftAlign = true;
bool BigInt::bComma = false;
bool BigInt::bScnt = false;

//
//static
//
void BigInt::ostream_param_reset()//��λ�������
{
	ostream_width = CHARS_PER_ROW_CONSOLE;
	bLeftAlign = true;
	//bComma = false;
}

bool BigInt::get_check()
{
	return bCheck;
}

void BigInt::set_check(bool check)
{
	bCheck = check;
}

UINT32 BigInt::get_ostream_width()
{
	return ostream_width;
}

void BigInt::set_ostream_width(UINT32 width)
{
	ostream_width = width;
}

bool BigInt::get_ostream_align()
{
	return bLeftAlign;
}

void BigInt::set_ostream_align(bool align)
{
	bLeftAlign = align;
}

bool BigInt::get_ostream_comma()
{
	return bComma;
}

void BigInt::set_ostream_comma(bool sep)
{
	bComma = sep;
}

bool BigInt::get_step_display()
{
	return bStep;
}

void BigInt::set_step_display(bool label)
{
	bStep = label;
}

bool BigInt::get_scnt()
{
	return bScnt;
}

void BigInt::set_scnt(bool s)
{
	bScnt = s;
}

BigInt BigInt::abs(const BigInt& bi)//ȡ����ֵ����ʱ��������޸�ԭ��
{
	BigInt temp(bi);
	if (!temp.positive)
		temp.positive = true;
	return temp;
}

BigInt BigInt::abs(const BigInt * bi)
{
	if (bi == NULL)
		return 0;
	BigInt temp(*bi);
	if (!temp.positive)
		temp.positive = true;
	return temp;
}

//
//param
//
void BigInt::init_param()
{
	bCheck = true;
	bStep = false;

	ostream_width = 0;
	bLeftAlign = true;
	bComma = false;
}

//
//error
//
int BigInt::Check(string source)const//��� Bigint����
{
	if (bCheck == false)
		return -1;

	if (!num || (num&&index < 1))
	{
		if (source[0] != '=')
			cerr << "�����ڴ�ջ��������" << endl;
		return 1;
	}

	int totalerror = 0;

	string checkhead("");
	checkhead.append("[check source = ");
	checkhead.append(source);
	checkhead.append("]BigInt in wrong status : ");

	//index<=0
	if (index <= 0)
	{
		totalerror++;

		cerr << checkhead;
		cerr << "index <= 0, set zero" << endl;

		//��Ϊconst��Ա����
		/*INIT_WITH_INDEX(1);
		num[1] = 0;*/
	}

	//num[0]����
	if (num[0] != 0)
	{
		totalerror++;

		cerr << checkhead;
		cerr << "num[0]!=0" << endl;

		num[0] = 0;
	}

	//�߽���0
	if (num[index] == 0 && index > 1)
	{
		totalerror++;

		cerr << checkhead;
		cerr << "top index = 0, corrected" << endl;

		//for (int i = index; i >= INDEX_BEGIN + 1; i--)
		//{
		//	//ֻ��ȥ�߽׿�ʼ��0��������һ�����㣬�˳�
		//	if (num[i] == 0)
		//		Cuts();
		//	else
		//		break;
		//}
	}

	//-0
	if (index == 1 && num[1] == 0 && !positive)
	{
		totalerror++;

		cerr << checkhead;
		cerr << "-0, corrected" << endl;

		/*positive = true;*/
	}

	//ĳһ��<0
	for (int i = INDEX_BEGIN; i <= index; i++)
	{
		int count_block_negative = 0;
		if (num[i] < 0)
		{
			totalerror++;
			count_block_negative++;

			if (count_block_negative == 1)
			{
				cerr << checkhead;
				cerr << "one block " << i << "found <0" << num[i] << ", corrected" << endl;
			}
			num[i] = -num[i];
		}
	}

	//ĳһ��>MAX_INT_BLOCK
	for (int i = INDEX_BEGIN; i <= index; i++)
	{
		int count_error_in_block_size = 0;
		if (num[i] > MAX_BLOCK)
		{
			totalerror++;
			count_error_in_block_size++;
			num[i] %= INT_BASE;

			if (count_error_in_block_size == 1)
			{
				cerr << checkhead;
				cerr << "one block found larger than MAX_INT_BLOCK, corrected" << endl;
			}
		}
	}

	if (totalerror)
		cerr << "**[check source = " << source << "]BigInt Check : " << totalerror << " error(s) found**" << endl;

	return totalerror;
}

bool BigInt::SetCheck(bool aim)
{
	if (aim != bCheck)
	{
		bCheck = aim;
		return true;
	}

	return false;
}

//
//constructor
//
BigInt::BigInt()//Ĭ�Ϲ�������0
{
	cap = 2;
	index = 1;

	num = new int[cap];
	num[0] = 0;
	num[1] = 0;

	positive = true;

	arr = NULL;
}

BigInt::BigInt(const char &i8)
{
	num = NULL;
	arr = NULL;

	*this = i8;
}

BigInt::BigInt(const UINT8 &ui8)
{
	num = NULL;
	arr = NULL;

	*this = ui8;
}

BigInt::BigInt(const short &si)
{
	num = NULL;
	arr = NULL;

	*this = si;
}

BigInt::BigInt(const UINT16 &ui16)
{
	num = NULL;
	arr = NULL;

	*this = ui16;
}

BigInt::BigInt(const int &i32)
{
	num = NULL;
	arr = NULL;

	*this = i32;
}

BigInt::BigInt(const UINT32 &ui)
{
	num = NULL;
	arr = NULL;

	*this = ui;
}

BigInt::BigInt(const long long &bi)
{
	num = NULL;
	arr = NULL;

	*this = bi;
}

BigInt::BigInt(const UINT64 &ui64)
{
	num = NULL;
	arr = NULL;

	*this = ui64;
}

BigInt::BigInt(bool unused, int INDEX)//����ָ��index����������MAX_CONSTRUCT_INDEX������ֵ��0
{
	if (INDEX > MAX_CONSTRUCT_INDEX)
	{
		cerr << "index to construct new BigInt too large! cut it" << endl;
		INDEX = MAX_CONSTRUCT_INDEX;
	}
	positive = true;
	index = INDEX;
	cap = index + 1;

	num = new int[cap];
	for (int i = WHOLE_INDEX_BEGIN; i <= index; i++)
	{
		num[i] = 0;
	}

	arr = NULL;
}

//
//copy constructor
//
BigInt::BigInt(const BigInt& bi)
{
	positive = bi.positive;
	index = bi.index;
	cap = index + 1;

	num = new int[cap];
	for (int i = WHOLE_INDEX_BEGIN; i <= index; i++)
	{
		num[i] = bi.num[i];
	}

	arr = NULL;
}

//
//member
//
bool BigInt::GetSign()
{
#ifdef _DEBUG
	Check("GetSign");
#endif

	return positive;
}

bool BigInt::SetSign(bool aim)//����positive=aim
{
#ifdef _DEBUG
	Check("SetSign");
#endif

	if (isZero())
	{
		return false;
	}

	if (positive == aim)//δ�޸�
	{
		return false;
	}
	else//�޸�
	{
		positive = aim;
		return true;
	}
}

int BigInt::GetIndex()
{
#ifdef _DEBUG
	Check("GetIndex");
#endif

	return index;
}

int* BigInt::GetNum()
{
#ifdef _DEBUG
	Check("GetNum");
#endif

	return num;
}

void BigInt::ReverseSign()//ȡ�෴��
{
	//Check("ReverseSign");

	if (!isZero())//��0��ȡ��
		positive = !positive;
}

inline bool BigInt::isZero() const//�ж��Ƿ�Ϊ0
{
	//Check("isZero");

	return ((index == 1) && !num[1]);
}

inline bool BigInt::isNotZero() const
{
	return ((index != 1) || num[1]);
}

inline bool BigInt::isOne() const
{
	return (!(index - 1) && num[1] == 1 && positive);
}

inline bool BigInt::isEven() const
{
	return (!(num[1] & 0x01));
}

void BigInt::Extends(int ext)
{
	if (ext <= 0)
		return;

	index += ext;
	if (index >= cap)//��չ����
	{
		int i = 0;
		//�洢ԭ��
		int *temp = new int[index + 1];
		for (i = WHOLE_INDEX_BEGIN; i <= index; i++)
			temp[i] = num[i];
		
		//�½�
		cap = max(cap * 2, cap + ext)/*cap + ext*/;
		SAFE_DELETELIST(num);
		num = new int[cap];

		//��������
		for (i = WHOLE_INDEX_BEGIN; i <= index - ext; i++)
			num[i] = temp[i];

		SAFE_DELETELIST(temp);
	}
	for (int i = index - ext + 1; i <= index; i++)//��չ���ĸ�λ����0
		num[i] = 0;
}

void BigInt::Cuts(int cut)
{
	if (cut <= 0)
		return;
	if (cut >= index)
	{
		cerr << "in Cuts: cut all, set zero" << endl;
		num[1] = 0;

		return;
	}

	index -= cut;
}

//
//function
//
void BigInt::Display(bool newline)
{
#ifdef _DEBUG
	Check("Display");
#endif

	cout << "BigInt: ";

	cout << "'";
	if (!positive)
		cout << "-";
	for (int i = index; i >= INDEX_BEGIN; i--)
	{
		if (i != index)
		{
			cout << ",";//���ŷָ�

			cout.fill('0');
			cout.width(BLOCK_WIDTH);
		}
		cout << num[i];
	}
	cout << "'";

	cout << "\t";
	cout << "(";
	if (positive)
		cout << "positive";
	else
		cout << "negative";

	cout << ", ";
	cout << "index:" << index;

	cout << ", ";
	cout << "bits:" << GetBits();

	cout << ") ";


	if (newline)
		cout << endl;
}

void BigInt::DisplayInfo()
{
	//cout << "( ";
	if (positive)
		cout << " +";
	else
		cout << " -";

	cout << "  ";
	cout << "index:" << index;

	cout << "  ";
	cout << "bits:" << GetBits();

	cout << "  ";
	cout << "cap:" << cap;

	//cout << " )";
}

BigInt& BigInt::abs()
{
#ifdef _DEBUG
	Check("abs");
#endif

	if (!positive)
		positive = true;
	return *this;
}

int BigInt::GetBits()const//��0����λ����0����1��index�Ƿ�(<=0)����-1
{
#ifdef _DEBUG
	Check("GetBits");
#endif

	if (index == 1 && num[1] == 0)
		return 1;

	if (num[index] == 0)
	{
		int i;
		for (i = index; i >= INDEX_BEGIN; i--)
		{
			if (num[i] != 0)
				break;
		}

		//i>=0
		cerr << "minor error found in GetBits: 0 of high index, real effective bits:";
		if (i == 0)//ÿ����0
		{
			cerr << "0" << endl;
		}
		else
		{
			cerr << ::getBits(num[i]) + (i - 1) * 9 << endl;
		}
	}

	return (::getBits(num[index]) + (index - 1) * 9);
}

int getBits(const BigInt &bi)
{
	return bi.GetBits();
}

bool BigInt::LowestBitIs(byte b)
{
#ifdef _DEBUG
	Check("LowestBitIs");
#endif

	return (num[1] % 10) == b;
}

int BigInt::GetFirstIndex()
{
	if (num && index >= 1)
		return num[1];
	else
		return 0;
}

bool BigInt::SetZero()
{
#ifdef _DEBUG
	Check("SetZero");
#endif

	if (isZero())
		return false;

	positive = true;
	index = 1;
	cap = index + 1;

	SAFE_DELETELIST(num);
	num = new int[cap];
	num[0] = num[1] = 0;

	return true;
}

int BigInt::ShiftLeft9()
{
#ifdef _DEBUG
	Check("ShiftLeft9");
#endif

	if (isZero())//ԭ����0������λ
		return 0;

	Extends();

	for (int i = index; i >= INDEX_BEGIN + 1; i--)
	{
		num[i] = num[i - 1];
	}

	num[1] = 0;

	return 9;
}

int BigInt::ShiftRight9()
{
#ifdef _DEBUG
	Check("ShiftRight9");
#endif

	if (index == 1)//ԭ����λ�����0
	{
		int shifts = GetBits();

		SetZero();

		return shifts;
	}
	else
	{
		for (int i = INDEX_BEGIN; i <= index - 1; i++)
		{
			num[i] = num[i + 1];
		}
		Cuts();

		return 9;
	}
}

#define MAX_SHIFTLEFT	100000000
int BigInt::ShiftLeft(int shifts)
{
#ifdef _DEBUG
	Check("ShiftLeft");
#endif

	if (isZero())
	{
		//cerr << "minor error in ShiftLeft:*this = 0" << endl;
		return 0;
	}


	if (shifts <= 0)
	{
		cerr << "error in ShiftLeft:shifts <= 0" << endl;
		return 0;
	}
	else if (shifts > MAX_SHIFTLEFT)
	{
		cerr << "minor error in ShiftLeft:shift > " << MAX_SHIFTLEFT << endl;
		return 0;
	}

	int nShifts = shifts;
	int nIndexShifts = shifts / BLOCK_WIDTH;
	if (nIndexShifts > 0)
	{
		//��չ
		Extends(nIndexShifts);
		shifts -= nIndexShifts*BLOCK_WIDTH;//����ʣ����λ��

		//��λ, ����߽׿�ʼ�����������nIndexShifts�׵�����ֱ�������1�׵���
		for (int i = index; i >= nIndexShifts + INDEX_BEGIN; i--)
			num[i] = num[i - nIndexShifts];
		//��λ��0
		for (int i = nIndexShifts; i >= INDEX_BEGIN; i--)
			num[i] = 0;
	}

	if (shifts == 0)//��λ��ɣ���λ����9�ı���
		return nShifts;

	//��ʣ�� 0 < shifts < 9 λ��Ҫ����
		
	if (getBits(num[index]) + shifts > BLOCK_WIDTH)
	{//������λ������߽ף���չ
		Extends();
		nIndexShifts++;
	}

	//��λǰ��ÿһ���н���Ҫ������(BLOCK_WIDTH-shifts)λ����Ebaseȡģ�õ���λ
	int Ebase = getE(BLOCK_WIDTH - shifts);
	//��λʱ��ÿһ��ʣ������Ҫ��λ���(��һ�׵�)shiftsλ,��Esupply��˵õ�
	int Esupply = getE(shifts);

	for (int i = index; i >= INDEX_BEGIN + 1; i--)
	{
		num[i] %= Ebase;
		num[i] *= Esupply;

		//������һ�׵�����(BLOCK_WIDTH-shifts)λ����
		num[i] += num[i - 1] / Ebase;
	}

	//��1�ײ��ٽ��ո���λ�����������������
	num[1] %= Ebase;
	num[1] *= Esupply;

	//����ʵ����λ��
	return nShifts;
}

int BigInt::ShiftRight(int shifts)
{
#ifdef _DEBUG
	Check("ShiftRight");
#endif
	
	if (shifts <= 0)
	{
		return 0;
	}
	else if (shifts >= GetBits())//��λ������ԭ��λ��
	{
		SetZero();
		return GetBits();
	}

	int nShifts = shifts;
	int nIndexShifts = shifts / 9;

	//��λ���ӱ���߽׵�nIndexShifts�Ľ׿�ʼ����������߽�����ֱ����1��
	for (int i = index - nIndexShifts; i >= INDEX_BEGIN; i--)
	{
		num[i] = num[i + nIndexShifts];
	}
	//����
	Cuts(nIndexShifts);
	shifts -= nIndexShifts*BLOCK_WIDTH;//����ʣ����λ��

	if (shifts == 0)//��λ����9�ı�������λ���
		return nShifts;

	//��ʣ�� 0 < shifts < 9 λ��Ҫ����
	
	//��λǰ��ÿһ���н���Ҫ��ȥ��shiftsλ����Ebaseȡ�̵õ���(9-shifts)λ
	int Ebase = getE(shifts);
	//��λʱ��ÿһ��ʣ�����ĸ�λ��Ҫ���߽������shiftsλ,���������Esupply��˵õ��͸�λ�������
	int Esupply = INT_BASE / Ebase;

	for (int i = INDEX_BEGIN; i <= index - 1; i++)
	{
		num[i] /= Ebase;

		//����߽׾��������shiftsλ
		num[i] += num[i + 1] % Ebase *Esupply;
	}

	//��߽ײ��ٽ��ո��߽������������������
	num[index] /= Ebase;

	//�����߽�=0, ��ȥ
	if (num[index] == 0)
		Cuts();

	//����ʵ����λ��
	return nShifts;
}

BigInt BigInt::SubInt_rear(int bits)
{
#ifdef _DEBUG
	Check("SubInt_rear");
#endif

	if (bits < 1)//������������<1
	{
		//cerr << "subInt_last error:bits shorter than 1" << endl;
		return NULL;
	}
	if (bits > GetBits())//������������ > ��λ��
	{
		cerr << "subInt_last error:bits longer than total bits" << endl;
		return NULL;
	}

	//�����
	BigInt temp(true, (bits - 1) / BLOCK_WIDTH + 1);

	//��߽�λ��
	int excess = getBits(num[index]);

	//ȡ������������ʼ�ף��ӵͽ����߽�
	int startindex = index - (bits - excess + BLOCK_WIDTH - 1) / BLOCK_WIDTH;

	//ÿ�����ף�λ��BLOCK_WIDTH����Ҫ��ȥ��index_cutλ
	//����ʣ�µ�BLOCK_WIDTH-index_cutλ�����������һ�׵Ľϵ�λ���ϸ�λ����һ��ȡ�ã�
	//���index_cut=BLOCK_WIDTH,������ԭ���׶���
	int index_cut = BLOCK_WIDTH - (bits - excess + BLOCK_WIDTH) % BLOCK_WIDTH;
	if (index_cut == BLOCK_WIDTH)
		index_cut = 0;

	for (int i = startindex, j = 1; i <= index; i++, j++)
	{
		//ʣ��λ��=0��ȡ�����������
		//���ǰһѭ��ȡ����߽ף���ʱʣ��λ��=0
		if (bits == 0)
			break;
		else if (bits < 0)//ʣ��λ��<0���쳣
		{
			cerr << "error in SubInt::after cut bits<0" << endl;
			break;
		}

		//cutindex�洢��������������һ��
		//ԭ����һ��ȡ����9-index_cutλ����������λ
		//�ٴӸ�һ��ȡ����index_cutλ����������λ������9-index_cutλ�������
		int cutindex = num[i] / getE(index_cut);
		if (index_cut != 0 && i < index)//�������ԭ�����һ�ף�ֻ��ִ���Ͼ����
		{
			cutindex += num[i + 1] % getE(index_cut)*getE(BLOCK_WIDTH - index_cut);
		}

		//������������һ��
		temp.num[j] = cutindex;

		//����ʣ��λ��
		if (bits >= BLOCK_WIDTH)
			bits -= BLOCK_WIDTH;
		else
			bits = 0;
	}

	return temp;
}

BigInt BigInt::SubInt_front(int bits)
{
#ifdef _DEBUG
	Check("SubInt_front");
#endif

	if (bits < 1)//������������<1
	{
		//cerr << "subInt_front error:bits shorter than 1" << endl;
		return NULL;
	}
	if (bits > GetBits())//������������ > ��λ��
	{
		cerr << "subInt_last error:front longer than total bits" << endl;
		return NULL;
	}

	//�����
	BigInt temp(true, (bits - 1) / BLOCK_WIDTH + 1);

	int i;
	for (i = 1; bits >= BLOCK_WIDTH; i++, bits -= BLOCK_WIDTH)
	{
		temp.num[i] = num[i];
	}

	//��ʣ��0<bits<BLOCK_WIDTHλ��Ҫȡ
	if (bits > 0)
	{
		temp.num[i] = num[i] % getE(bits);
	}

	return temp;
}

//�����ַ����������֣�10���ƣ�
int BigInt::CreateByCharArray_decimal(char* arr)
{
#ifdef _DEBUG
	Check("CreateByCharArray_decimal");
#endif

	size_t nLen = strlen(arr);
	bool bSign = (arr[0] == '-' || arr[0] == '+');//�Ƿ��з���
	if ((arr[0] < 48 || arr[0] > 57) && arr[0] != '-' && arr[0] != '+')//���ַ��쳣�ж�
	{
		cerr << "error in CreateByCharArray_decimal: invalid char" << endl;
		return 0;
	}
	for (size_t i = 1; i < nLen; i++)//�����ַ��쳣�ж�
	{
		if (arr[i] < 48 || arr[i] > 57)
		{
			cerr << "error in CreateByCharArray_decimal: invalid char" << endl;
			return 0;
		}
	}
	if (nLen <= (size_t)bSign)//�����Ƿ���������ַ�
	{
		cerr << "error in CreateByCharArray_decimal: array too short" << endl;
		return 0;
	}

	positive = !(arr[0] == '-');//����
	index = (nLen - 1 - bSign) / 9 + 1;//�����������Ҫȥ����һλ����λ
	SAFE_DELETELIST(num);
	INIT_WITH_INDEX(index);

	//���ַ���ĩβ��������
	for (int i = nLen - 1, j = 1; i >= (int)bSign; j++)
	{
		int elem = 0;
		//ÿ9λ�ַ�����һ��
		for (int k = 1; k <= BLOCK_BASE && i >= (int)bSign; k *= 10, i--)
		{
			elem += k*(arr[i] - 48);
		}
		num[j] = elem;
	}

	return nLen - bSign;
}

char* BigInt::GetCharArray()
{
	int nArrLen = GetBits() + !positive;//����ռλ
	SAFE_DELETELIST(arr);
	arr = new char[nArrLen + 1];
	arr[nArrLen] = '\0';

	int seq = 0;
	int elem = num[index];
	int lastBlockBase = BLOCK_BASE;

	//����λ
	if (!positive)
		arr[seq++] = '-';

	//���һ��ת��
	while (elem < lastBlockBase)
		lastBlockBase /= 10;
	for (int i = lastBlockBase; i >= 1; i /= 10, seq++)
	{
		arr[seq] = (char)(elem / i + 48);
		elem %= i;
	}

	//������ת�����Ӹ߽׵��ͽ�
	for (int idx = index - 1; idx >= INDEX_BEGIN; idx--)
	{
		elem = num[idx];//ȡ��������
		for (int i = BLOCK_BASE; i >= 1; i /= 10, seq++)
		{
			arr[seq] = (char)(elem / i + 48);
			elem %= i;
		}
	}

	return arr;
}

int BigInt::Decomposition()
{
	factors.RemoveAll();//��������б�

	UINT32 testFactor = 1;					//ѭ�����ȵ�������Ϊ3-2
	int nTested = 0;						//���������������
	int listCount = Prime::GetCount();		//�б�洢����������
	int listIndex = 1;						//�����б��±ꡣѭ�����ȵ�������Ϊ1����Ϊ��һ������2���±�0���Ѿ����
	bool bInfoShowed = false;				//�Ƿ��������Ϣ

	while (isEven() && !isZero())//�����������жϱ�2����
	{
		*this /= 2;
		factors.Add(2);
	}

	//ѭ����������
	while (!isOne())
	{
		//���´�������
		if (listIndex <= listCount)
		{//�Ӵ洢�б��л�ȡ��������
			testFactor = Prime::GetNum(++listIndex);
		}
		else
		{//�����б�洢��Χ����������+2���洢�б�����ʱʹ��
			testFactor += 2;
			if (testFactor % 10 == 5 && testFactor != 5)
				testFactor += 2;
		}

		//�Ƿ񳬳����Է�Χ
		if (/*fct2 > *this || */getBits(testFactor) > (GetBits() + 1) / 2)
		{
			factors.Add(*this);
			break;
		}

		//�����Ϣ
		nTested++;
		if (nTested % 10000 == 0/*nTested << 17 == 0*/)
		{
			BigInt tf(testFactor);
			bInfoShowed = true;
			cout << testFactor << '/' << *this << '(' << (tf*tf * 100 / *this) << "%)"
				<< '(' << nTested << '/' << Prime::GetCount() << ')' << " tried\r";
		}

		//�ҵ�������
		BigInt remainder = (*this % testFactor);
		while (remainder == 0 && *this > 1)
		{
			*this /= testFactor;
			factors.Add(testFactor);

			remainder = (*this % testFactor);
		}
	}
	if(bInfoShowed)
		cout << endl;

	return 0;
}

void BigInt::ShowDCPList()
{
	int nFactors = factors.GetSize();

	if (nFactors >= 1)
	{
		cout << factors[0];
		for (int i = 1; i < nFactors; i++)
		{
			cout << '*' << factors[i];
		}
	}
}

//
//relation operator
//
bool BigInt::operator==(const BigInt& bi)const
{
#ifdef _DEBUG
	Check("== BigInt");
#endif

	//������ͬ�����
	if (index != bi.index || positive != bi.positive)
		return false;

	//here ����index���
	for (int i = INDEX_BEGIN; i <= index; i++)
	{
		if (num[i] != bi.num[i])
			return false;
	}
	return true;
}

bool BigInt::operator==(const char &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool BigInt::operator==(const UINT8 &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool BigInt::operator==(const short &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool BigInt::operator==(const UINT16 &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool BigInt::operator==(const int &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool BigInt::operator==(const UINT32 &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool BigInt::operator==(const long long &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool BigInt::operator==(const UINT64 &eql)const
{
	BigInt bi(eql);

	return *this == bi;
}

bool operator==(const char &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool operator==(const UINT8 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool operator==(const short &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool operator==(const UINT16 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool operator==(const int &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool operator==(const UINT32 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool operator==(const long long &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool operator==(const UINT64 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp == bi;
}

bool BigInt::operator!=(const BigInt &bi)const
{
#ifdef _DEBUG
	Check("!= BigInt");
#endif

	//��������Ȼ����
	if (index != bi.index || positive != bi.positive)
		return true;

	//here ���������ͷ������
	for (int i = INDEX_BEGIN; i <= index; i++)
	{
		if (num[i] != bi.num[i])
			return true;
	}
	return false;
}

bool BigInt::operator!=(const char &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool BigInt::operator!=(const UINT8 &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool BigInt::operator!=(const short &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool BigInt::operator!=(const UINT16 &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool BigInt::operator!=(const int &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool BigInt::operator!=(const UINT32 &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool BigInt::operator!=(const long long &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool BigInt::operator!=(const UINT64 &eql)const
{
	BigInt bi(eql);

	return *this != bi;
}

bool operator!=(const char &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool operator!=(const UINT8 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool operator!=(const short &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool operator!=(const UINT16 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool operator!=(const int &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool operator!=(const UINT32 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool operator!=(const long long &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool operator!=(const UINT64 &eql, const BigInt &bi)
{
	BigInt temp(eql);

	return temp != bi;
}

bool BigInt::operator<(const BigInt &bi)const
{
#ifdef _DEBUG
	Check("< BigInt");
#endif

	if (positive>bi.positive)
		return false;
	if (positive < bi.positive)
		return true;

	//here����������ͬ
	if (positive)
	{
		if (index < bi.index)
			return true;
		else if (index > bi.index)
			return false;
	}
	else
	{
		if (index < bi.index)
			return false;
		else if (index > bi.index)
			return true;
	}

	//here����index�ͷ��Ŷ���ͬ(������δ��),�Ӹ߽׿�ʼ�Ƚ�
	for (int i = index; i >= INDEX_BEGIN; i--)
	{
		if (num[i] < bi.num[i] && positive || num[i]>bi.num[i] && !positive)
			return true;
		else if (num[i] < bi.num[i] && !positive || num[i]>bi.num[i] && positive)
			return false;
	}
	return false;
}

bool BigInt::operator<(const char &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool BigInt::operator<(const UINT8 &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool BigInt::operator<(const short &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool BigInt::operator<(const UINT16 &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool BigInt::operator<(const int &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool BigInt::operator<(const UINT32 &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool BigInt::operator<(const long long &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool BigInt::operator<(const UINT64 &big)const
{
	BigInt bi(big);

	return *this < bi;
}

bool operator<(const char &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool operator<(const UINT8 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool operator<(const short &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool operator<(const UINT16 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool operator<(const int &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool operator<(const UINT32 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool operator<(const long long &less, const BigInt& bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool operator<(const UINT64 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp < bi;
}

bool BigInt::operator>(const BigInt &bi)const
{
#ifdef _DEBUG
	Check("> BigInt");
#endif

	if (positive>bi.positive)
		return true;
	if (positive < bi.positive)
		return false;

	//here����������ͬ
	if (positive)
	{
		if (index > bi.index)
			return true;
		else if (index < bi.index)
			return false;
	}
	else
	{
		if (index < bi.index)
			return true;
		else if (index > bi.index)
			return false;
	}

	//here����index�ͷ��Ŷ���ͬ(������δ��),�Ӹ߽׿�ʼ�Ƚ�
	for (int i = index; i >= INDEX_BEGIN; i--)
	{
		if (num[i] < bi.num[i] && positive || num[i]>bi.num[i] && !positive)
			return false;
		else if (num[i] < bi.num[i] && !positive || num[i]>bi.num[i] && positive)
			return true;
	}
	return false;
}

bool BigInt::operator>(const char &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool BigInt::operator>(const UINT8 &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool BigInt::operator>(const short &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool BigInt::operator>(const UINT16 &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool BigInt::operator>(const int &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool BigInt::operator>(const UINT32 &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool BigInt::operator>(const long long &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool BigInt::operator>(const UINT64 &less)const
{
	BigInt bi(less);

	return *this > bi;
}

bool operator>(const char &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool operator>(const UINT8 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool operator>(const short &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool operator>(const UINT16 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool operator>(const int &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool operator>(const UINT32 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool operator>(const long long &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool operator>(const UINT64 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp > bi;
}

bool BigInt::operator<=(const BigInt &bi)const
{
#ifdef _DEBUG
	Check("<= BigInt");
#endif

	if (positive>bi.positive)
		return false;
	if (positive < bi.positive)
		return true;

	//here����������ͬ
	if (positive)
	{
		if (index < bi.index)
			return true;
		else if (index > bi.index)
			return false;
	}
	else
	{
		if (index < bi.index)
			return false;
		else if (index > bi.index)
			return true;
	}

	//here����index�ͷ��Ŷ���ͬ(������δ��),�Ӹ߽׿�ʼ�Ƚ�
	for (int i = index; i >= INDEX_BEGIN; i--)
	{
		if (num[i] < bi.num[i] && positive || num[i]>bi.num[i] && !positive)
			return true;
		else if (num[i] < bi.num[i] && !positive || num[i]>bi.num[i] && positive)
			return false;
	}
	return true;//��<�Ƚϣ���ͬ�ĵط�
}

bool BigInt::operator<=(const char &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool BigInt::operator<=(const UINT8 &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool BigInt::operator<=(const short &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool BigInt::operator<=(const UINT16 &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool BigInt::operator<=(const int &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool BigInt::operator<=(const UINT32 &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool BigInt::operator<=(const long long &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool BigInt::operator<=(const UINT64 &big)const
{
	BigInt bi(big);

	return *this <= bi;
}

bool operator<=(const char &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool operator<=(const UINT8 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool operator<=(const short &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool operator<=(const UINT16 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool operator<=(const int &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool operator<=(const UINT32 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool operator<=(const long long &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool operator<=(const UINT64 &less, const BigInt &bi)
{
	BigInt temp(less);

	return temp <= bi;
}

bool BigInt::operator>=(const BigInt &bi)const
{
#ifdef _DEBUG
	Check(">= BigInt");
#endif

	if (positive > bi.positive)
		return true;
	if (positive < bi.positive)
		return false;

	//here����������ͬ
	if (positive)
	{
		if (index < bi.index)
			return false;
		else if (index > bi.index)
			return true;
	}
	else
	{
		if (index < bi.index)
			return true;
		else if (index > bi.index)
			return false;
	}

	//here����index�ͷ��Ŷ���ͬ(������δ��),�Ӹ߽׿�ʼ�Ƚ�
	for (int i = index; i >= INDEX_BEGIN; i--)
	{
		if (num[i] < bi.num[i] && positive || num[i]>bi.num[i] && !positive)
			return false;
		else if (num[i] < bi.num[i] && !positive || num[i]>bi.num[i] && positive)
			return true;
	}
	return true;
}

bool BigInt::operator>=(const char &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool BigInt::operator>=(const UINT8 &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool BigInt::operator>=(const short &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool BigInt::operator>=(const UINT16 &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool BigInt::operator>=(const int &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool BigInt::operator>=(const UINT32 &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool BigInt::operator>=(const long long &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool BigInt::operator>=(const UINT64 &less)const
{
	BigInt bi(less);

	return *this >= bi;
}

bool operator>=(const char &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

bool operator>=(const UINT8 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

bool operator>=(const short &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

bool operator>=(const UINT16 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

bool operator>=(const int &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

bool operator>=(const UINT32 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

bool operator>=(const long long &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

bool operator>=(const UINT64 &big, const BigInt &bi)
{
	BigInt temp(big);

	return temp >= bi;
}

//
//single operator
//
BigInt BigInt::operator + ()
{
#ifdef _DEBUG
	Check("+");
#endif

	return *this;
}

BigInt BigInt::operator - ()
{
#ifdef _DEBUG
	Check("-");
#endif

	BigInt neg(*this);
	neg.ReverseSign();

	return neg;
}

BigInt* BigInt::operator&()
{
	return this;
}

//
//calculation operator
//
BigInt BigInt::operator+(const BigInt &bi)
{
	BigInt temp(*this);
	temp += bi;

	return temp;
}

BigInt BigInt::operator+(const char &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt BigInt::operator+(const UINT8 &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt BigInt::operator+(const short &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt BigInt::operator+(const UINT16 &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt BigInt::operator+(const int &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt BigInt::operator+(const UINT32 &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt BigInt::operator+(const long long &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt BigInt::operator+(const UINT64 &addend)
{
	BigInt temp(*this);
	BigInt temp2(addend);
	temp += temp2;

	return temp;
}

BigInt operator+(const char &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt operator+(const UINT8 &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt operator+(const short &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt operator+(const UINT16 &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt operator+(const int &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt operator+(const UINT32 &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt operator+(const long long &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt operator+(const UINT64 &addend, BigInt bi)
{
	BigInt temp(addend);
	temp += bi;

	return temp;
}

BigInt BigInt::operator-(const BigInt &bi)
{
	BigInt temp(*this);
	temp -= bi;

	return temp;
}

BigInt BigInt::operator-(const char &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt BigInt::operator-(const UINT8 &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt BigInt::operator-(const short &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt BigInt::operator-(const UINT16 &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt BigInt::operator-(const int &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt BigInt::operator-(const UINT32 &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt BigInt::operator-(const long long &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt BigInt::operator-(const UINT64 &subtractor)
{
	BigInt temp(*this);
	BigInt temp2(subtractor);
	temp -= temp2;

	return temp;
}

BigInt operator-(const char &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt operator-(const UINT8 &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt operator-(const short &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt operator-(const UINT16 &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt operator-(const int &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt operator-(const UINT32 &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt operator-(const long long &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt operator-(const UINT64 &minuend, const BigInt &bi)
{
	BigInt temp(minuend);
	temp -= bi;

	return temp;
}

BigInt BigInt::operator*(const BigInt &bi)
{
	BigInt temp(*this);
	temp *= bi;

	return temp;
}

BigInt BigInt::operator*(const char &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt BigInt::operator*(const UINT8 &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt BigInt::operator*(const short &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt BigInt::operator*(const UINT16 &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt BigInt::operator*(const int &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt BigInt::operator*(const UINT32 &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt BigInt::operator*(const long long &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt BigInt::operator*(const UINT64 &multiplicator)
{
	BigInt temp(*this);
	BigInt temp2(multiplicator);
	temp *= temp2;

	return temp;
}

BigInt operator*(const char &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt operator*(const UINT8 &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt operator*(const short &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt operator*(const UINT16 &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt operator*(const int &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt operator*(const UINT32 &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt operator*(const long long &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt operator*(const UINT64 &multiplicant, BigInt bi)
{
	BigInt temp(multiplicant);
	temp *= bi;

	return temp;
}

BigInt BigInt::operator/(BigInt bi)
{
	BigInt temp(*this);
	temp /= bi;

	return temp;
}

BigInt BigInt::operator/(const char &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor);
	temp /= temp2;

	return temp;
}

BigInt BigInt::operator/(const UINT8 &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor); 
	temp /= temp2;

	return temp;
}

BigInt BigInt::operator/(const short &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor);
	temp /= temp2;

	return temp;
}

BigInt BigInt::operator/(const UINT16 &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor);
	temp /= temp2;

	return temp;
}

BigInt BigInt::operator/(const int &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor);
	temp /= temp2;

	return temp;
}

BigInt BigInt::operator/(const UINT32 &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor);
	temp /= temp2;

	return temp;
}

BigInt BigInt::operator/(const long long &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor);
	temp /= temp2;

	return temp;
}

BigInt BigInt::operator/(const UINT64 &divisor)
{
	BigInt temp(*this);
	BigInt temp2(divisor);
	temp /= temp2;

	return temp;
}

BigInt operator/(const char &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt operator/(const UINT8 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt operator/(const short &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt operator/(const UINT16 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt operator/(const int &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt operator/(const UINT32 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt operator/(const long long &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt operator/(const UINT64 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp /= bi;

	return temp;
}

BigInt BigInt::operator%(BigInt bi)
{
	BigInt temp(*this);
	//temp %= bi;

	return temp %= bi;//TODO:�Ƿ�ȫ?
}

BigInt BigInt::operator%(const char &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt BigInt::operator%(const UINT8 &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt BigInt::operator%(const short &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt BigInt::operator%(const UINT16 &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt BigInt::operator%(const int &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt BigInt::operator%(const UINT32 &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt BigInt::operator%(const long long &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt BigInt::operator%(const UINT64 &mod)
{
	BigInt temp(*this);
	BigInt temp2(mod);
	temp %= temp2;

	return temp;
}

BigInt operator%(const char &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt operator%(const UINT8 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt operator%(const short &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt operator%(const UINT16 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt operator%(const int &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt operator%(const UINT32 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt operator%(const long long &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt operator%(const UINT64 &dividend, const BigInt &bi)
{
	BigInt temp(dividend);
	temp %= bi;

	return temp;
}

BigInt BigInt::power(const BigInt &bi)
{
	BigInt temp(*this);
	temp.selfpower(bi);

	return temp;
}

BigInt BigInt::power(const char &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt BigInt::power(const UINT8 &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt BigInt::power(const short &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt BigInt::power(const UINT16 &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt BigInt::power(const int &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt BigInt::power(const UINT32 &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt BigInt::power(const long long &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt BigInt::power(const UINT64 &exponent)
{
	BigInt temp(*this);
	BigInt temp2(exponent);
	temp.selfpower(temp2);

	return temp;
}

BigInt power(const char &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt power(const UINT8 &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt power(const short &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt power(const UINT16 &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt power(const int &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt power(const UINT32 &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt power(const long long &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt power(const UINT64 &base, BigInt &bi)
{
	BigInt temp(base);
	temp.selfpower(bi);

	return temp;
}

BigInt BigInt::selfFactorial()
{
#ifdef _DEBUG
	Check("selfFactorial");
#endif

	BigInt bi(*this);
	//���ԭ����Ϊ1
	positive = true;

	INIT_WITH_INDEX(1);
	num[1] = 1;

	for (; bi > 0; bi--)
	{
		*this *= bi;
	}

	return *this;
}

BigInt factorial(BigInt fct)
{
	fct.selfFactorial();

	return fct;
}

BigInt factorial(const char &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt factorial(const UINT8 &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt factorial(const short &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt factorial(const UINT16 &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt factorial(const int &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt factorial(const UINT32 &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt factorial(const long long &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt factorial(const UINT64 &fct)
{
	BigInt temp(fct);
	temp.selfFactorial();

	return temp;
}

BigInt BigInt::Permutation(const BigInt &bi)
{
	BigInt temp(*this);
	temp.selfPermutation(bi);

	return temp;
}

BigInt BigInt::Permutation(const char &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt BigInt::Permutation(const UINT8 &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt BigInt::Permutation(const short &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt BigInt::Permutation(const UINT16 &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt BigInt::Permutation(const int &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt BigInt::Permutation(const UINT32 &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt BigInt::Permutation(const long long &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt BigInt::Permutation(const UINT64 &pmt)
{
	BigInt temp(*this);
	BigInt temp2(pmt);
	temp.selfPermutation(temp2);

	return temp;
}

BigInt Permutation(const char &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt Permutation(const UINT8 &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt Permutation(const short &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt Permutation(const UINT16 &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt Permutation(const int &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt Permutation(const UINT32 &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt Permutation(const long long &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt Permutation(const UINT64 &pmt, const BigInt &bi)
{
	BigInt temp(pmt);
	temp.selfPermutation(bi);

	return temp;
}

BigInt BigInt::Combination(const BigInt &bi)
{
	BigInt temp(*this);
	temp.selfCombination(bi);

	return temp;
}

BigInt BigInt::Combination(const char &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt BigInt::Combination(const UINT8 &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt BigInt::Combination(const short &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt BigInt::Combination(const UINT16 &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt BigInt::Combination(const int &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt BigInt::Combination(const UINT32 &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt BigInt::Combination(const long long &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt BigInt::Combination(const UINT64 &cmb)
{
	BigInt temp(*this);
	BigInt temp2(cmb);
	temp.selfCombination(temp2);

	return temp;
}

BigInt Combination(const char &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt Combination(const UINT8 &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt Combination(const short &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt Combination(const UINT16 &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt Combination(const int &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt Combination(const UINT32 &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt Combination(const long long &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt Combination(const UINT64 &cmb, const BigInt &bi)
{
	BigInt temp(cmb);
	temp.selfCombination(bi);

	return temp;
}

BigInt * BigInt::Add(BigInt *dest, const BigInt *src)
{
#ifdef _DEBUG
	//Check("+= BigInt");
#endif
	if (dest == NULL || src == NULL)
	{
		return NULL;
	}

	//����ͬ�ţ����Ų��䣬�������
	if (src->positive == dest->positive)
	{
		int blocksum = 0;//ĳһ�׵ĺͣ����λ
		int i = 0;

		int minIdx = min(src->index, dest->index);
		for (i = INDEX_BEGIN, blocksum = 0; i <= minIdx; i++)
		{
			blocksum = src->num[i] + dest->num[i] + blocksum;//��int���ͣ�ǰ���Ǵ洢block�Ϸ���������MAX_BLOCK

			dest->num[i] = blocksum % INT_BASE;//��һ��

			blocksum /= INT_BASE;//��λ
		}

		if (i <= src->index)//������С��ԭ��,����λ��ԭ�������������
		{
			for (; i <= src->index && blocksum > 0; i++)//�н�λ�������ѭ��
			{
				blocksum = src->num[i] + blocksum;

				dest->num[i] = blocksum % INT_BASE;

				blocksum /= INT_BASE;
			}
			if (blocksum > 0)//��߽׽�λ
			{
				dest->Extends();
				dest->num[src->index] = blocksum;
			}
		}
		else if (i <= dest->index)//�����״���ԭ��������λ������������ӵ�ԭ��
		{
			dest->Extends(dest->index - src->index);
			for (; i <= dest->index; i++)
			{
				blocksum = dest->num[i] + blocksum;

				dest->num[i] = blocksum%INT_BASE;

				blocksum /= INT_BASE;
			}
			if (blocksum > 0)//��߽׽�λ
			{
				dest->Extends();
				dest->num[src->index] = blocksum;
			}
		}
		else//������ԭ��������ͬ
		{
			if (blocksum > 0)
			{
				dest->Extends();
				dest->num[src->index] = blocksum;
			}
		}

		return dest;
	}
	else//�������
	{
		if (abs(dest) < abs(src))
		{
			Add(dest, src);

			return dest;
		}

		bool abdicate = false;
		int i = 0;
		int minIdx = min(src->index, dest->index);
		for (i = INDEX_BEGIN; i <= minIdx; i++)
		{
			dest->num[i] = dest->num[i] - src->num[i] - abdicate;

			if (dest->num[i] < 0)//��λ
			{
				dest->num[i] += INT_BASE;

				abdicate = true;
			}
			else
			{
				abdicate = false;
			}
		}
		if (i <= dest->index)//������С��ԭ��,����λ��ԭ������������
		{
			for (; i <= src->index && abdicate; i++)
			{
				dest->num[i] = dest->num[i] - src->num[i] - abdicate;

				if (dest->num[i] < 0)//��λ
				{
					dest->num[i] += INT_BASE;
					abdicate = true;
				}
				else
				{
					abdicate = false;
				}
			}

			//��ʱabdicate�϶�=0
			if (dest->num[src->index] == 0)//��߽���1���ұ���λ,��ȥ
			{
				dest->Cuts();
			}
		}
		else if (i <= src->index)//�����״���ԭ�������ᷢ��
		{
		}
		else//������ԭ��������ͬ
		{
		}

		for (i = dest->index; i >= INDEX_BEGIN + 1; i--)
		{
			//ֻ��ȥ�߽׿�ʼ��0��������һ�����㣬�˳�
			if (dest->num[i] == 0)
				dest->Cuts();//�߽�0��ȥ
			else
				break;
		}

		return dest;
	}
}

BigInt * BigInt::Assign(BigInt *dest, const BigInt *src)
{
#ifdef _DEBUG
	//Check("= BigInt");
#endif
	if (dest == NULL || src == NULL)
	{
		return NULL;
	}

	dest->positive = src->positive;

	if (dest->index < src->index)
	{
		INIT_WITH_PINDEX(dest, src->index);
	}
	else
		dest->index = src->index;

	//��������
	for (int i = WHOLE_INDEX_BEGIN; i <= dest->index; i++)
	{
		dest->num[i] = src->num[i];
	}

	return dest;
}

BigInt * BigInt::Add(BigInt *dest, const BigInt *src, const BigInt *obj)
{
#ifdef _DEBUG
	//Check("+= BigInt");
#endif
	if (dest == NULL || src == NULL || obj == NULL)
	{
		return NULL;
	}

	//����ͬ�ţ����Ų��䣬�������
	if (src->positive == obj->positive)
	{
		int blocksum = 0;//ĳһ�׵ĺͣ����λ
		int i = 0;

		for (i = INDEX_BEGIN, blocksum = 0; i <= src->index && i <= obj->index; i++)
		{
			blocksum = src->num[i] + obj->num[i] + blocksum;//��int���ͣ�ǰ���Ǵ洢block�Ϸ���������MAX_BLOCK=999999999

			dest->num[i] = blocksum % INT_BASE;//��һ��

			blocksum /= INT_BASE;//��λ
		}

		if (i <= src->index)//������С��ԭ��,����λ��ԭ�������������
		{
			for (; i <= src->index && blocksum > 0; i++)//�н�λ�������ѭ��
			{
				blocksum = src->num[i] + blocksum;

				dest->num[i] = blocksum % INT_BASE;

				blocksum /= INT_BASE;
			}
			if (blocksum > 0)//��߽׽�λ
			{
				dest->Extends();
				dest->num[src->index] = blocksum;
			}
		}
		else if (i <= obj->index)//�����״���ԭ��������λ������������ӵ�ԭ��
		{
			dest->Extends(obj->index - src->index);
			for (; i <= obj->index; i++)
			{
				blocksum = obj->num[i] + blocksum;

				dest->num[i] = blocksum%INT_BASE;

				blocksum /= INT_BASE;
			}
			if (blocksum > 0)//��߽׽�λ
			{
				dest->Extends();
				dest->num[src->index] = blocksum;
			}
		}
		else//������ԭ��������ͬ
		{
			if (blocksum > 0)
			{
				dest->Extends();
				dest->num[src->index] = blocksum;
			}
		}

		return dest;
	}
	else//�������
	{
		if (abs(src) < abs(obj))
		{
			/*BigInt temp(*obj);
			temp += *src;

			*dest = temp;*/
			Add(dest, obj, src);

			return dest;
		}

		bool abdicate = false;
		int i = 0;
		for (i = INDEX_BEGIN; i <= src->index && i <= obj->index; i++)
		{
			dest->num[i] = src->num[i] - obj->num[i] - abdicate;

			if (dest->num[i] < 0)//��λ
			{
				dest->num[i] += INT_BASE;

				abdicate = true;
			}
			else
			{
				abdicate = false;
			}
		}
		if (i <= src->index)//������С��ԭ��,����λ��ԭ������������
		{
			for (; i <= src->index && abdicate; i++)
			{
				dest->num[i] = src->num[i] - obj->num[i] - abdicate;

				if (dest->num[i] < 0)//��λ
				{
					dest->num[i] += INT_BASE;
					abdicate = true;
				}
				else
				{
					abdicate = false;
				}
			}

			//��ʱabdicate�϶�=0
			if (dest->num[src->index] == 0)//��߽���1���ұ���λ,��ȥ
			{
				dest->Cuts();
			}
		}
		else if (i <= obj->index)//�����״���ԭ�������ᷢ��
		{
		}
		else//������ԭ��������ͬ
		{
		}

		for (i = src->index; i >= INDEX_BEGIN + 1; i--)
		{
			//ֻ��ȥ�߽׿�ʼ��0��������һ�����㣬�˳�
			if (dest->num[i] == 0)
				dest->Cuts();//�߽�0��ȥ
			else
				break;
		}

		return dest;
	}
}

#define tempindex (i+j-1)
BigInt * BigInt::Mul(BigInt *dest, const BigInt *src, const BigInt *obj)
{
#ifdef _DEBUG
	//Check("*= BigInt");
#endif
	if (dest == NULL || src == NULL || obj == NULL)
	{
		return NULL;
	}

	/*if (src->isZero() || obj->isZero())
	{
		dest->SetZero();

		return dest;
	}*/

	bool iflabelout = false;

	//����һ����temp,�����ǳ���������index��
	//temp���ȳ��㣬�п��ܽ����ȥһ��
	BigInt temp(true, (src->index + obj->index));
	temp.positive = (src->positive == obj->positive);

	//
	//��ʽ���㣺�����������棬����������
	//���ѭ���ǳ���
	//clock_t s = clock(), e;
	for (int j = INDEX_BEGIN; j <= obj->index; j++)
	{
		//�ڲ�ѭ���Ǳ�����
		int carry = 0;
		for (int i = INDEX_BEGIN; i <= src->index; i++)
		{
			/*if (bStep)
			{
			e = clock();

			if (e - s >= 1000)
			{
			s = clock();
			iflabelout = true;
			cout << "\r�Ѽ���" << j << '/' << bi.index << " & " << i << '/' << index;
			}
			}*/
			//multiply core
#define acce
#ifdef acce
			//int tempindex = i + j - 1;
			long long blockproduct = ((long long)src->num[i]) * obj->num[j] + temp.num[tempindex] + carry;
			temp.num[tempindex] = blockproduct%INT_BASE;
			carry = (int)(blockproduct / INT_BASE);
#else
			temp.num[tempindex] += carry;
			temp.num[tempindex] %= INT_BASE;
			carry = temp.num[tempindex] / INT_BASE;

			long long blockproduct = (long long)this->num[i] * bi.num[j];
			temp.num[tempindex] += blockproduct % INT_BASE;
			temp.num[tempindex] %= INT_BASE;
			carry += temp.num[tempindex] / INT_BASE;

			carry += blockproduct / INT_BASE;
#endif

			//�����������꣬�����н�λ
			if (i == src->index && carry > 0)
				temp.num[tempindex + 1] = carry;
		}
	}
	/*if (bStep && iflabelout)
	cout << endl;*/

	//������߽�
	if (temp.num[temp.index] == 0)
		temp.Cuts();

	if (temp.isZero())
		temp.positive = true;

	Assign(dest, &temp);

	return dest;
}
#undef tempindex

BigInt * BigInt::Pow(BigInt *dest, const BigInt *src, const BigInt *obj)
{
#ifdef _DEBUG
	//Check("Pow");
#endif
	if (dest == NULL || src == NULL || obj == NULL)
	{
		return NULL;
	}

	if (src->isZero() && obj->isZero())//0 power 0
	{
		cerr << "error in Pow: 0 power 0" << endl;
	}
	else if (obj->isZero())//0�η�
	{
		dest->positive = true;

		INIT_WITH_PINDEX(dest, 1);
		dest->num[1] = 1;

		return dest;
	}

	//����
	if (src->isOne())
	{
		INIT_WITH_PINDEX(dest, 1);
		dest->num[1] = 1;
		return dest;
	}
	else if (src->num[1] == 1 && src->index == 1)//-1, && !positive
	{
		INIT_WITH_PINDEX(dest, 1);
		dest->num[1] = 1;
		dest->positive = ((obj->num[1] & 1) == 0);
		return dest;
	}
	else if (!obj->positive)//����ֵ����1�����ĸ����η�����Ϊ0
	{
		dest->SetZero();
		return dest;
	}

	//��ʱԭ������ֵ����1

	//#define INDEX1_FILL_ACCE
#ifdef INDEX1_FILL_ACCE
	//1��������ģ��
	clock_t s1 = clock();
	if (index == 1 && num[1] <= SQR_BLOCK_FLOOR && bi != 1)
	{
		int newbi = 1;
		int i = 0;
		//ԭ���η���(i)���ܴ���bi
		for (i = 0; bi > i && newbi * num[1] <= MAX_BLOCK; i++)
		{
			newbi *= num[1];
		}
		//��ѭ��ʱ��i��ʾԭ���Ѿ���i�η�

		//����η���
		BigInt extra(1);
		int extratime = (bi % i).num[1];//bi%i�϶���int
		for (; extratime > 0; extratime--)
			extra *= *this;

		num[1] = newbi;
		selfpower(bi / i);//�����ټ��num[1]
		clock_t e1 = clock();

		*this *= extra;

		return *this;
	}
#endif

	clock_t s = clock();
	clock_t e;
#define RECURSION_METHOD
#ifdef RECURSION_METHOD
	//�ݹ��Գ˷���

	Assign(dest, src);
	//�洢ԭ��
	BigInt origin(*src);
	BigInt tempobj(*obj);
	//���γ�2����ָ���Ƚ����ж�ԭ���Ƿ�����Գ˷�������ǰ������ԭ�����ٴη�
	BigInt binary(1);
	for (; binary * 2 <= tempobj; binary *= 2)
	{
		if (dest->index == 1 && dest->num[1] <= SQR_BLOCK_FLOOR)
		{
			if (!dest->positive)
				dest->positive = true;
			dest->num[1] *= dest->num[1];
		}
		else
			Mul(dest, dest, dest);

		//if (BigInt::bStep)//�����ǩ
		//{
		//	e = clock();
		//	if (e - s >= 1000 || (binary * 4 > *obj))
		//	{
		//		s = clock();

		//		cout << "�Ѽ���" << binary * 2 << "/" << *obj << "�η�" << endl;
		//	}
		//}
	}

	tempobj -= binary;

	if (tempobj == 1)
		Mul(dest, dest, &origin);
	else
	{
		Pow(&origin, &origin, &tempobj);
		Mul(dest, dest, &origin);
	}

	/*if (bStep)
		cout << "�Ѽ���ԭ��" << binary << "�η���" << bi << "�η�" << endl;*/

#else

	//�ǵݹ��Գ˷���
	s = clock();

	//�洢ԭ��
	BigInt origin(*this);
	BigInt temp;
	*this = 1;
	//���γ�2����ָ���Ƚ����ж�ԭ���Ƿ�����Գ˷�������ǰ������ԭ�����ٴη�
	BigInt binary;
	for (; bi > 0;)
	{
		binary = 1;
		temp = origin;
		for (; binary * 2 <= bi; binary *= 2)
		{
			if (temp.index == 1 && temp.num[1] <= SQR_BLOCK_FLOOR)
				temp.num[1] *= temp.num[1];
			else
				temp *= temp;

			e = clock();
			if (e - s >= 1000 || (binary * 4 > bi))
			{
				s = clock();

				if (step_display)
					cout << "�Ѽ���" << binary * 2 << "/" << bi << "�η�" << endl;
			}
		}
		*this *= temp;
		bi -= binary;
	}
#endif
	
	return dest;
}

//
//selfcal operator
//
BigInt& BigInt::operator++()
{
#ifdef _DEBUG
	Check("++");
#endif

	if (positive)//�Ǹ���
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != MAX_BLOCK)
			{
				num[i]++;
				return *this;
			}
			num[i] = 0;
		}
		//���ѭ���л�û��return,˵���н�λ
		//BigInt����չ

		Extends();

		num[index] = 1;//���λΪ1

		return *this;
	}
	else//����
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != 0)//�޽�λ
			{
				num[i]--;

				if (num[i] == 0 && index == 1)//ԭ����1
					positive = 1;
				else if (num[i] == 0 && i == index)//ԭ�����λ��ȥ,���¹���num
				{
					Cuts();
				}
				return *this;
			}
			num[i] = MAX_BLOCK;
		}
		cerr << "error in BigInt:negative++ not correctly returned" << endl;
		return *this;
	}
}

BigInt& BigInt:: operator--()
{
#ifdef _DEBUG
	Check("--");
#endif

	if (!positive)//����
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != MAX_BLOCK)
			{
				num[i]++;
				return *this;
			}
			num[i] = 0;
		}
		//���ѭ���л�û��return,˵���н�λ
		//BigInt����չ

		Extends();

		num[index] = 1;//���λ��1

		return *this;
	}
	else//����
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != 0)//�޽�λ
			{
				num[i]--;


				if (num[i] == 0 && index == 1)//ԭ����1
					;//positive = 1;//0Ҳ��positive
				else if (num[i] == 0 && i == index)//ԭ�����λ��ȥ,���¹���num
				{
					Cuts();
				}
				return *this;
			}
			num[i] = MAX_BLOCK;
		}
		cerr << "error in BigInt:positive-- not correctly returned" << endl;
		return *this;
	}
}

BigInt BigInt::operator++(int)
{
#ifdef _DEBUG
	Check("++ rear");
#endif

	//�洢++֮ǰ��BigInt
	BigInt former(*this);

	if (positive)//�Ǹ���
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != MAX_BLOCK)//��һ��û��λ�ͽ���
			{
				num[i]++;
				return former;
			}
			else
				num[i] = 0;
		}

		//���ѭ���л�û��return,˵���н�λ
		//����չ
		Extends();
		num[index] = 1;

		return former;
	}
	else//����
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != 0)//�޽�λ
			{
				num[i]--;

				if (num[i] == 0 && index == 1)//ԭ����1
					positive = true;
				else if (num[i] == 0 && i == index)//ԭ�����λ��ȥ,���¹���num
				{
					Cuts();
				}
				return former;
			}
			else
				num[i] = MAX_BLOCK;
		}
		cerr << "error in BigInt:negative++ not correctly returned" << endl;
		return former;
	}
}

BigInt BigInt::operator--(int)
{
#ifdef _DEBUG
	Check("-- rear");
#endif

	//�洢δ--��BigInt
	BigInt former(*this);

	if (!positive)//����
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != MAX_BLOCK)
			{
				num[i]++;
				return former;
			}
			num[i] = 0;
		}

		//���ѭ���л�û��return,˵���н�λ
		//BigInt����չ
		Extends();
		num[index] = 1;

		return former;
	}
	else//�Ǹ���
	{
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			if (num[i] != 0)//�޽�λ
			{
				num[i]--;

				if (num[i] == 0 && index == 1)//ԭ����1
					;// positive = 1;0��positive
				else if (num[i] == 0 && i == index)//ԭ�����λ��ȥ,���¹���num
				{
					Cuts();
				}
				return former;
			}
			num[i] = MAX_BLOCK;
		}
		cerr << "error in BigInt:negative++ not correctly returned" << endl;
		return former;
	}
}

//
//assignment operator
//
BigInt& BigInt::operator += (const BigInt &bi)
{
#ifdef _DEBUG
	Check("+= BigInt");
#endif
	Add(this, this, &bi);

	return *this;
}

BigInt& BigInt::operator+=(const char &addend)
{
	BigInt bi(addend);
	Add(this, this, &bi);

	return *this;
}

BigInt& BigInt::operator+=(const UINT8 &addend)
{
	BigInt bi(addend);

	return *this += bi;
}

BigInt& BigInt::operator += (const short &addend)
{
	BigInt bi(addend);

	return *this += bi;
}

BigInt& BigInt::operator+=(const UINT16 &addend)
{
	BigInt bi(addend);

	return *this += bi;
}

BigInt& BigInt::operator += (const int &addend)
{
	BigInt bi(addend);

	return *this += bi;
}

BigInt& BigInt::operator+=(const UINT32 &addend)
{
	BigInt bi(addend);

	return *this += bi;
}

BigInt& BigInt::operator += (const long long &addend)
{
	BigInt bi(addend);

	return *this += bi;
}

BigInt& BigInt::operator+=(const UINT64 &addend)
{
	BigInt bi(addend);

	return *this += bi;
}

BigInt& BigInt::operator -= (const BigInt &bi)
{
#ifdef _DEBUG
	Check("-= BigInt");
#endif

	//����ͬ��
	if (positive == bi.positive)
	{
		/*if (abs(*this) < abs(bi))
		{
		BigInt temp(bi);
		temp -= (*this);

		*this = -temp;//����operator=
		return *this;
		}*/

		bool abdicate = false;
		int i = 0;
		for (i = INDEX_BEGIN; i <= index && i <= bi.index; i++)
		{
			num[i] = num[i] - bi.num[i] - abdicate;

			if (num[i] < 0)//��λ
			{
				num[i] += INT_BASE;

				abdicate = true;
			}
			else
			{
				abdicate = false;
			}
		}

		if (i <= index)//������С��ԭ��,����λ��ԭ������������
		{
			for (; i <= index && abdicate; i++)
			{
				num[i] = num[i] - abdicate;

				if (num[i] < 0)//��λ
				{
					num[i] += INT_BASE;
					abdicate = true;
				}
				else
				{
					abdicate = false;
				}
			}
		}
		else if (i <= bi.index)//�����״���ԭ��������λ������������ӵ�ԭ��
		{
			Extends(bi.index - index);
			for (; i <= index; i++)
			{
				num[i] = 0 - bi.num[i] - abdicate;

				if (num[i] < 0)//��λ
				{
					num[i] += INT_BASE;
					abdicate = true;
				}
				else
				{
					abdicate = false;
				}
			}

			//��ģ��
			abdicate = false;
			for (i = INDEX_BEGIN; i <= index; i++)
			{
				num[i] = INT_BASE - num[i] - abdicate;

				if (num[i] == INT_BASE)
					num[i] = 0;
				else
				{
					abdicate = true;
				}
			}
			positive = !positive;//���
		}
		else//������ԭ��������ͬ
		{
			//��������ԭ������ģ��
			if (abdicate)
			{
				abdicate = false;
				for (i = INDEX_BEGIN; i <= index; i++)
				{
					num[i] = INT_BASE - num[i] - abdicate;

					if (num[i] == INT_BASE)
						num[i] = 0;
					else
					{
						abdicate = true;
					}
				}
				positive = !positive;//���
			}
		}

		for (i = index; i >= INDEX_BEGIN + 1; i--)
		{
			//ֻ��ȥ�߽׿�ʼ��0��������һ�����㣬�˳�ѭ��
			if (num[i] == 0)
				Cuts();//�߽�0��ȥ
			else
				break;
		}

		//�����0
		if (iszero)
			positive = true;

		return *this;
	}
	else//������ţ�������ӣ����Ų���
	{
		int blocksum = 0;
		int i = 0;
		for (i = INDEX_BEGIN, blocksum = 0; i <= index && i <= bi.index; i++)
		{
			blocksum = num[i] + bi.num[i] + blocksum;//��int���ͣ�ǰ���Ǵ洢block�Ϸ���������MAX_BLOCK=999999999

			num[i] = blocksum%INT_BASE;

			blocksum /= INT_BASE;
		}
		if (i <= index)//������С��ԭ��,����λ��ԭ�������������
		{
			for (; i <= index && blocksum > 0; i++)
			{
				blocksum = num[i] + blocksum;

				num[i] = blocksum%INT_BASE;

				blocksum /= INT_BASE;
			}
			if (blocksum > 0)//��߽׽�λ
			{
				Extends();
				num[index] = blocksum;
			}
		}
		else if (i <= bi.index)//�����״���ԭ��������λ������������ӵ�ԭ��
		{
			for (; i <= bi.index; i++)
			{
				blocksum = bi.num[i] + blocksum;

				Extends();
				num[i] = blocksum%INT_BASE;

				blocksum /= INT_BASE;
			}
			if (blocksum > 0)//��߽׽�λ
			{
				Extends();
				num[index] = blocksum;
			}
		}
		else//������ԭ��������ͬ
		{
			if (blocksum > 0)
			{
				Extends();
				num[index] = blocksum;
			}
		}

		return *this;
	}
}

BigInt& BigInt::operator-=(const char &subtractor)
{
	BigInt bi(subtractor);
	
	return *this -= bi;
}

BigInt& BigInt::operator-=(const UINT8 &subtractor)
{
	BigInt bi(subtractor);

	return *this -= bi;
}

BigInt& BigInt::operator -= (const short &subtractor)
{
	BigInt bi(subtractor);

	return *this -= bi;
}

BigInt& BigInt::operator-=(const UINT16 &subtractor)
{
	BigInt bi(subtractor);

	return *this -= bi;
}

BigInt& BigInt::operator -= (const int &subtractor)
{
	BigInt bi(subtractor);

	return *this -= bi;
}

BigInt& BigInt::operator-=(const UINT32 &subtractor)
{
	BigInt bi(subtractor);

	return *this -= bi;
}

BigInt& BigInt::operator -= (const long long &subtractor)
{
	BigInt bi(subtractor);

	return *this -= bi;
}

BigInt& BigInt::operator-=(const UINT64 &subtractor)
{
	BigInt bi(subtractor);

	return *this -= bi;
}

#define tempindex i+j-1
BigInt& BigInt::operator *= (const BigInt& bi)
{
#ifdef _DEBUG
	//Check("*= BigInt");
#endif
	if (isZero() || bi.isZero())
	{
		SetZero();

		return *this;
	}

	bool iflabelout = false;

	//����һ����temp,�����ǳ���������index��
	//temp���ȳ��㣬�п��ܽ����ȥһ��
	BigInt temp(true, (index + bi.index));
	temp.positive = (positive == bi.positive);

	//
	//��ʽ���㣺�����������棬����������
	//���ѭ���ǳ���
	//clock_t s = clock(), e;
	for (int j = INDEX_BEGIN; j <= bi.index; j++)
	{
		//�ڲ�ѭ���Ǳ�����
		int carry = 0;
		for (int i = INDEX_BEGIN; i <= index; i++)
		{
			/*if (bStep)
			{
			e = clock();

			if (e - s >= 1000)
			{
			s = clock();
			iflabelout = true;
			cout << "\r�Ѽ���" << j << '/' << bi.index << " & " << i << '/' << index;
			}
			}*/
			//multiply core
#define acce
#ifdef acce
			//int tempindex = i + j - 1;
			long long blockproduct = ((long long)num[i]) * bi.num[j] + temp.num[tempindex] + carry;
			temp.num[tempindex] = blockproduct%INT_BASE;
			carry = (int)(blockproduct / INT_BASE);
#else
			temp.num[tempindex] += carry;
			temp.num[tempindex] %= INT_BASE;
			carry = temp.num[tempindex] / INT_BASE;

			long long blockproduct = (long long)this->num[i] * bi.num[j];
			temp.num[tempindex] += blockproduct % INT_BASE;
			temp.num[tempindex] %= INT_BASE;
			carry += temp.num[tempindex] / INT_BASE;

			carry += blockproduct / INT_BASE;
#endif

			//�����������꣬�����н�λ
			if (i == index && carry > 0)
				temp.num[tempindex + 1] = carry;
		}
	}
	/*if (bStep && iflabelout)
	cout << endl;*/

	//������߽�
	if (temp.num[temp.index] == 0)
		temp.Cuts();

	if (temp.isZero())
		temp.positive = true;

	Assign(this, &temp);

	return *this;
}

BigInt& BigInt::operator*=(const char &multiplicator)
{
	BigInt bi(multiplicator);
	
	return *this *= bi;
}

BigInt& BigInt::operator*=(const UINT8 &multiplicator)
{
	BigInt bi(multiplicator);

	return *this *= bi;
}

BigInt& BigInt::operator*=(const short &multiplicator)
{
	BigInt bi(multiplicator);

	return *this *= bi;
}

BigInt& BigInt::operator*=(const UINT16 &multiplicator)
{
	BigInt bi(multiplicator);

	return *this *= bi;
}

BigInt& BigInt::operator*=(const int &multiplicator)
{
	BigInt bi(multiplicator);

	return *this *= bi;
}

BigInt& BigInt::operator*=(const UINT32 &multiplicator)
{
	BigInt bi(multiplicator);

	return *this *= bi;
}

BigInt& BigInt::operator*=(const long long &multiplicator)
{
	BigInt bi(multiplicator);

	return *this *= bi;
}

BigInt& BigInt::operator*=(const UINT64 &multiplicator)
{
	BigInt bi(multiplicator);

	return *this *= bi;
}

BigInt& BigInt::operator/=(BigInt divisor)
{
#ifdef _DEBUG
	Check("/= BigInt");
#endif

	if (divisor.isZero())//������0
	{
		cerr << "error in BigInt operator /=" << endl;
		return *this;
	}

	bool resultsign;//�洢�������
	if (positive ^ divisor.positive)//���
		resultsign = false;
	else//ͬ��
		resultsign = true;

	abs();
	divisor.abs();

	if (*this < divisor)//�����0
	{
		positive = true;

		INIT_WITH_INDEX(1);
		num[1] = 0;

		return *this;
	}


	int bits_shift = GetBits() - divisor.GetBits();//�������������룬������Ҫ���Ƶ�λ��

	if (bits_shift>0)
		divisor.ShiftLeft(bits_shift);

	if (divisor>*this)//���ƺ󲻹���������������������һλ
	{
		divisor.ShiftRight();
		bits_shift--;
	}

	//�����洢�̽���ַ������λ��������1λ����mλ����������nλ�������̿�����m-n��m-n+1λ��
	//���divisor����һ�Σ�����m-nλ����bits_shift=m-n-1. ��������m-n+1λ��bits_shift=m-n
	//�����̵�λ�� = bits_shift + 1
	int charlength = bits_shift + 1;
	//����ַ�����
	char *ch;
	ch = new char[charlength + 2];//0λ�洢���ţ����һλ�洢'\0'��ֹ��
	ch[charlength + 1] = '\0';

	if (positive ^ divisor.positive)//���
		ch[0] = '-';
	else
		ch[0] = '+';

	//��*this�����޸ģ�temp��ʱ�洢���
	for (int bitnum = 1; bits_shift >= 0; bits_shift--, bitnum++)
	{
		int bit = 0;
		for (bit = 0; *this >= divisor; bit++)
		{
			*this -= divisor;
		}
		ch[bitnum] = bit + 48;

		if (bits_shift != 0)
			divisor.ShiftRight();
	}

	CreateByCharArray_decimal(ch);

	positive = resultsign;
	return divisor;
}

BigInt& BigInt::operator/=(const char &div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator/=(const UINT8 &div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator/=(const short& div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator/=(const UINT16 &div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator/=(const int& div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator/=(const UINT32 &div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator/=(const long long& div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator/=(const UINT64 &div)
{
	BigInt divisor(div);

	return *this /= divisor;
}

BigInt& BigInt::operator%=(BigInt modder)
{
#ifdef _DEBUG
	Check("%= BigInt");
#endif

	if (modder.isZero())//������0
	{
		cerr << "error in BigInt operator %= BigInt: modder = 0" << endl;
		return *this;
	}

	bool resultsign = positive;//�洢�������

	abs();
	modder.abs();

	if (*this < modder)//�����ԭ��
		return *this;


	int bits_shift = GetBits() - modder.GetBits();//���������ƶ���λ���뱻��������

	if (bits_shift>0)
		modder.ShiftLeft(bits_shift);

	if (modder > *this)
	{
		modder.ShiftRight();
		bits_shift--;
	}

	int charlength = bits_shift + 1;

	for (int bitnum = 1; bits_shift >= 0; bits_shift--, bitnum++)
	{
		int bit = 0;
		for (bit = 0; *this >= modder; bit++)
		{
			*this -= modder;
		}

		if (bits_shift != 0)
			modder.ShiftRight();
	}

	if (!isZero())
		positive = resultsign;

	return *this;
}

BigInt& BigInt::operator%=(const char &mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::operator%=(const UINT8 &mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::operator%=(const short& mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::operator%=(const UINT16 &mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::operator%=(const int& mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::operator%=(const UINT32 &mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::operator%=(const long long &mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::operator%=(const UINT64 &mod)
{
	BigInt modder(mod);

	return *this %= modder;
}

BigInt& BigInt::oldselfpower(BigInt bi)
{
#ifdef _DEBUG
	Check("oldselfpower BigInt");
#endif

	if (isZero() && bi.isZero())//0 power 0
	{
		cerr << "error in self power BigInt: 0 power 0" << endl;
	}
	else if (bi.isZero())//0�η�
	{
		positive = true;

		INIT_WITH_INDEX(1);
		num[1] = 1;

		return *this;
	}

	//����
	if (*this == 1)
	{
		return *this;
	}
	else if (abs(*this) == 1)//-1
	{
		positive = (bi.num[1] % 2 == 0);
		return *this;
	}
	else if (!bi.positive)//����ֵ����1�����ĸ����η�����Ϊ0
	{
		SetZero();
		return *this;
	}

	//��ʱԭ������ֵ����1

	//#define INDEX1_FILL_ACCE
#ifdef INDEX1_FILL_ACCE
	//1��������ģ��
	clock_t s1 = clock();
	if (index == 1 && num[1] <= SQR_BLOCK_FLOOR && bi != 1)
	{
		int newbi = 1;
		int i = 0;
		//ԭ���η���(i)���ܴ���bi
		for (i = 0; bi > i && newbi * num[1] <= MAX_BLOCK; i++)
		{
			newbi *= num[1];
		}
		//��ѭ��ʱ��i��ʾԭ���Ѿ���i�η�

		//����η���
		BigInt extra(1);
		int extratime = (bi % i).num[1];//bi%i�϶���int
		for (; extratime > 0; extratime--)
			extra *= *this;

		num[1] = newbi;
		selfpower(bi / i);//�����ټ��num[1]
		clock_t e1 = clock();

		*this *= extra;

		return *this;
	}
#endif

	clock_t s = clock();
	clock_t e;
#define RECURSION_METHOD
#ifdef RECURSION_METHOD
	//�ݹ��Գ˷���

	//�洢ԭ��
	BigInt origin(*this);
	//���γ�2����ָ���Ƚ����ж�ԭ���Ƿ�����Գ˷�������ǰ������ԭ�����ٴη�
	BigInt binary(1);
	for (; binary * 2 <= bi; binary *= 2)
	{
		if (index == 1 && num[1] <= SQR_BLOCK_FLOOR)
			num[1] *= num[1];
		else
			*this *= *this;

		if (BigInt::bStep)//�����ǩ
		{
			e = clock();
			if (e - s >= 1000 || (binary * 4 > bi))
			{
				s = clock();

				cout << "�Ѽ���" << binary * 2 << "/" << bi << "�η�" << endl;
			}
		}
	}

	bi -= binary;

	//ACCE
	if (bi == 1)
		*this *= origin;
	else
		*this *= (origin.selfpower(bi));

	if (bStep)
		cout << "�Ѽ���ԭ��" << binary << "�η���" << bi << "�η�" << endl;

#else

	//�ǵݹ��Գ˷���
	s = clock();

	//�洢ԭ��
	BigInt origin(*this);
	BigInt temp;
	*this = 1;
	//���γ�2����ָ���Ƚ����ж�ԭ���Ƿ�����Գ˷�������ǰ������ԭ�����ٴη�
	BigInt binary;
	for (; bi > 0;)
	{
		binary = 1;
		temp = origin;
		for (; binary * 2 <= bi; binary *= 2)
		{
			if (temp.index == 1 && temp.num[1] <= SQR_BLOCK_FLOOR)
				temp.num[1] *= temp.num[1];
			else
				temp *= temp;

			e = clock();
			if (e - s >= 1000 || (binary * 4 > bi))
			{
				s = clock();

				if (step_display)
					cout << "�Ѽ���" << binary * 2 << "/" << bi << "�η�" << endl;
			}
		}
		*this *= temp;
		bi -= binary;
	}
#endif

	return *this;
}

BigInt& BigInt::selfpower(BigInt bi)
{
#ifdef _DEBUG
	Check("selfpower BigInt");
#endif

	Pow(this, this, &bi);
	return *this;

	if (isZero() && bi.isZero())//0 power 0
	{
		cerr << "error in self power BigInt: 0 power 0" << endl;
	}
	else if (bi.isZero())//0�η�
	{
		positive = true;

		INIT_WITH_INDEX(1);
		num[1] = 1;

		return *this;
	}

	//����
	if (*this == 1)
	{
		return *this;
	}
	else if (num[1] == 1 && index == 1)//-1, && !positive
	{
		positive = ((bi.num[1] & 1) == 0);
		return *this;
	}
	else if (!bi.positive)//����ֵ����1�����ĸ����η�����Ϊ0
	{
		SetZero();
		return *this;
	}

	//��ʱԭ������ֵ����1

	//#define INDEX1_FILL_ACCE
#ifdef INDEX1_FILL_ACCE
	//1��������ģ��
	clock_t s1 = clock();
	if (index == 1 && num[1] <= SQR_BLOCK_FLOOR && bi != 1)
	{
		int newbi = 1;
		int i = 0;
		//ԭ���η���(i)���ܴ���bi
		for (i = 0; bi > i && newbi * num[1] <= MAX_BLOCK; i++)
		{
			newbi *= num[1];
		}
		//��ѭ��ʱ��i��ʾԭ���Ѿ���i�η�

		//����η���
		BigInt extra(1);
		int extratime = (bi % i).num[1];//bi%i�϶���int
		for (; extratime > 0; extratime--)
			extra *= *this;

		num[1] = newbi;
		selfpower(bi / i);//�����ټ��num[1]
		clock_t e1 = clock();

		*this *= extra;

		return *this;
	}
#endif

	clock_t s = clock();
	clock_t e;
#define RECURSION_METHOD
#ifdef RECURSION_METHOD
	//�ݹ��Գ˷���

	//�洢ԭ��
	BigInt origin(*this);
	//���γ�2����ָ���Ƚ����ж�ԭ���Ƿ�����Գ˷�������ǰ������ԭ�����ٴη�
	BigInt binary(1);
	for (; binary * 2 <= bi; binary *= 2)
	{
		if (index == 1 && num[1] <= SQR_BLOCK_FLOOR)
		{
			if (!positive)
				positive = true;
			num[1] *= num[1];
		}
		else
			*this *= *this;

		if (BigInt::bStep)//�����ǩ
		{
			e = clock();
			if (e - s >= 1000 || (binary * 4 > bi))
			{
				s = clock();

				cout << "�Ѽ���" << binary * 2 << "/" << bi << "�η�" << endl;
			}
		}
	}

	bi -= binary;

	if (bi == 1)
		*this *= origin;
	else
		*this *= (origin.selfpower(bi));

	if (bStep)
		cout << "�Ѽ���ԭ��" << binary << "�η���" << bi << "�η�" << endl;

#else

	//�ǵݹ��Գ˷���
	s = clock();

	//�洢ԭ��
	BigInt origin(*this);
	BigInt temp;
	*this = 1;
	//���γ�2����ָ���Ƚ����ж�ԭ���Ƿ�����Գ˷�������ǰ������ԭ�����ٴη�
	BigInt binary;
	for (; bi > 0;)
	{
		binary = 1;
		temp = origin;
		for (; binary * 2 <= bi; binary *= 2)
		{
			if (temp.index == 1 && temp.num[1] <= SQR_BLOCK_FLOOR)
				temp.num[1] *= temp.num[1];
			else
				temp *= temp;

			e = clock();
			if (e - s >= 1000 || (binary * 4 > bi))
			{
				s = clock();

				if (step_display)
					cout << "�Ѽ���" << binary * 2 << "/" << bi << "�η�" << endl;
			}
		}
		*this *= temp;
		bi -= binary;
	}
#endif


	return *this;
}

BigInt& BigInt::selfpower(const char &exponent)
{
	BigInt bi(exponent);
	
	return this->selfpower(bi);
}

BigInt& BigInt::selfpower(const UINT8 &exponent)
{
	BigInt bi(exponent);

	return this->selfpower(bi);
}

BigInt& BigInt::selfpower(const short &exponent)
{
	BigInt bi(exponent);

	return this->selfpower(bi);
}

BigInt& BigInt::selfpower(const UINT16 &exponent)
{
	BigInt bi(exponent);

	return this->selfpower(bi);
}

BigInt& BigInt::selfpower(const int &exponent)
{
	BigInt bi(exponent);

	return this->selfpower(bi);
}

BigInt& BigInt::selfpower(const UINT32 &exponent)
{
	BigInt bi(exponent);

	return this->selfpower(bi);
}

BigInt& BigInt::selfpower(const long long &exponent)
{
	BigInt bi(exponent);

	return this->selfpower(bi);
}

BigInt& BigInt::selfpower(const UINT64 &exponent)
{
	BigInt bi(exponent);

	return this->selfpower(bi);
}

BigInt& BigInt::selfPermutation(BigInt sel)
{
#ifdef _DEBUG
	Check("selfPermutation BigInt");
#endif

	//mPn, m<=0 �� n<0
	if (isZero() || !positive || !sel.positive)
	{
		cerr << "error in selfPermutation (BigInt): m <= 0 or n < 0, set zero" << endl;
		SetZero();

		return *this;
	}
	if (sel > *this)
	{
		cerr << "error in Permutation (BigInt): m < n, set zero" << endl;
		SetZero();

		return *this;
	}

	BigInt temp(1);
	for (sel = *this - sel + 1; sel <= *this; sel++)
	{
		temp *= sel;
	}

	*this = temp;

	/*
	sel = *this - sel;//m-n

	factorial(*this);

	sel.factorial(sel);

	*this /= sel;*/

	return *this;
}

BigInt& BigInt::selfPermutation(const char &pmt)
{
	BigInt bi(pmt);
	
	return this->selfPermutation(bi);
}

BigInt& BigInt::selfPermutation(const UINT8 &pmt)
{
	BigInt bi(pmt);

	return this->selfPermutation(bi);
}

BigInt& BigInt::selfPermutation(const short& pmt)
{
	BigInt bi(pmt);

	return this->selfPermutation(bi);
}

BigInt& BigInt::selfPermutation(const UINT16 &pmt)
{
	BigInt bi(pmt);

	return this->selfPermutation(bi);
}

BigInt& BigInt::selfPermutation(const int& pmt)
{
	BigInt bi(pmt);

	return this->selfPermutation(bi);
}

BigInt& BigInt::selfPermutation(const UINT32 &pmt)
{
	BigInt bi(pmt);

	return this->selfPermutation(bi);
}

BigInt& BigInt::selfPermutation(const long long& pmt)
{
	BigInt bi(pmt);

	return this->selfPermutation(bi);
}

BigInt& BigInt::selfPermutation(const UINT64 &pmt)
{
	BigInt bi(pmt);

	return this->selfPermutation(bi);
}

BigInt& BigInt::selfCombination(BigInt sel)
{
#ifdef _DEBUG
	Check("selfCombination BigInt");
#endif

	//mCn, m<=0 �� n<0
	if (isZero() || !positive || !sel.positive)
	{
		cerr << "error in Combination (BigInt): m <= 0 or n<0, set zero" << endl;
		SetZero();

		return *this;
	}
	if (sel > *this)
	{
		cerr << "error in Combination (BigInt): m < n, set zero" << endl;
		SetZero();

		return *this;
	}

	BigInt temp(1);
	BigInt stosel(sel);//�洢sel

					   //mCn, n��m-n�Ƚ�, n��С, �����˼���m!/(m-n)!
	if (sel + sel < *this)
	{
		for (sel = *this - sel + 1; sel <= *this; sel++)
		{
			temp *= sel;
		}

		for (sel = 1; sel <= stosel; sel++)//�ټ���/n!
		{
			temp /= sel;
		}
	}
	else//�����˼���m!/n!
	{
		for (sel = sel + 1; sel <= *this; sel++)
		{
			temp *= sel;
		}

		for (sel = 1; sel <= *this - stosel; sel++)//�ټ���/(m-n)!
		{
			temp /= sel;
		}
	}

	*this = temp;

	return *this;
}

BigInt& BigInt::selfCombination(const char &cbn)
{
	BigInt bi(cbn);
	
	return this->selfCombination(bi);
}

BigInt& BigInt::selfCombination(const UINT8 &cbn)
{
	BigInt bi(cbn);

	return this->selfCombination(bi);
}

BigInt& BigInt::selfCombination(const short &cbn)
{
	BigInt bi(cbn);

	return this->selfCombination(bi);
}

BigInt& BigInt::selfCombination(const UINT16 &cbn)
{
	BigInt bi(cbn);

	return this->selfCombination(bi);
}

BigInt& BigInt::selfCombination(const int &cbn)
{
	BigInt bi(cbn);

	return this->selfCombination(bi);
}

BigInt& BigInt::selfCombination(const UINT32 &cbn)
{
	BigInt bi(cbn);

	return this->selfCombination(bi);
}

BigInt& BigInt::selfCombination(const long long &cbn)
{
	BigInt bi(cbn);

	return this->selfCombination(bi);
}

BigInt& BigInt::selfCombination(const UINT64 &cbn)
{
	BigInt bi(cbn);

	return this->selfCombination(bi);
}

BigInt & BigInt::selfGCD(BigInt bi)
{
	if (!positive)
		ReverseSign();

	if (!bi.positive)
		bi.ReverseSign();

	while (bi.isNotZero())
	{
		//��a = b
		//��b = a % b
		*this %= bi;//b

		if (isZero())
		{
			*this = bi;
			return bi;
		}

		bi %= *this;
	}

	return *this;
}

BigInt BigInt::GCD(BigInt rb)
{
	BigInt lb = *this;
	lb.selfGCD(rb);
	return lb;
}

BigInt BigInt::EGCD(BigInt br, BigInt &bx, BigInt &by)
{
	if (br.isZero())
	{
		bx = 1;
		by = 0;
		return *this;
	}
	BigInt br2 = br;
	BigInt ans = br2.EGCD(*this%br, bx, by);
	BigInt temp = bx;
	bx = by;
	by = temp - *this / br*by;

	return ans;
}

BigInt & BigInt::selfLCM(BigInt bi)
{
	BigInt temp = *this;
	selfGCD(bi);
	*this = temp / (*this)*bi;

	return *this;
}

//
//special operator
//
BigInt& BigInt::operator = (const BigInt& bi)
{
#ifdef _DEBUG
	Check("= BigInt");
#endif

	Assign(this, &bi);

	return *this;
}

inline BigInt& BigInt::operator=(char i8)
{
#ifdef _DEBUG
	Check("= short");
#endif

	positive = (i8 >= 0);
	i8 = ::abs(i8);

	cap = 2;
	index = 1;

	num = new int[cap];
	num[0] = 0;
	num[1] = i8;

	return *this;
}

inline BigInt& BigInt::operator=(const UINT8 &ui8)
{
#ifdef _DEBUG
	Check("= UINT8");
#endif

	positive = true;

	cap = 2;
	index = 1;

	num = new int[cap];
	num[0] = 0;
	num[1] = ui8;

	return *this;
}

inline BigInt& BigInt::operator=(short si)
{
#ifdef _DEBUG
	Check("= short");
#endif

	positive = (si >= 0);
	si = ::abs(si);

	cap = 2;
	index = 1;

	num = new int[cap];
	num[0] = 0;
	num[1] = si;

	return *this;
}

inline BigInt& BigInt::operator=(const UINT16 &ui16)
{
#ifdef _DEBUG
	Check("= UINT16");
#endif

	positive = true;

	cap = 2;
	index = 1;

	num = new int[cap];
	num[0] = 0;
	num[1] = ui16;

	return *this;
}

inline BigInt& BigInt::operator=(int ni)
{
#ifdef _DEBUG
	Check("= int");
#endif

	positive = (ni >= 0);
	ni = ::abs(ni);

	if (ni > MAX_BLOCK)
	{
		cap = 3;
		index = 2;

		num = new int[cap];
		num[0] = 0;
		num[1] = ni % INT_BASE;
		num[2] = ni / INT_BASE;
	}
	else
	{
		cap = 2;
		index = 1;

		num = new int[2];
		num[0] = 0;
		num[1] = ni;
	}

	return *this;
}

inline BigInt& BigInt::operator=(const UINT32 &ui)
{
#ifdef _DEBUG
	Check("= UINT32");
#endif

	positive = true;

	if (ui > MAX_BLOCK)
	{
		cap = 3;
		index = 2;

		num = new int[cap];
		num[0] = 0;
		num[1] = ui % INT_BASE;
		num[2] = ui / INT_BASE;
	}
	else
	{
		cap = 2;
		index = 1;

		num = new int[cap];
		num[0] = 0;
		num[1] = ui;
	}

	return *this;
}

inline BigInt& BigInt::operator=(long long li)
{
	positive = (li >= 0);
	li = ::abs(li);

	if (li > MAX_2BLOCK)
	{
		cap = 4;
		index = 3;

		num = new int[cap];
		num[0] = 0;
		num[1] = li%INT_BASE;
		num[2] = (li / INT_BASE) % INT_BASE;
		num[3] = (int)(li / INT_2BASE);
	}
	else if (li > MAX_BLOCK)
	{
		cap = 3;
		index = 2;

		num = new int[cap];
		num[0] = 0;
		num[1] = li%INT_BASE;
		num[2] = (int)(li / INT_BASE);
	}
	else
	{
		cap = 2;
		index = 1;

		num = new int[cap];
		num[0] = 0;
		num[1] = (int)li;
	}

	return *this;
}

inline BigInt& BigInt::operator=(const UINT64 &li)
{
	positive = true;

	if (li > MAX_2BLOCK)
	{
		cap = 4;
		index = 3;

		num = new int[cap];
		num[0] = 0;
		num[1] = li%INT_BASE;
		num[2] = (li / INT_BASE) % INT_BASE;
		num[3] = (int)(li / INT_2BASE);
	}
	else if (li > MAX_BLOCK)
	{
		cap = 3;
		index = 2;

		num = new int[cap];
		num[0] = 0;
		num[1] = li%INT_BASE;
		num[2] = (int)(li / INT_BASE);
	}
	else
	{
		cap = 2;
		index = 1;

		num = new int[cap];
		num[0] = 0;
		num[1] = (int)li;
	}

	return *this;
}

//
//stream
//
ostream& operator<<(ostream& out, BigInt& bi)
{
#ifdef _DEBUG
	bi.Check("ostream <<");
#endif
	
	//������
	int nCommas = bi.index - 1;
	if (BigInt::bComma == false)
		nCommas = 0;

	if (BigInt::bLeftAlign)
	{
		//����
		if (!bi.positive)
			out << '-';
		//��߽�
		out << bi.num[bi.index];
		//����߽�����
		//��Ҫ����λ0���
		for (int i = bi.index - 1; i >= INDEX_BEGIN; i--)
		{
			//������ŷָ�
			if (BigInt::bComma)
				out << ',';

			out.fill('0');
			out.width(BLOCK_WIDTH);
			out << bi.num[i];
		}
	}
	else if (BigInt::bComma)
	{
		//�������¸�ʽ���
		//ÿ���и���֮���ö��ŷֿ�������������ŷָ���
		//[SPACES_FOR_SIGN]+ 000000000��000000000��000000000+[RIGHT_SPACES]
		//		  000000000��000000000��000000000��000000000+[RIGHT_SPACES]
		int nNumLen = bi.GetBits() + nCommas + !bi.positive;
		int nFirstLineLen = BigInt::ostream_width - RIGHT_SPACES - SPACES_FOR_SIGN;
		int nOtherLineLen = BigInt::ostream_width - RIGHT_SPACES;
		const int nBlockLen = ((int)BigInt::bComma + BLOCK_WIDTH);
		const int nFirstIndexLen = getBits(bi.num[bi.index]) + (int)!bi.positive;
		int nOtherIndexs = (nOtherLineLen - BLOCK_WIDTH) / nBlockLen + 1;//������ÿ�������ԷŶ��ٽ�
		int nFirstIndexs = (bi.index + nOtherIndexs - 1) % nOtherIndexs + 1;//��һ��ʵ�ʷŶ��ٽ�
		bool bSkipFirst = nFirstIndexLen + (nFirstIndexs - 1)*nBlockLen > nFirstLineLen;//��һ������Ų��£��ӵڶ��п�ʼ���
		int nFirstFills;//������Ҫ����Ŀո���
		if (bSkipFirst)
			nFirstFills = nOtherLineLen - nFirstIndexLen - (nFirstIndexs - 1)*nBlockLen;
		else
			nFirstFills = nFirstLineLen - nFirstIndexLen - (nFirstIndexs - 1)*nBlockLen;
		int nOtherFills = nOtherLineLen - (nOtherIndexs - 1)*nBlockLen - BLOCK_WIDTH;//��������Ҫ����Ŀո���

		if (bSkipFirst)
			out << endl;
		//��һ��
		for (int i = 0; i < nFirstFills; i++)
			out << ' ';
		if (!bi.positive)//����
			out << '-';
		out << bi.num[bi.index];//��߽׵������
		int idx = bi.index - 1;//ʣ����Ҫ����Ľ���
		for (; idx > bi.index - nFirstIndexs; idx--)
		{
			out << ',';

			//����߽����⣬��Ҫ����λ0���
			out.fill('0');
			out.width(BLOCK_WIDTH);
			out << bi.num[idx];
		}

		//ʣ����
		while (idx >= 1)
		{
			out << endl;
			for (int j = 0; j < nOtherFills; j++)
				out << ' ';

			out.fill('0');
			out.width(BLOCK_WIDTH);
			out << bi.num[idx--];
			for (int j = 1; j < nOtherIndexs && idx >= 1; j++, idx--)
			{
				out << ',';

				out.fill('0');
				out.width(BLOCK_WIDTH);
				out << bi.num[idx];
			}
		}
	}
	else//BigInt::bComma=false && BigInt::bLeftAlign=false
	{
		if (!bi.positive)//����
			out << '-';
		out << bi.num[bi.index];//��߽׵������
		int idx = bi.index - 1;//ʣ����Ҫ����Ľ���
		while (idx >= 1)
		{
			//����߽����⣬��Ҫ����λ0���
			out.fill('0');
			out.width(BLOCK_WIDTH);
			out << bi.num[idx--];
		}
	}

	//��ԭostream����
	BigInt::ostream_param_reset();

	return out;
}

istream& operator>>(istream& in, BigInt& bi)
{
	//�����ո�\t,\n,\r
	char ch;
	int arraysize = 9;//λ�ַ����������洢��������ռ�(bit)
	char* bitarray = new char[11];
	int countbits = 0;

	in >> resetiosflags(ios::skipws);//���Զ������ո�

	in >> ch;
	while (ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')//�����ո�
		in >> ch;

	//��һ������
	if (ch != '-' && ch != '+' && ch < '0' && ch > '9')
	{
		in.putback(ch);
		cerr << "error in BigInt istream: invalid first char, fail to get BigInt, nothing read" << endl;
		delete[] bitarray;
		return in;
	}

	bool bSign = (ch == '-' || ch == '+');
	if ((ch < 48 || ch > 57) && ch != '-' && ch != '+')
	{
		cerr << "error in BigInt istream: invalid first char" << endl;
		return in;
	}
	if (ch == '-')
		bitarray[countbits++] = '-';
	if (bSign)
		in >> ch;

	if (ch == '0')
	{
		bi.SetZero();
		while (ch == '0')//����ǰ��0
		{
			in >> ch;
		}
		//��������֧����
		if (ch < '0' || ch > '9')
		{
			in.putback(ch);
			//cerr << "in BigInt istream:just get an array of 0, construct bigint 0" << endl;
			delete[] bitarray;
			return in;
		}
	}

	if (ch > '0' && ch <= '9')
	{
		while (ch >= '0' && ch <= '9')
		{
			if (countbits > arraysize)//�ַ�������չ
			{
				arraysize *= 2;
				char *newbitarray = new char[arraysize + 2];

				for (int i = 0; i <= countbits - 1; i++)//copy, ��countbitsλ��ûд�룬������
				{
					newbitarray[i] = bitarray[i];
				}

				delete[] bitarray;

				bitarray = newbitarray;
			}

			bitarray[countbits++] = ch;
			bitarray[countbits] = '\0';

			in >> ch;
		}

		in.putback(ch);

		bi.CreateByCharArray_decimal(bitarray);

		delete[] bitarray;
		return in;
	}

	in.putback(ch);
	cerr << "error in BigInt istream:valid char after sign, fail to construct" << endl;

	delete[] bitarray;
	return in;
}

void OutputTime(double time)
{
	if (cout.good())
	{
		if (time >= 1)
			cout << (int)time << "s ";
		if (time >= 0.001)
			cout << ((int)(time * 1000000)) % 1000000 / 1000 << "ms ";
		cout << (time * 1000 - (int)(time * 1000)) * 1000 << "��s";
	}
}

//int Prime::GetFactor(BigInt num)
//{
//	if (primeI64)//���ݴ洢���������
//	{
//		for (int i = 1; i <= nCountU32 && num <= (int)(primeI64[i] * primeI64[i]); i++)
//		{
//			if (num%(int)primeI64[i] == 0)
//				return primeI64[i];
//		}
//	}
//
//	int prime = primeI64[nCountU32];
//	for (; getBits(prime) <= num.GetBits() / 2 + 1 && prime < num &&prime <= MAX_INT; prime += 2)
//	{
//		if (num % prime == 0)
//			return prime;
//	}
//	return prime;
//}

string BICalc::singleOprList = "+-*/%^!$cp";
string BICalc::remainOprList = "+-*/%^!$";

BICalc::BICalc()
{
	Clear();
	ansBig = lBig;

	strcpy_s(answer, "ans");

	int nSingleOprLen = singleOprList.length();
	for (int i = 0; i < nSingleOprLen; i++)
	{
		char str[2] = { 0 };
		str[0] = singleOprList.at(i);
		oprList.Add(string(str));
	}
	oprList.Add(string("egcd"));
	oprList.Add(string("gcd"));
	oprList.Add(string("lcm"));
	oprList.Add(string("<<"));
	oprList.Add(string(">>"));

	QueryPerformanceFrequency(&frequency);
	hConsole = NULL;
}

bool BICalc::GetLOperand()
{
	char c = 0;
	int i = 0;

	cin >> c;
	while (c == ' ' || c == '\t' || c == '\r' || c == '\n')//�����ո�
		cin >> c;
	//ʶ��answer������
	for (i = 0; c != '\n' && i < strlen(answer);)
	{
		if (c != answer[i])
		{
			cin.putback(c);
			for (int j = i; j > 0; j--)
				cin.putback(answer[j]);
			break;
		}
		i++;
		if (i == strlen(answer))
			break;
		cin >> c;
	}

	if (i == strlen(answer))
		lBig = ansBig;
	else
		cin >> lBig;//���������
	return true;
}

bool BICalc::GetOperator()
{
	char get;
	cin >> get;

	// �����հ��ַ�
	while (spacechar(get))
		cin >> get;

	// ��ȡ�����
	int i = 0;
	if (isalpha(get))//��ʶ����ʽ��������������֣����Ժ�'_'����ĸ��
	{
		while (validchar(get) && indentifierinitialchar(get))
		{
			opr[i++] = get;
			cin >> get;

			if (i > nOprSize)
				break;
		}
	}
	else//���Ÿ�ʽ�����
	{
		while (validchar(get) && !indentifierinitialchar(get) && !isdigit(get) && !spacechar(get))//TODO:����%-�ᱻ��Ϊ��һ�������
		{
			opr[i++] = get;
			cin >> get;

			if (i > nOprSize)
				break;
		}
	}
	cin.putback(get);
	opr[i] = '\0';

	// ������������ַ���ʽ
	singleOpr = opr[0];

	// ����������Ϸ���
	int nListLenU32 = oprList.GetSize();
	for (int i = 0; i < nListLenU32; i++)
	{
		if (_stricmp(opr, oprList.GetAt(i).c_str()) == 0)
			return true;
	}
	return false;
}

bool BICalc::GetROperand()
{
	char c = 0;
	int i = 0;
	for (i = 0; c != '\n' && i < strlen(answer); i++)
	{
		cin >> c;
		if (c != answer[i])
		{
			cin.putback(c);
			for (int j = i; j > 0; j--)
				cin.putback(answer[j]);
			break;
		}
	}

	if (i == strlen(answer))
		rBig = ansBig;
	else
	{
		if (singleOpr != '!' && singleOpr != '$')
			cin >> rBig;
	}
	return true;
}

void BICalc::Echo()
{
	//����������
	SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_GREYD));
	BigInt::set_ostream_width(CHARS_PER_ROW_CONSOLE);
	BigInt::set_ostream_align(false);
	for (int i = 0; i < SPACES_FOR_SIGN; i++)
		cout << ' ';//�ճ�����λ��ʹ���ֶ���
	cout << lBig << endl;

	//��������
	SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_REDD));
	cout << opr;

	//�Ҳ�����
	if (_stricmp(opr, "<<") == 0 || _stricmp(opr, ">>") == 0)
	{
		//���Ҳ�����
		cout << endl;
	}
	else if (singleOpr != '!' && singleOpr != '$')
	{
		//����Ҳ�����
		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_GREYD));
		BigInt::set_ostream_width(CHARS_PER_ROW_CONSOLE);
		BigInt::set_ostream_align(false);
		for (int i = 0; i < SPACES_FOR_SIGN - strlen(opr); i++)
			cout << ' ';
		cout << rBig << endl;
	}
	else if (singleOpr == '!' || singleOpr == '$')
	{
		//���Ҳ�����
		cout << endl;
	}
}

void BICalc::Calc()
{
	QueryPerformanceCounter(&starttime);
	bool bCalced = true;
	if (_stricmp(opr, "<<") == 0)
		lBig.ShiftLeft(11);
	else if(_stricmp(opr, ">>") == 0)
		lBig.ShiftRight(11);
	else
	{
		switch (singleOpr)
		{
		case '+':
			lBig += rBig;
			break;
		case '-':
			lBig -= rBig;
			break;
		case '*':
			BigInt::Mul(&lBig, &lBig, &rBig);
			//lBig *= rBig;
			break;
		case '/':
			lBig /= rBig;
			break;
		case '%':
			lBig %= rBig;
			break;
		case '^':
			BigInt::Pow(&lBig, &lBig, &rBig);
			//lBig.selfpower(rBig);
			break;
		case '!':
			lBig.selfFactorial();
			break;
		case 'c':
			lBig.selfCombination(rBig);
			break;
		case 'e':
			bz = lBig.GCD(rBig);
			lBig.EGCD(rBig, bx, by);
		case 'g':
			lBig.selfGCD(rBig);
			break;
		case 'l':
			lBig.selfLCM(rBig);
			break;
		case 'p':
			lBig.selfPermutation(rBig);
			break;
		case '$':
			lBig.Decomposition();
			break;
		default:
			bCalced = false;
			break;
		}
	}
	if(bCalced)
		ansBig = lBig;
	QueryPerformanceCounter(&endtime);
}

void BICalc::Output()
{
	//�����
	cout << "=";
	for (int i = 0; i < SPACES_FOR_SIGN - 1; i++)
		cout << ' ';
	if (singleOpr == '$')
	{
		lBig.ShowDCPList();
		cout << "\n";
	}
	else if (singleOpr == 'e')
	{
		//�Ҷ���
		BigInt::set_ostream_width(CHARS_PER_ROW_CONSOLE);
		BigInt::set_ostream_comma(true);
		BigInt::set_ostream_align(false);
		cout << lBig << "\n";
		
		for (int i = 0; i < SPACES_FOR_SIGN; i++)
			cout << ' ';
		BigInt::set_ostream_width(CHARS_PER_ROW_CONSOLE);
		BigInt::set_ostream_comma(true);
		BigInt::set_ostream_align(false);
		cout << bx << "\n";

		for (int i = 0; i < SPACES_FOR_SIGN; i++)
			cout << ' ';
		BigInt::set_ostream_width(CHARS_PER_ROW_CONSOLE);
		BigInt::set_ostream_comma(true);
		BigInt::set_ostream_align(false);
		cout << by << "\n";

		//�������Ϣ
		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_BLUED));
		lBig.DisplayInfo();
	}
	else
	{
		//�Ҷ���
		BigInt::set_ostream_width(CHARS_PER_ROW_CONSOLE);
		BigInt::set_ostream_comma(true);
		BigInt::set_ostream_align(false);
		cout << lBig << "\n";

		//�������Ϣ
		SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_BLUED));
		lBig.DisplayInfo();
	}

	//��ʱ��Ϣ
	SetConsoleTextAttribute(hConsole, CTX_WX(CONSOLE_GREEND));
	cout << "\ttime: ";
	float period = ((endtime.QuadPart - starttime.QuadPart) / (float)frequency.QuadPart);
	OutputTime(period);

	cout << endl << endl;
}