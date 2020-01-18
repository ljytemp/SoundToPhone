#pragma once
//vc6.0对友元支持有问题。用DEV C++可以通过 cin << ant, VC还是调用print函数
	//求多个数字的最大公约数和最小公倍数
	//作者：蚁爷风格/HungryAnt

#include<iostream>
#include<vector>

	using namespace std;

class ANT
{
public:
	ANT(const int* const a, int size):ivec(size),g1(0),g2(0)
	{
		int i;
		for(i=0; i<size; ++i)
			ivec[i] = a[i];

		g1 = fun1();
		g2 = fun2();
	};

	friend ostream& operator<<(ostream&, ANT&);
	void print()
	{
		cout << L"最大公约数:" << g1 << endl;
		cout << L"最小公倍数:" << g2 << endl;
	}
private:
	ANT();
	int fun1();    // 求最大公约数
	int fun2();    // 求最小公倍数
	int _fun1(int, int);
	int _fun2(int, int);
	vector<int> ivec;
	int g1;    // 最大公约数
	int g2;    // 最小公倍数
};

//ostream& operator<<(ostream& out, ANT& ant){
//	out << "最大公约数:" << ant.g1 << endl;
//	out << "最小公倍数:" << ant.g2 << endl;
//}

//int main(){
//	const int Size_Array = 3;
//	int a[Size_Array] = {12, 24, 48};

//	ANT ant(a, Size_Array);
//	cout << ant;
//	//ant.print();
//	return 0;
//} 