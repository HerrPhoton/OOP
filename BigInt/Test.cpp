#define _CRT_SECURE_NO_WARNINGS
#include "BigInt.h"

#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <windows.h>

using namespace std;

constexpr int CNT_OF_REPEATS_BENCHMARKING = 50000;
constexpr int CNT_OF_ONE_BLOCK_BENCHMARKING = 1000;

//Available operations { '+', '-', '*', '/', '%', '|', '&', '^' }
std::vector<char> oper_lst { '+', '-', '*', '/', '%', '|', '&', '^' };
//std::vector<char> oper_lst { '+', '-', '*', '/', '%' };
//Available signs { 1, -1};
std::vector<int> sign_lst {1, -1};
#define LONG_LONG_SUPPORT true

#define CALCULATE
#define DEBUG_LOG

bool exsistOper(char oper) {
	return std::find(oper_lst.begin(), oper_lst.end(), oper) != oper_lst.end();
}

bool exsistSign(int sign) {
	return std::find(sign_lst.begin(), sign_lst.end(), sign) != sign_lst.end();
}

#ifdef LONG_LONG_SUPPORT
#define CHECKEXPR(a,b,c) test_twoL(a,b,c)
#else
#define CHECKEXPR(a,b,c) test_twoI(a,b,c)
#endif
#ifdef DEBUG_LOG
#define LOG(a){a;}
#else
#define LOG(a){0;};
#endif
#ifdef CALCULATE
#define TEST_RUN
#include <sys/timeb.h>
typedef _timeb sys_time_t;
inline long long time_to_msec(const sys_time_t& t) {
	return t.time * 1000LL + t.millitm;
}

sys_time_t T_st, T_end;
vector<long long> timers;
#endif

int generateBIGint();
int generateSMALLint();

bool test_twoL(BigInt&, char&, BigInt&);
bool test_twoI(BigInt&, char&, BigInt&);
bool test_two(string);


int main() {

	BigInt a(1);
	BigInt b(3);
	
	BigInt c;


	std::cout << ~a << endl;

	c = a & b;
	std::cout << c << endl;

	c = a | b;
	std::cout << c << endl;
	
	c = a ^ b;
	std::cout << c << endl;

	bool haveBinary = false;

	std::cout << "Using long long to check: " << (LONG_LONG_SUPPORT ? "True" : "False") << endl;
	std::cout << "Number of operation calls: " << CNT_OF_REPEATS_BENCHMARKING << endl;
	std::cout << "Number of operation calls in block: " << CNT_OF_ONE_BLOCK_BENCHMARKING << endl;
	std::cout << "Operations to be checked: ";
	for (auto oper : oper_lst) {
		std::cout << oper << ' ';
	}
	std::cout << endl;
	if (exsistOper('&') || exsistOper('|') || exsistOper('^')) {
		std::cout << "[Warning] Binary operations are checked only on positive numbers" << endl;
		haveBinary = true;
	}
	std::cout << "Checking using positive numbers: " << (exsistSign(1) ? "True" : "False") << endl;
	std::cout << "Checking using negative numbers: " << (exsistSign(-1) ? "True" : "False") << endl;
	std::cout << endl << endl;
	
	std::cout << "Press enter to start" << endl;
	cin.get();

#ifdef CALCULATE
	if (CNT_OF_ONE_BLOCK_BENCHMARKING > CNT_OF_REPEATS_BENCHMARKING) {
		throw invalid_argument("WRONG ATRIBUTES");
	}

	int numbers_types_count = 4 - (LONG_LONG_SUPPORT ? 0 : 3);

	srand(time(NULL));

	bool is_correct = true;

	size_t test_num = 0;
	do {
		_ftime(&T_st);
		for (size_t block_num = 0; block_num <= CNT_OF_ONE_BLOCK_BENCHMARKING && is_correct; block_num++) {
			if (test_num == CNT_OF_REPEATS_BENCHMARKING) {
				is_correct = false;
				break;
			}

			int fstNum, sndNum;
			LOG(std::cout << "(" << test_num << ") ";)

			stringstream expr_s;

			char oper = oper_lst[rand() % oper_lst.size()];

			switch (rand() % numbers_types_count)
			{
			case(0):
				fstNum = generateSMALLint();
				sndNum = generateSMALLint();
				break;
			case(1):
				fstNum = generateBIGint();
				sndNum = generateSMALLint();
				break;
			case(2):
				fstNum = generateBIGint();
				sndNum = generateBIGint();
				break;
			case(3):
				sndNum = generateBIGint();
				fstNum = generateSMALLint();
				break;
			}

			if ((oper == '/') || (oper == '&')) {
				sndNum += 1;
			}
			if (!(oper == '&' || oper == '|' || oper == '^')){
				fstNum *= sign_lst[rand() % sign_lst.size()];
				sndNum *= sign_lst[rand() % sign_lst.size()];
			}

			expr_s << fstNum << ' ' << oper << ' ' << sndNum;

			try {
				is_correct = test_two(expr_s.str());
			}
			catch (...) {
				std::cout << "\tAn error occurred in the test. Continue testing" << endl;
			}
			test_num++;
		};
		_ftime(&T_end);
		timers.push_back(time_to_msec(T_end) - time_to_msec(T_st));
	} while (is_correct);


	long long total_time = 0;
	for (auto i : timers) {
		total_time += i;
	}

	//Mixed (pos/neg)
	//91094ms total work time(50000 cycles)
	//1821ms(1000 cycles)
	//1.82188ms on 1 cycle

	//Add (only positive)
	//93394ms total work time(50000 cycles)
	//1867ms(1000 cycles)
	//1.86788ms on 1 cycle

	//Sub (only positive)
	//93502ms total work time(50000 cycles)
	//1870ms(1000 cycles)
	//1.87004ms on 1 cycle

	//Multiplication (pos/neg)
	//117658ms total work time(50000 cycles)
	//2353ms(1000 cycles)
	//2.35316ms on 1 cycle

	//Division (pos/neg)
	//75847ms total work time(50000 cycles)
	//1516ms(1000 cycles)
	//1.51694ms on 1 cycle

	//Mod (pos/neg)
	//69660ms total work time(50000 cycles)
	//1393ms(1000 cycles)
	//1.3932ms on 1 cycle

	std::cout << "Operations completed: " << test_num << endl;
	std::cout << total_time << "ms total work time (" << CNT_OF_REPEATS_BENCHMARKING << " cycles)" << endl;
	std::cout << total_time / timers.size() << "ms (" << CNT_OF_ONE_BLOCK_BENCHMARKING << " cycles)" << endl;
	std::cout << (double)total_time / timers.size() / CNT_OF_ONE_BLOCK_BENCHMARKING << "ms on 1 cycle" << endl;

#endif
	return 0;
}

bool test_two(string expr) {
	LOG(std::cout << "TESTING this: " << expr;)

	char* pos = strtok((char*)expr.c_str(), " ");
	BigInt* a = new BigInt((string(pos)));
	pos = strtok(NULL, " ");
	char oper = *pos;
	pos = strtok(NULL, " ");
	BigInt* b = new BigInt((string(pos)));
	
	bool is_correct = CHECKEXPR(*a, oper, *b);

	delete a;
	delete b;
	return is_correct;
}

bool test_twoL(BigInt& a, char& oper, BigInt& b) {

	BigInt my_ans{ 0 };
	long long correct_ans{ 0 };

	bool is_correct = false;

	switch (oper)
	{
	case('+'):
		correct_ans = (long long)a + (long long)b;
		my_ans = a + b;
		break;
	case('-'):
		correct_ans = (long long)a - (long long)b;
		my_ans = a - b;
		break;
	case('*'):
		correct_ans = (long long)a * (long long)b;
		my_ans = a * b;
		break;
	case('/'):
		correct_ans = (long long)a / (long long)b;
		my_ans = a / b;
		break;
	case('%'):
		correct_ans = (long long)a % (long long)b;
		my_ans = a % b;
		break;
	case('|'):
		correct_ans = (long long)a | (long long)b;
		my_ans = a | b;
		break;
	case('&'):
		correct_ans = (long long)a & (long long)b;
		my_ans = a & b;
		break;
	case('^'):
		correct_ans = (long long)a ^ (long long)b;
		my_ans = a ^ b;
		break;
	}

	is_correct = (BigInt)correct_ans == my_ans;
	LOG(std::cout << " = [" << my_ans << "] / {" << correct_ans << "} ";
	std::cout << (is_correct ? "CORRECT" : "!!WRONG!!");
	std::cout << endl;)
	return is_correct;
}

bool test_twoI(BigInt& a, char& oper, BigInt& b) {

	BigInt my_ans{ 0 };
	int correct_ans{ 0 };

	bool is_correct = false;

	switch (oper)
	{
	case('+'):
		correct_ans = (int)a + (int)b;
		my_ans = a + b;
		break;
	case('-'):
		correct_ans = (int)a - (int)b;
		my_ans = a - b;
		break;
	case('*'):
		correct_ans = (int)a * (int)b;
		my_ans = a * b;
		break;
	case('/'):
		correct_ans = (int)a / (int)b;
		my_ans = a / b;
		break;
	case('%'):
		correct_ans = (int)a % (int)b;
		my_ans = a % b;
		break;
	case('|'):
		correct_ans = (int)a | (int)b;
		my_ans = a | b;
		break;
	case('&'):
		correct_ans = (int)a & (int)b;
		my_ans = a & b;
		break;
	case('^'):
		correct_ans = (int)a ^ (int)b;
		my_ans = a ^ b;
		break;
	}

	is_correct = (BigInt)correct_ans == my_ans;
	LOG(std::cout << " = [" << my_ans << "] / {" << correct_ans << "} ";
	std::cout << (is_correct ? "CORRECT" : "!!WRONG!!");
	std::cout << endl;)
		return is_correct;
}


int generateBIGint() {
	return ((int)rand() + ((int)rand() << 16));
}
int generateSMALLint() {
	return ((int)rand());
}