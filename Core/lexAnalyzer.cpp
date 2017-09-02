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

lexAn::lexAn() //构造函数
{
	return;
}

int lexAn::getCharType(char x) //确认当前单个字符的token
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
	if (x == '\\') //转义符
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

int lexAn::getTokenType(int status) //查看status
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

int firstWL = 1; //第一错误行号初始化为1
int bug = 0;
int printMess = 0;

vector<token> lexAn::analyze(string inputData) //先进入这个
{
	vector<token> tokenList; //链表
	int chartype;
	int status = CHAR_SPACE;
	int line = 1; //行号初始化为1
	string save = ""; //初始化

	for (char i : inputData)
	{
		chartype = lexAn::getCharType(i);

		switch (chartype)
		{

		case CHAR_ERROR: //发现词法错误之后
			tokenList.push_back(token(getTokenType(status), save, line));
			save = "";
			save += i; 
			status = CHAR_ERROR |1;

		case CHAR_SPACE: //当前输入是空格
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
				save += i; //读入SPACE（145行
				status = CHAR_SPACE;
			}
			break;

		case CHAR_DOT: //当前输入是小数点
			switch (status)
			{
			case (CHAR_DIVIS | 1):
				save += i;
				break;
			case CHAR_DIGIT:
				save += i;
				status = CHAR_DIGIT | 1; //当前状态为real
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_DIGIT; 
			}
			break;

		case CHAR_DIGIT: //当前输入数字
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

		case CHAR_ALPHA: //当前输入字母
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

		case CHAR_OPERA: //当前输入操作符（除了除号
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

		case CHAR_ENTER: //当前输入换行
			switch (status)
			{
			case (CHAR_DIVIS | 1):
				status = (CHAR_DIVIS | 2); //注释结束只有换行这一种可能，那就结算
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

		case CHAR_EQUAL: //当前输入'='
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

		case CHAR_LARGE: //当前输入'>'
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

		case CHAR_SMALL: //当前输入'<'
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

		case CHAR_DIVIS: //当前输入除号
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				break;
			case CHAR_DIVIS:
				save += i;
				status = (CHAR_DIVIS | 1); //是注释！
				break;
			default:
				tokenList.push_back(token(getTokenType(status), save, line));
				save = "";
				save += i;
				status = CHAR_DIVIS;
			}
			break;

		case CHAR_ESCAP: //当前输入转义符'\'
			switch (status)
			{
			case (CHAR_DIVIS | 1):
			case CHAR_QUOTE:
				save += i;
				status = CHAR_ESCAP; //引号中的转义符
				break;
			case CHAR_ESCAP:
				save += i;
				status = CHAR_QUOTE;//这里已经在上一次中排除不是在引号里的爆炸边
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

token::token(int type, string val, int line) //token的构造函数
{
	this->type = type;
	this->val = val;
	this->line = line;
	return;
}

void token::print() //根据类型染色
{
	if (COLOR) //是否执行染色？（宏定义中修改
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
	
	setColor(COLOR_GREEN); //对检查部分的染色
	//cout << "[" << line << "]"; //用于检查line是否正确
	cout << "[" << type << "]"; //用于检查type是否正确

	return;
}

void token::trans() //从变量里面挑出保留字
{
	if (this->type == TYPE_VAR && reserved.find(this->val) != reserved.end()) //对比变量和保留字的文本
	{
		this->type = TYPE_KEY; //是保留字
	}
	return;
}
