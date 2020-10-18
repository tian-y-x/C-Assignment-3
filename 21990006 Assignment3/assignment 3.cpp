#include <iostream>
#include <algorithm>
#include <string>
#include <cmath>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <windows.h>

#pragma optimize(2)
using namespace std;
using namespace literals;




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
			V[i] = float(1.1) ;
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
			cout << "������ά�Ȳ�ͬ" << endl, exit(-1);
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
				cout << "���벻�Ϸ�" << endl, exit(-1); 
			if (this->len % 10000 == 0)
				realloc(v, 10000 + this->len);
			temp = stof(p + pos, &tpos);
			this->v[len++] = temp;
			pos += tpos;
			if (str[pos] == '-' || str[pos] == '+')
				cout << "���벻�Ϸ�" << endl, exit(-1);  //�����������-123-2���ж�Ϊ�Ϸ�
		}
	}
	
};
#endif

float* p, * q;
long float ans = 0;
HANDLE hMutex = NULL;//������
//�̺߳���
DWORD WINAPI cal(LPVOID lpParamter)
{
	//����һ����������
	WaitForSingleObject(hMutex, INFINITE);
	register int i, j;
	for (i = 1e7; i < 1e8; i = i + 4000)
	{
		for (j = i; j < i + 4000; ++j)
		{
			ans += p[j] * q[j];
		}
	}

	//�ͷŻ�������
	ReleaseMutex(hMutex);
	
	return 0L;//��ʾ���ص���long�͵�0

}

int main()
{
	auto startio = chrono::steady_clock::now();

	/*********��ʼ��*********/
#ifdef Big
	register int i,j;
	BigData V1, V2;
	V1.Init();
	V2.Init();
	 p = &(V1.V[100000000]),  q = &(V2.V[100000000]);
	hMutex = CreateMutex(NULL, FALSE, L"screen");
	HANDLE hThread = CreateThread(NULL, 0, cal, NULL, 0, NULL);
	CloseHandle(hThread); 
	auto start = chrono::steady_clock::now();
	cout
		<< "IO: "
		<< chrono::duration_cast<chrono::microseconds>(start - startio).count() 
		<< "΢��" << endl;
	/*********����***********/
	
	cout << "The length is: " << V1.len << endl;

	
	
	WaitForSingleObject(hMutex, INFINITE);
	for (i = 0; i < 1e8; i=i+4000)
	{
		//m = i + 4;
		for (j = i; j < i+4000; ++j)
		{
			ans += V1.V[j] * V2.V[j];
		}
		
	}
	ReleaseMutex(hMutex);

	//for (i = 0; i < 2.5e7; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	//for (; i < 5e7; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	//for (; i < 7.5e7; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	//for (; i < 1e8; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	//for (; i < 1.25e8; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	//for (; i < 1.5e8; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	//for (; i < 1.75e8; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	//for (; i < 2e8; i++)
	//{
	//	ans += V1.V[i] * V2.V[i];
	//}
	cout << "The answer is: " << ans << endl;
	

	/************************/ 

	auto end = chrono::steady_clock::now();
	cout
		<< chrono::duration_cast<chrono::microseconds>(end - start).count() << "΢�� �� "
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << "���� �� "
		<< chrono::duration_cast<chrono::seconds>(end - start).count() << "��." << endl;
	delete(V1.V);
	delete(V2.V);
#endif


#ifdef input
	Vec a, b;
	a.in();
	b.in();
	a.equal(b);
	



	auto start = chrono::steady_clock::now();
	cout
		<< "IO: "
		<<chrono::duration_cast<chrono::microseconds>(start - startio).count() << "΢��"<<endl;

	long float ans=0;
	
	for (int i = 0; i < a.len; i++)
	{
		ans += a.v[i] * b.v[i];
	}
	cout << ans << endl;;
	//cout<<DotProduct(a, b);




	auto end = chrono::steady_clock::now();

	cout
		<< chrono::duration_cast<chrono::microseconds>(end - start).count() << "΢�� �� "
		<< chrono::duration_cast<chrono::milliseconds>(end - start).count() << "���� �� "
		<< chrono::duration_cast<chrono::seconds>(end - start).count() << "��.\n";
#endif
	return 0;
}
