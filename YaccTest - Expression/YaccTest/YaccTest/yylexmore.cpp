#include<iostream>
#include<string>
#include"yytab.h"
using namespace std;
extern string analysis(char *yytext, int n);



int Ana(char *yytext, int n){
	string s = analysis(yytext, n);
	if (s == "INTEGER") return DIGIT;
	else if (s == "*") return 30003;
	else if (s == "+") return 30001;
	else if (s == "-") return 30002;
	else if (s == "/") return 30004;
	else return 0;
}