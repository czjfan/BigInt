#include<time.h>
#include<fstream>
using namespace std;

class stopwatch{
private:
	clock_t start;
	clock_t end;

	clock_t period;

	bool stdshow();
	bool file_show(ofstream &fout);

public:
	stopwatch();
	void go();
	long newone();
	long newone_show();
	long cut();
	long cut_show();
	long cut_show(ofstream &fout);

	static bool Go();
};