#include <cctype>
#include <set>
#include "lexAnalyzer.h"
#include "conColor.h"

using namespace std;

int typeColor[] =
{
	COLOR_BLUE,
	COLOR_LIGHT,
	COLOR_GREY,
	COLOR_CYAN,
	COLOR_WHITE,
	COLOR_WHITE,
	COLOR_RED,
	COLOR_GREEN
};
//#define TYPE_KEY 0
//#define TYPE_VAR 1
//#define TYPE_CON 2
//#define TYPE_OPR 3
//#define TYPE_DEL 4
//#define TYPE_FMT 5
//#define TYPE_ERR 6
//#define TYPE_NOTE 7

set<string>reserved = { "int", "real", "string", "while", "if", "else",
		"out", "in", "return" };

lexAn::lexAn() //���캯��
{
	return;
}

int lexAn::getCharType(char x) //ȷ�ϵ�ǰ�����ַ���token
{
	if (isdigit(x))
	{
		return CHAR_DIGIT;
	}
	if (isalpha(x))
	{
		return CHAR_ALPHA;
	}
	if (x == '.')
	{
		return CHAR_DOT;
	}
	if (x == '=')
	{
		return CHAR_EQUAL;
	}
	if (x == '>')
	{
		return CHAR_LARGE;
	}
	if (x == '<')
	{
		return CHAR_SMALL;
	}
	if (x == '\\') //ת���
	{
		return CHAR_ESCAP;
	}
	if ( x == '\n' || x == '\0')
	{
		return CHAR_ENTER;
	}
	if (x == '\t' || x == ' ')
	{
		return CHAR_SPACE;
	}
	if (x == '"')
	{
		return CHAR_QUOTE;
	}
	if (x == '/')
	{
		return CHAR_DIVIS;
	}

	if (x == '.') //real
	{
		return CHAR_DIGIT | 1;
	}


	if (DEBUG)
	{
		cout << "DEBUG: char\'" << x << "\' classificated as type \"operate\"\n";
	}
	return CHAR_OPERA;
	return 0;
}

int lexAn::getTokenType(int status) //�鿴status
{
	switch (status)
	{
	case CHAR_ERROR:
			return TYPE_ERR;
	case CHAR_ERROR | 1:
		return TYPE_NOTE;
	case CHAR_DIVIS | 2:	//note
		return TYPE_NOTE;
	case CHAR_SPACE:		//' ',  '\t'
		return TYPE_FMT;
	case CHAR_DIGIT:		//integer
	case CHAR_DIGIT | 1:	//real
	case CHAR_QUOTE | 1:	//string
		return TYPE_CON;
	case CHAR_ALPHA:		//variable
		return TYPE_VAR;
	case CHAR_OPERA:		//operator
	case CHAR_EQUAL:		//'='
	case CHAR_EQUAL | 1:	//"=="
	case CHAR_EQUAL | 2:	//"<="
	case CHAR_EQUAL | 3:	//">="
	case CHAR_ESCAP:		//'\\'
		return TYPE_CON;
	case CHAR_SMALL:		//'<'
	case CHAR_SMALL | 1:	//'<>'
	case CHAR_LARGE:		//'>'
	case CHAR_DIVIS:		//'/'
		return TYPE_OPR;
	}
	return 0;
}

int firstWL = 1; //��һ�����кų�ʼ��Ϊ1
int bug = 0;
int printMess = 0;

vector<token> lexAn::analyze(string inputData) //�Ƚ������
{
	vector<token> tokenList; //����
	int chartype;
	int status = CHAR_SPACE;
	int line = 1; //�кų�ʼ��Ϊ1
	string save = ""; //��ʼ��

	for (char i : inputData)
	{
		chartype = lexAn::getCharType(i);

		switch (chartype)
		{

		case CHAR_ERROR: //���ִʷ�����֮��
			tokenList.push_back(token(getTokenType(status), save, line));
			save = "";
			save += i; 
			status = CHAR_ERROR |1;

		case CHAR_SPACE: //��ǰ�����ǿո�
			switch (status)
			{
			case CHAR_ENTER:
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			case CHAR_DIVIS:
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i; //����SPACE��145��
				status = CHAR_SPACE;
			}
			break;

		case CHAR_DOT: //��ǰ������С����
			switch (status)
			{
			case (CHAR_DIVIS | 1):
				save += i;
				break;
			case CHAR_DIGIT:
				save += i;
				status = CHAR_DIGIT | 1; //��ǰ״̬Ϊreal
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_DIGIT; 
			}
			break;

		case CHAR_DIGIT: //��ǰ��������
			switch (status)
			{
			case CHAR_DIGIT:
			case (CHAR_DIGIT | 1): //real
			case CHAR_ALPHA: //variable
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;

			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_DIGIT;
			}
			break;

		case CHAR_ALPHA: //��ǰ������ĸ
			switch (status)
			{
			case CHAR_ALPHA:
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_ALPHA;
			}
			break;

		case CHAR_OPERA: //��ǰ��������������˳���
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_OPERA;
			}
			break;

		case CHAR_ENTER: //��ǰ���뻻��
			switch (status)
			{
			case (CHAR_DIVIS | 1):
				status = (CHAR_DIVIS | 2); //ע�ͽ���ֻ�л�����һ�ֿ��ܣ��Ǿͽ���
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_ENTER;
				break;
			}
			line++;
			break;

		case CHAR_EQUAL: //��ǰ����'='
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			case CHAR_EQUAL:
				save += i;
				status = (CHAR_EQUAL | 1);
				break;
			case CHAR_SMALL:
				save += i;
				status = (CHAR_EQUAL | 2);
				break;
			case CHAR_LARGE:
				save += i;
				status = (CHAR_EQUAL | 3);
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_EQUAL;
			}
			break;

		case CHAR_LARGE: //��ǰ����'>'
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			case CHAR_SMALL:
				save += i;
				status = (CHAR_SMALL | 1);
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_LARGE;
			}
			break;

		case CHAR_SMALL: //��ǰ����'<'
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_SMALL;
			}
			break;

		case CHAR_DIVIS: //��ǰ�������
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			case CHAR_DIVIS:
				save += i;
				status = (CHAR_DIVIS | 1); //��ע�ͣ�
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_DIVIS;
			}
			break;

		case CHAR_ESCAP: //��ǰ����ת���'\'
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				status = CHAR_ESCAP; //�����е�ת���
				break;
			case CHAR_ESCAP:
				save += i;
				status = CHAR_QUOTE;//�����Ѿ�����һ�����ų�������������ı�ը��
				break;
			default:
				if (!bug)
				{
					firstWL = line;
					bug = 1;
				}
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_ERROR;
				break;
			}
			break;

		case CHAR_QUOTE:
			switch (status)
			{
			case (CHAR_DIVIS | 1):
				save += i;
				status = (CHAR_DIVIS | 1);
				break;
			case CHAR_QUOTE:
				save += i;
				status = (CHAR_QUOTE | 1);
				break;
			case CHAR_ESCAP:
				save += i;
				status = CHAR_QUOTE;
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_QUOTE;
			}
			break;
		}
	}
	for (auto i = tokenList.begin();i != tokenList.end();i++)
	{
		i->trans();
	}
	return tokenList;
}

token::token(int type, string val, int line) //token�Ĺ��캯��
{
	this->type = type;
	this->val = val;
	this->line = line;
	return;
}

void token::print() //��������Ⱦɫ
{
	if (COLOR) //�Ƿ�ִ��Ⱦɫ�����궨�����޸�
	{
		setColor(typeColor[type]);
	}
	else setColor(COLOR_LIGHT);

	if (bug  && line == firstWL && !printMess)
	{
		setColor(typeColor[TYPE_ERR]);
		cout << "\n" << "ERROR in line: " << firstWL ;
		printMess = 1;
	}

	cout << val;
	
	setColor(COLOR_GREEN); //�Լ�鲿�ֵ�Ⱦɫ
	//cout << "[" << line << "]"; //���ڼ��line�Ƿ���ȷ
	cout << "[" << type << "]"; //���ڼ��type�Ƿ���ȷ

	return;
}

void token::trans() //�ӱ�����������������
{
	if (this->type == TYPE_VAR && reserved.find(this->val) != reserved.end()) //�Աȱ����ͱ����ֵ��ı�
	{
		this->type = TYPE_KEY; //�Ǳ�����
	}
	return;
}
