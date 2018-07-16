#include<iostream>
#include<map>
#include<stack>
#include<fstream>
#include<string>
#define E 40000
#define ACCEPT 0
#define SOURCE_END 40001
union SV
{

	int ival;
};
SV u[10];
SV yyval;
using namespace std;
extern int Ana(char *yytext,int n);
struct Sym
{
	int symbol;
	int state;
};
map<int,int> symbolTable;
int producerN[40]={1,6,5,1,1,1,1,1,1,1,1,1,1,4,2,3,1,2,1,1,1,2,2,3,1,1,1,1,2,4,4,4,3,5,3,1,1,1,1,5};
int pLeftSection[40]={34999,35000,35000,35001,35001,35001,35001,35001,35001,35001,35001,35002,35005,35003,35003,35004,35006,35006,35007,35007,35007,35008,35008,35011,35013,35013,35013,35013,35012,35012,35012,35012,35012,35009,35014,35015,35015,35015,35015,35010};
ifstream fin;
ifstream producerfile;
int actionTable[80][46]=
		{		{E,E,E,E,E,1,2,3,4,5,6,7,8,E,E,E,E,E,9,10,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-3,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-4,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-5,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-6,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-7,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-8,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-9,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-10,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,0},
		{11,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,12,E,E,E,E,13,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,14,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-11,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,1,2,3,4,5,6,7,8,E,E,E,E,E,E,16,E,E,17,15,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,19,E,E,18,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{20,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,21,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,22,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{23,E,24,25,E,1,2,3,4,5,6,7,8,E,E,E,E,E,E,26,E,E,E,E,E,27,E,E,28,E,29,30,31,32,33,E,34,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-2},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,E,E,-12,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-14,E,E,35,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,36,E,E,18,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,-12,-12,-12,-12,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,37,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,38,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{39,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,40,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,46,41,42,43,44,45,E,E,E},
		{23,E,24,25,E,1,2,3,4,5,6,7,8,E,E,E,E,E,E,26,E,E,E,E,E,27,E,E,E,47,48,30,31,32,33,E,34,E,E,E,E,E,E,E,E,E},
		{-16,E,-16,-16,E,-16,-16,-16,-16,-16,-16,-16,-16,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-16,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-18,E,-18,-18,E,-18,-18,-18,-18,-18,-18,-18,-18,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-18,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-19,E,-19,-19,E,-19,-19,-19,-19,-19,-19,-19,-19,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-19,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-20,E,-20,-20,E,-20,-20,-20,-20,-20,-20,-20,-20,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-20,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,49,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,50,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,1,2,3,4,5,6,7,8,E,E,E,E,E,E,16,E,E,51,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-1},
		{52,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,53,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,54,E,E},
		{52,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,53,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,55,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,E,E,E,E,E,E,-12,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-28,E,E,E,E,E,E,56,E,E,E},
		{-24,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-25,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-26,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-27,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{57,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,58,E,E,E,E,E,E,E,E,59,E,E,E,E,E,E,E,E,E,E,E},
		{60,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,61,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-15},
		{-17,E,-17,-17,E,-17,-17,-17,-17,-17,-17,-17,-17,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-17,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-21,E,-21,-21,E,-21,-21,-21,-21,-21,-21,-21,-21,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-21,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-22,E,-22,-22,E,-22,-22,-22,-22,-22,-22,-22,-22,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-22,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-13,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,-12,-12,-12,-12,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,62,63,64,65,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,66,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,67,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,68,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{69,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,70,E,E,E,E,E,E,E,71,E,E,E,E,E,E,E,E,72,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,-12,-12,-12,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,46,41,42,43,44,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-32,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-23,E,E,E,E,E,E,E,E,E,E},
		{-35,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-36,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-37,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-38,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{73,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,74,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,76,E,E,75,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,77,E,E,75,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,E,E,-12,-12,-12,-12,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-29,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-30,E,46,41,42,43,44,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-31,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-12,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-34,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{23,E,24,25,E,1,2,3,4,5,6,7,8,E,E,E,E,E,E,26,E,E,E,E,E,27,E,E,78,E,29,30,31,32,33,E,34,E,E,E,E,E,E,E,E,E},
		{-39,E,-39,-39,E,-39,-39,-39,-39,-39,-39,-39,-39,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-39,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{-33,E,-33,-33,E,-33,-33,-33,-33,-33,-33,-33,-33,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-33,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E},
		{23,E,24,25,E,1,2,3,4,5,6,7,8,E,E,E,E,E,E,26,E,E,E,E,E,27,E,E,E,79,48,30,31,32,33,E,34,E,E,E,E,E,E,E,E,E},
		{-15,E,-15,-15,E,-15,-15,-15,-15,-15,-15,-15,-15,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,-15,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E,E}};
int predictTable(int cstate,char symbol)
{
	return actionTable[cstate][symbol];
}

int readToken()
{
	if(fin.eof())
		return SOURCE_END;
	char buf[256];//不支持超过256个字符的符号
	int pos=0;
	bool isEnd=false;
	bool isToken=false;//与isOperator互斥
	bool isOperator=false;
	string ops("!@#$%^&*()+-=|[]{};':\",.<>/?");
	string wss("\t\n\r ");
	string letter("_0123456789aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ");
	char c=fin.get();
	while(c==' '||c=='\t'||c=='\n'||c=='\r') c=fin.get();//滤掉空白符
	while(!isEnd)
	{
		if(c==-1)
		{
			buf[pos]='\0';
			isEnd=true;
			continue;
		}		if(ops.find(c)>=0&&ops.find(c)<ops.size())//表示当c不是正常字符时。
		{
			//进入非正常字符的处理过程
			if(isToken)
			{
				fin.unget();
				buf[pos]='\0';
				isEnd=true;
			}
			else
			{
				isOperator=true;
				buf[pos]=c;
				pos++;
				c=fin.get();
			}
			continue;
		}
		if(letter.find(c)>=0&&letter.find(c)<letter.size())
		{
			if(isOperator)
			{
				fin.unget();
				buf[pos]='\0';
				isEnd=true;
			}
			else
			{
				isToken=true;
				buf[pos]=c;
				pos++;
				c=fin.get();
			}
			continue;
		}
		if(wss.find(c)>=0&&wss.find(c)<wss.size())
		{
			buf[pos]='\0';
			isEnd=true;
		}
		
		
		else return -1;
	}
	return Ana(buf,strlen(buf));
}
void runaction(int num)
{
	switch(num)
	{
	}
}
void getvalue(int symbol,SV & val)
{
	switch(symbol)
	{
	case 30000:val.ival=30000;break;
	case 30001:val.ival=30001;break;
	case 30002:val.ival=30002;break;
	case 30003:val.ival=30003;break;
	case 30004:val.ival=30004;break;
	case 30005:val.ival=30005;break;
	case 30006:val.ival=30006;break;
	case 30007:val.ival=30007;break;
	case 30008:val.ival=30008;break;
	case 30009:val.ival=30009;break;
	case 30010:val.ival=30010;break;
	case 30011:val.ival=30011;break;
	case 30012:val.ival=30012;break;
	case 30013:val.ival=30013;break;
	case 30014:val.ival=30014;break;
	case 30015:val.ival=30015;break;
	case 30016:val.ival=30016;break;
	case 30017:val.ival=30017;break;
	}
}
int parse()
{
	int inputsymbol=0;
	int cstate=0;
	stack<Sym> symStack;//符号栈
	stack<SV> valStack;//语义值栈
	Sym st;//用作分析时的临时栈顶元素存储变量
	st.symbol=0;
	st.state=0;
	SV val;
	symStack.push(st);//语义值栈必须要和符号栈同步
	valStack.push(val);
	inputsymbol=readToken();
	while(1)
	{
		st=symStack.top();
		int col=symbolTable[inputsymbol];
		int result=predictTable(st.state,col);
		if(result==E)//出错
		{
			cout<<"不符合yacc语法规范，编译出错!"<<endl;
			return 0;
		}
		if(result==ACCEPT)
		{
			cout<<"编译成功!"<<endl;
			return 1;
		}
		if(result<0)//负数表示为归约项目
		{
			result*=-1;
			char p[256]="";
			producerfile.open("producer.txt");
			for(int i=0;i<=result;i++)
			{
				producerfile.getline(p, 256);
			}
			cout<<p<<endl;
			producerfile.close();
			int n=producerN[result];//取得该号产生式右部符号数量，以作弹栈用
			for(int i=0;i<n;i++)
			{
				symStack.pop();
				u[n-i]=valStack.top();
				valStack.pop();
			}
			runaction(result);//执行语义动作
			//再将产生式左部的符号压栈，语义值一同压栈
			st.symbol=pLeftSection[result];
			st.state=predictTable(symStack.top().state,symbolTable[st.symbol]);
			symStack.push(st);
			//将产生式左部符号的语义值入栈。这个值在runaction()中已经修改
			valStack.push(u[0]);
		}
		else
		{
			st.symbol=inputsymbol;
			st.state=result;
			symStack.push(st);
			SV tu;
			if(yyval.ival==0)
				getvalue(inputsymbol,tu);
			else tu=yyval;
			valStack.push(tu);
			inputsymbol=readToken();
		}
	}
}
void main()
{
	pair<int,int> tp;
	tp.first=40001;
	tp.second=45;
	symbolTable.insert(tp);
	tp.first=30018;
	tp.second=21;
	symbolTable.insert(tp);
	tp.first=30019;
	tp.second=23;
	symbolTable.insert(tp);
	tp.first=30026;
	tp.second=40;
	symbolTable.insert(tp);
	tp.first=30024;
	tp.second=38;
	symbolTable.insert(tp);
	tp.first=30020;
	tp.second=26;
	symbolTable.insert(tp);
	tp.first=30025;
	tp.second=39;
	symbolTable.insert(tp);
	tp.first=30027;
	tp.second=41;
	symbolTable.insert(tp);
	tp.first=30023;
	tp.second=35;
	symbolTable.insert(tp);
	tp.first=30028;
	tp.second=42;
	symbolTable.insert(tp);
	tp.first=30009;
	tp.second=9;
	symbolTable.insert(tp);
	tp.first=30010;
	tp.second=10;
	symbolTable.insert(tp);
	tp.first=30004;
	tp.second=4;
	symbolTable.insert(tp);
	tp.first=30013;
	tp.second=13;
	symbolTable.insert(tp);
	tp.first=30006;
	tp.second=6;
	symbolTable.insert(tp);
	tp.first=30001;
	tp.second=1;
	symbolTable.insert(tp);
	tp.first=30015;
	tp.second=15;
	symbolTable.insert(tp);
	tp.first=30000;
	tp.second=0;
	symbolTable.insert(tp);
	tp.first=30003;
	tp.second=3;
	symbolTable.insert(tp);
	tp.first=30005;
	tp.second=5;
	symbolTable.insert(tp);
	tp.first=30014;
	tp.second=14;
	symbolTable.insert(tp);
	tp.first=30007;
	tp.second=7;
	symbolTable.insert(tp);
	tp.first=30016;
	tp.second=16;
	symbolTable.insert(tp);
	tp.first=30017;
	tp.second=17;
	symbolTable.insert(tp);
	tp.first=30008;
	tp.second=8;
	symbolTable.insert(tp);
	tp.first=30011;
	tp.second=11;
	symbolTable.insert(tp);
	tp.first=30012;
	tp.second=12;
	symbolTable.insert(tp);
	tp.first=30002;
	tp.second=2;
	symbolTable.insert(tp);
	tp.first=30021;
	tp.second=27;
	symbolTable.insert(tp);
	tp.first=30022;
	tp.second=29;
	symbolTable.insert(tp);
	tp.first=35000;
	tp.second=18;
	symbolTable.insert(tp);
	tp.first=35001;
	tp.second=19;
	symbolTable.insert(tp);
	tp.first=35002;
	tp.second=20;
	symbolTable.insert(tp);
	tp.first=35003;
	tp.second=22;
	symbolTable.insert(tp);
	tp.first=35004;
	tp.second=24;
	symbolTable.insert(tp);
	tp.first=35005;
	tp.second=25;
	symbolTable.insert(tp);
	tp.first=35006;
	tp.second=28;
	symbolTable.insert(tp);
	tp.first=35007;
	tp.second=30;
	symbolTable.insert(tp);
	tp.first=35008;
	tp.second=31;
	symbolTable.insert(tp);
	tp.first=35009;
	tp.second=32;
	symbolTable.insert(tp);
	tp.first=35010;
	tp.second=33;
	symbolTable.insert(tp);
	tp.first=35011;
	tp.second=34;
	symbolTable.insert(tp);
	tp.first=35012;
	tp.second=36;
	symbolTable.insert(tp);
	tp.first=35013;
	tp.second=37;
	symbolTable.insert(tp);
	tp.first=35014;
	tp.second=43;
	symbolTable.insert(tp);
	tp.first=35015;
	tp.second=44;
	symbolTable.insert(tp);
	string filename;
	cout<<"请输入编写的测试文件名:"<<endl;
	cin>>filename;
	fin.open(filename.c_str());
	if(fin.fail())
	{
		cout<<"无法打开文件 "<<filename<<endl;
		return;
	}
	parse();
	system("pause");
}
