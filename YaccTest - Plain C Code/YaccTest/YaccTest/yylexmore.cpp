#include<iostream>
#include<string>
#include"yytab.h"
using namespace std;
extern string analysis(char *yytext, int n);



int Ana(char *yytext, int n){
	string s = analysis(yytext, n);
	if (s == "INT") return INT;
	else if (s == "CHAR") return CHAR;
	else if (s == "DOUBLE") return DOUBLE;
	else if (s == "ELSE") return ELSE;
	else if (s == "==") return EQU;
	else if (s == "FLOAT") return FLOAT;
	else if (s == "FOR") return FOR;
	else if (s == ">") return GREAT;
	else if (s == "IDENTIFIER") return IDENTIFIER;
	else if (s == "IF") return IF;
	else if (s == "<") return LESS;
	else if (s == "LONG") return LONG;
	else if (s == "!=") return NE;
	else if (s == "INTEGER") return NUM;
	else if (s == "SHORT") return SHORT;
	else if (s == "SIGNED") return SIGNED;
	else if (s == "UNSIGNED") return UNSIGNED;
	else if (s == "WHILE") return WHILE;
	else if (s == ",") return 30020;
	else if (s == ";") return 30023;
	else if (s == "{") return 30021;
	else if (s == "}") return 30022;
	else if (s == "*") return 30026;
	else if (s == "+") return 30024;
	else if (s == "-") return 30025;
	else if (s == "/") return 30027;
	else if (s == "=") return 30028;
	else if (s == "(") return 30018;
	else if (s == ")") return 30019;
	else return 0;
}