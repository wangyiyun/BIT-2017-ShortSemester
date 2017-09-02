#pragma once
#include <string>
#include <iostream>
#include <vector>
#include "head.h"

using namespace std;

class token
{
	int type;
	string val;
	int line;
public:
	token(int type, string val, int line);
	void print();
	void trans();
};

static class lexAn
{
public:
	lexAn();
	int getCharType(char x);
	int getTokenType(int status);
	vector<token> analyze(string inputData);
};