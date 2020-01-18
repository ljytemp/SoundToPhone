#include "StdAfx.h"
#include "ANT.h"


int ANT::_fun1(int a, int b){
	int r;
	while(b!=0)
	{
		r = a%b;
		a = b;
		b = r;
	}
	return a;
}

int ANT::_fun2(int a, int b){
	int n = a*b;
	int r;
	while(b!=0)
	{
		r = a%b;
		a = b;
		b = r;
	}
	return n/a;
}


int ANT::fun1(){
	int size = ivec.size();
	int a,b;
	a = ivec[0];

	int i = 1;
	for(; i<size; ++i){
		b = ivec[i];
		a = _fun1(a, b);
	}
	return a;
}

int ANT::fun2(){
	int size = ivec.size();
	int a,b;
	a = ivec[0];

	int i = 1;
	for(; i<size; ++i){
		b = ivec[i];
		a = _fun2(a, b);
	}
	return a;
}
