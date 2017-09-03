/*
File:	Core.cpp
Author:	Reskip
Team:	helloMaster

Program design practice in 2017 short semester
Micro program interpreter of new language Min-c

8/29
	Lexical analyzer Developed
*/
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstdio>
#include <algorithm>
#include <string>
#include <vector>
#include <cctype>
#include "lexAnalyzer.h"
#include "head.h"

using namespace std;

int main()
{
	freopen("in.txt", "r", stdin);
	string dat;
	char tmp;
	dat = "";
	while (scanf("%c", &tmp) != EOF)
	{
		dat += tmp;
	}
	dat += '\0';
	lexAn a = lexAn(); //词法分析器对象
	auto k = a.analyze(dat);
	for (auto i : k) //循环到k结束
	{
		i.print();
	} 
	while (1);
	return 0;
}
