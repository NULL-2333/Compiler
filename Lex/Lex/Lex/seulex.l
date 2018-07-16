%{
#include <iostream>
#include <string>
#include <stdio.h>

FILE * yyin;
FILE * yyout;
%}
number	[0-9]
letter	[a-zA-Z]
id	{letter}+
integer	{number}*
%%
void		return "VOID";
int	    	return "INT";
char		return "CHAR";
double  	return "DOUBLE";
switch  	return "SWITCH";
break		return "BREAK";
case		return "CASE";
do      	return "DO";
while		return "WHILE";
for 		return "FOR";
continue	return "CONTINUE";
if   		return "IF";
else    	return "ELSE";
return  	return "RETURN";
"{"			return "{";
"}"			return "}";
"("			return "(";
")"			return ")";
"["			return "[";
"]"			return "]";
"+"			return "+";
"-"			return "-";
"<"			return "<";
">"			return ">";
"*"			return "*";
"^"			return "^";
"/"			return "/";
"="			return "=";
"=="		return "==";
"!"			return "!";
";"			return ";";
{id}		return "IDENTIFIER";
{integer}	return "INTEGER";
%%

bool isChaDig(char c)
{
	if ((c >= 'a'&&c <= 'z' )||( c >= 'A'&&c <= 'Z' )|| (c >= '0'&&c <= '9'))
		return true;
	else return false;
}


int main()
{
	printf("please input the test program in input.txt\n");
	system("input.txt");
	yyin = fopen("input.txt", "r");
	yyout = fopen("output.txt", "w");
	char yy[20];
	yy[0] = '\n';
	int length=0;
	char be='a';
	char c;
	while (fscanf(yyin,"%c", &c) != EOF)
	{
		if (isChaDig(be) != isChaDig(c) || (!isChaDig(c) && !isChaDig(be)&&c!=be)||  (c == '\('&&be == '\(') || (c == '\)'&& be == '\)'))
		{
			if (yy[0] != '\n')
			{
				for (int i = 0; i < length; i++)
					fputc(yy[i], yyout);
				fputc('\t', yyout);
				string result = analysis(yy, length);
				for (int i = 0; i < result.size(); i++)
					fputc(result[i], yyout);
				fputc('\n', yyout);
			}			
			if (c != '\t'&&c!=' ')
			{
				yy[0] = c;
				length=1;
			}
			else
			{
				yy[0] = '\n';
			}
		}
		else
		{
			yy[length]= c;
			length++;
		}
		be = c;
	}
	for (int i = 0; i < length; i++)
		fputc(yy[i], yyout);
	fputc('\t', yyout);
	string result = analysis(yy, length);
	for (int i = 0; i < result.size();i++)
		fputc(result[i], yyout);
	fputc('\n', yyout);
	fclose(yyin);
	fclose(yyout);
	system("output.txt");
	return 0;
}