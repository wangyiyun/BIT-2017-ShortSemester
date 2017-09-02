#pragma once
#include <Windows.h>

#define DEBUG 0
#define COLOR 1

#define TYPE_KEY 0	//keywords
#define TYPE_VAR 1	//variable
#define TYPE_CON 2	//constant
#define TYPE_OPR 3	//operator
#define TYPE_DEL 4	//delimiter
#define TYPE_FMT 5	//format
#define TYPE_ERR 6	//error
#define TYPE_NOTE 7 //note

#define CHAR_TYPE_NUM 12	//当前有几个状态
#define CHAR_SPACE (0<<4)	//space
#define CHAR_DIGIT (1<<4)	//digit
#define CHAR_ALPHA (2<<4)	//alpha
#define CHAR_OPERA (3<<4)	//operator
#define CHAR_ENTER (4<<4)	//enter	'\n'
#define CHAR_EQUAL (5<<4)	//equal	'='
#define CHAR_LARGE (6<<4)	//large	'>'
#define CHAR_SMALL (7<<4)	//small	'<'
#define CHAR_DIVIS (8<<4)	//division '/'
#define CHAR_ESCAP (9<<4)	//escape '\\'
#define CHAR_QUOTE (10<<4)	//quote	'\"'
#define CHAR_DOT (11<<4)	//dot '.'
#define CHAR_ERROR (13<<4)	//ERROR