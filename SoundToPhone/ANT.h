#pragma once
//vc6.0����Ԫ֧�������⡣��DEV C++����ͨ�� cin << ant, VC���ǵ���print����
	//�������ֵ����Լ������С������
	//���ߣ���ү���/HungryAnt

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
		cout << L"���Լ��:" << g1 << endl;
		cout << L"��С������:" << g2 << endl;
	}
private:
	ANT();
	int fun1();    // �����Լ��
	int fun2();    // ����С������
	int _fun1(int, int);
	int _fun2(int, int);
	vector<int> ivec;
	int g1;    // ���Լ��
	int g2;    // ��С������
};

//ostream& operator<<(ostream& out, ANT& ant){
//	out << "���Լ��:" << ant.g1 << endl;
//	out << "��С������:" << ant.g2 << endl;
//}

//int main(){
//	const int Size_Array = 3;
//	int a[Size_Array] = {12, 24, 48};

//	ANT ant(a, Size_Array);
//	cout << ant;
//	//ant.print();
//	return 0;
//} 