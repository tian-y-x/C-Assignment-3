#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <windows.h>
#include <smmintrin.h>
#include <xmmintrin.h>

#pragma optimize(2)

using namespace std;
using namespace literals;


float dot_product(const float* x, const float* y, const long& len);

#define Big
//#define input
const long long amount = long long int(2e8);

class BigData
{
public:
	long long len ;
	float* V;  
	BigData()
	{
		len = amount;
		V = (float*)malloc(amount * sizeof(float));
		if (!V)
			exit(-1);
	}
	void Init()
	{
		for (int i = 0; i < amount; i++)
		{
			V[i] = float(1) ;
		}
	}
};
#ifdef input
class Vec
{
public:

	long long len;
	float* v;
	string str;
	Vec()
	{
		len = 0;
		v = (float*)malloc(10000 * sizeof(float));
	}
	void equal(const Vec& b)
	{
		if (this->len != b.len)
			cout << "两向量维度不同" << endl, exit(-1);
		return;
	}
	void in()
	{
		float temp;
		getline(cin, str);
		size_t pos = 0, tpos = 0;
		char* p = &str[0];
		while (pos != str.length())
		{
			while (str[pos] == ' ')
				pos++;

			if (str[pos] != '-' && str[pos] != '+' && (str[pos] < '0' || str[pos]>'9') && (str[pos] != ' '))
				cout << "输入不合法" << endl, exit(-1); 
			if (this->len % 10000 == 0)
				realloc(v, 10000 + this->len);
			temp = stof(p + pos, &tpos);
			this->v[len++] = temp;
			pos += tpos;
			if (str[pos] == '-' || str[pos] == '+')
				cout << "输入不合法" << endl, exit(-1);  //避免出现形如-123-2被判断为合法
		}
	}
	
};
#endif

float* p, * q, * m, * n;
long float ans = 0;

/**************************SSE指令集***************************/
float dot_product(const float* x, const float* y, const long& len) {
	long float prod = 0.0f;
	const int mask = 0xff;
	__m128 X, Y;
	float tmp;


	long i;
	for (i = 0; i < len; i += 4) {
		X = _mm_loadu_ps(x + i); //_mm_loadu_ps把float转为__m128
		Y = _mm_loadu_ps(y + i);
		_mm_storeu_ps(&tmp, _mm_dp_ps(X, Y, mask));
		prod += tmp;
	}

	return prod;
}
/**************************SSE指令集***************************/


/***********************多线程*********************************/
HANDLE hMutex = NULL;//互斥量
//线程函数
DWORD WINAPI cal(LPVOID lpParamter)
{
	//请求一个互斥量锁
	WaitForSingleObject(hMutex, INFINITE);
	register int i, j;
	ans+=dot_product(p, q, 100000000);
	//释放互斥量锁
	ReleaseMutex(hMutex);
	return 0L;

}
/***********************多线程*********************************/



int main()
{
	auto startio = chrono::steady_clock::now();

	/*********初始化*********/
#ifdef Big
	register int i,j;
	BigData V1, V2;
	V1.Init();
	V2.Init();
	m = V1.V, n = V2.V;
	p = &(V1.V[100000000]), q = &(V2.V[100000000]);

	/*线程初始化*/
	hMutex = CreateMutex(NULL, FALSE, L"screen");
	HANDLE hThread = CreateThread(NULL, 0, cal, NULL, 0, NULL);
	CloseHandle(hThread);

	auto start = chrono::steady_clock::now();
	cout<< "IO: "<< chrono::duration_cast<chrono::microseconds>(start - startio).count() << "微秒" << endl;

	
	/*********计算***********/
	WaitForSingleObject(hMutex, INFINITE);
	ReleaseMutex(hMutex);


	ans+=dot_product(m, n, 100000000);
	cout << "The answer is: " << ans << endl;
	

	/************************/ 

	auto end = chrono::steady_clock::now();
	cout
		<< chrono::duration_cast<chrono::microseconds>(end - start).count() << "微秒 ≈ "
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << "毫秒 ";




	delete[] V1.V;
	delete[] V2.V;


#endif


#ifdef input
	Vec a, b;
	a.in();
	b.in();
	a.equal(b);
	



	auto start = chrono::steady_clock::now();
	cout
		<< "IO: "
		<<chrono::duration_cast<chrono::microseconds>(start - startio).count() << "微秒"<<endl;

	long float ans=0;
	
	for (int i = 0; i < a.len; i++)
	{
		ans += a.v[i] * b.v[i];
	}
	cout << ans << endl;;
	//cout<<DotProduct(a, b);




	auto end = chrono::steady_clock::now();

	cout
		<< chrono::duration_cast<chrono::microseconds>(end - start).count() << "微秒 ≈ "
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << "毫秒 ≈ "
		<< chrono::duration_cast<chrono::seconds>(end - start).count() << "秒.\n";
#endif
	return 0;
}
