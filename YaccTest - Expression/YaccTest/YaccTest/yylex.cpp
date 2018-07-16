#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string>
#include <stdio.h>
#include"yytab.h"""
extern SV yyval;
FILE * yyin;
FILE * yyout;
using namespace std;
const int START = 0;
const int ERROR = 32767;
string analysis(char *yytext, int n)
{
	int state = START;
	int N = n + 1;
	for (int i = 0; i<N; i++)
	{
		switch (state)
		{
		case 0:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			if (yytext[i] == 'v')
			{
				state = 1;
				break;
			}
			if (yytext[i] == 'i')
			{
				state = 2;
				break;
			}
			if (yytext[i] == 'c')
			{
				state = 3;
				break;
			}
			if (yytext[i] == 'd')
			{
				state = 4;
				break;
			}
			if (yytext[i] == 's')
			{
				state = 5;
				break;
			}
			if (yytext[i] == 'b')
			{
				state = 6;
				break;
			}
			if (yytext[i] == 'w')
			{
				state = 7;
				break;
			}
			if (yytext[i] == 'f')
			{
				state = 8;
				break;
			}
			if (yytext[i] == 'e')
			{
				state = 9;
				break;
			}
			if (yytext[i] == 'r')
			{
				state = 10;
				break;
			}
			if (yytext[i] == '{')
			{
				state = 11;
				break;
			}
			if (yytext[i] == '}')
			{
				state = 12;
				break;
			}
			if (yytext[i] == '(')
			{
				state = 13;
				break;
			}
			if (yytext[i] == ')')
			{
				state = 14;
				break;
			}
			if (yytext[i] == '[')
			{
				state = 15;
				break;
			}
			if (yytext[i] == ']')
			{
				state = 16;
				break;
			}
			if (yytext[i] == '+')
			{
				state = 17;
				break;
			}
			if (yytext[i] == '-')
			{
				state = 18;
				break;
			}
			if (yytext[i] == '<')
			{
				state = 19;
				break;
			}
			if (yytext[i] == '>')
			{
				state = 20;
				break;
			}
			if (yytext[i] == '*')
			{
				state = 21;
				break;
			}
			if (yytext[i] == '^')
			{
				state = 22;
				break;
			}
			if (yytext[i] == '/')
			{
				state = 23;
				break;
			}
			if (yytext[i] == '=')
			{
				state = 24;
				break;
			}
			if (yytext[i] == '!')
			{
				state = 25;
				break;
			}
			if (yytext[i] == ';')
			{
				state = 26;
				break;
			}
			if (yytext[i] == 'a')
			{
				state = 27;
				break;
			}
			if (yytext[i] == 'g')
			{
				state = 28;
				break;
			}
			if (yytext[i] == 'h')
			{
				state = 29;
				break;
			}
			if (yytext[i] == 'j')
			{
				state = 30;
				break;
			}
			if (yytext[i] == 'k')
			{
				state = 31;
				break;
			}
			if (yytext[i] == 'l')
			{
				state = 32;
				break;
			}
			if (yytext[i] == 'm')
			{
				state = 33;
				break;
			}
			if (yytext[i] == 'n')
			{
				state = 34;
				break;
			}
			if (yytext[i] == 'o')
			{
				state = 35;
				break;
			}
			if (yytext[i] == 'p')
			{
				state = 36;
				break;
			}
			if (yytext[i] == 'q')
			{
				state = 37;
				break;
			}
			if (yytext[i] == 't')
			{
				state = 38;
				break;
			}
			if (yytext[i] == 'u')
			{
				state = 39;
				break;
			}
			if (yytext[i] == 'x')
			{
				state = 40;
				break;
			}
			if (yytext[i] == 'y')
			{
				state = 41;
				break;
			}
			if (yytext[i] == 'z')
			{
				state = 42;
				break;
			}
			if (yytext[i] == 'A')
			{
				state = 43;
				break;
			}
			if (yytext[i] == 'B')
			{
				state = 44;
				break;
			}
			if (yytext[i] == 'C')
			{
				state = 45;
				break;
			}
			if (yytext[i] == 'D')
			{
				state = 46;
				break;
			}
			if (yytext[i] == 'E')
			{
				state = 47;
				break;
			}
			if (yytext[i] == 'F')
			{
				state = 48;
				break;
			}
			if (yytext[i] == 'G')
			{
				state = 49;
				break;
			}
			if (yytext[i] == 'H')
			{
				state = 50;
				break;
			}
			if (yytext[i] == 'I')
			{
				state = 51;
				break;
			}
			if (yytext[i] == 'J')
			{
				state = 52;
				break;
			}
			if (yytext[i] == 'K')
			{
				state = 53;
				break;
			}
			if (yytext[i] == 'L')
			{
				state = 54;
				break;
			}
			if (yytext[i] == 'M')
			{
				state = 55;
				break;
			}
			if (yytext[i] == 'N')
			{
				state = 56;
				break;
			}
			if (yytext[i] == 'O')
			{
				state = 57;
				break;
			}
			if (yytext[i] == 'P')
			{
				state = 58;
				break;
			}
			if (yytext[i] == 'Q')
			{
				state = 59;
				break;
			}
			if (yytext[i] == 'R')
			{
				state = 60;
				break;
			}
			if (yytext[i] == 'S')
			{
				state = 61;
				break;
			}
			if (yytext[i] == 'T')
			{
				state = 62;
				break;
			}
			if (yytext[i] == 'U')
			{
				state = 63;
				break;
			}
			if (yytext[i] == 'V')
			{
				state = 64;
				break;
			}
			if (yytext[i] == 'W')
			{
				state = 65;
				break;
			}
			if (yytext[i] == 'X')
			{
				state = 66;
				break;
			}
			if (yytext[i] == 'Y')
			{
				state = 67;
				break;
			}
			if (yytext[i] == 'Z')
			{
				state = 68;
				break;
			}
			if (yytext[i] == '0')
			{
				state = 69;
				break;
			}
			if (yytext[i] == '1')
			{
				state = 70;
				break;
			}
			if (yytext[i] == '2')
			{
				state = 71;
				break;
			}
			if (yytext[i] == '3')
			{
				state = 72;
				break;
			}
			if (yytext[i] == '4')
			{
				state = 73;
				break;
			}
			if (yytext[i] == '5')
			{
				state = 74;
				break;
			}
			if (yytext[i] == '6')
			{
				state = 75;
				break;
			}
			if (yytext[i] == '7')
			{
				state = 76;
				break;
			}
			if (yytext[i] == '8')
			{
				state = 77;
				break;
			}
			if (yytext[i] == '9')
			{
				state = 78;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 1:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'o')
			{
				state = 79;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 2:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'n')
			{
				state = 80;
				break;
			}
			if (yytext[i] == 'f')
			{
				state = 81;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 3:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'h')
			{
				state = 82;
				break;
			}
			if (yytext[i] == 'a')
			{
				state = 83;
				break;
			}
			if (yytext[i] == 'o')
			{
				state = 84;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 4:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'o')
			{
				state = 85;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 5:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'w')
			{
				state = 86;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 6:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'r')
			{
				state = 87;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 7:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'h')
			{
				state = 88;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 8:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'o')
			{
				state = 89;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 9:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'l')
			{
				state = 90;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 10:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'e')
			{
				state = 91;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 11:
		{
			if (i == N - 1)
			{
				return "{";
				break;
			}
			break;
		}
		case 12:
		{
			if (i == N - 1)
			{
				return "}";
				break;
			}
			break;
		}
		case 13:
		{
			if (i == N - 1)
			{
				return "(";
				break;
			}
			break;
		}
		case 14:
		{
			if (i == N - 1)
			{
				return ")";
				break;
			}
			break;
		}
		case 15:
		{
			if (i == N - 1)
			{
				return "[";
				break;
			}
			break;
		}
		case 16:
		{
			if (i == N - 1)
			{
				return "]";
				break;
			}
			break;
		}
		case 17:
		{
			if (i == N - 1)
			{
				return "+";
				break;
			}
			break;
		}
		case 18:
		{
			if (i == N - 1)
			{
				return "-";
				break;
			}
			break;
		}
		case 19:
		{
			if (i == N - 1)
			{
				return "<";
				break;
			}
			break;
		}
		case 20:
		{
			if (i == N - 1)
			{
				return ">";
				break;
			}
			break;
		}
		case 21:
		{
			if (i == N - 1)
			{
				return "*";
				break;
			}
			break;
		}
		case 22:
		{
			if (i == N - 1)
			{
				return "^";
				break;
			}
			break;
		}
		case 23:
		{
			if (i == N - 1)
			{
				return "/";
				break;
			}
			break;
		}
		case 24:
		{
			if (i == N - 1)
			{
				return "=";
				break;
			}
			break;
		}
		case 25:
		{
			if (i == N - 1)
			{
				return "!";
				break;
			}
			break;
		}
		case 26:
		{
			if (i == N - 1)
			{
				return ";";
				break;
			}
			break;
		}
		case 27:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 28:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 29:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 30:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 31:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 32:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 33:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 34:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 35:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 36:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 37:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 38:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 39:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 40:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 41:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 42:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 43:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 44:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 45:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 46:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 47:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 48:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 49:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 50:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 51:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 52:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 53:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 54:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 55:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 56:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 57:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 58:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 59:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 60:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 61:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 62:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 63:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 64:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 65:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 66:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 67:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			break;
		}
		case 68:
		{
			if (i == N - 1)
			{
				return "IDENTIFIER";
				break;
			}
			if (yytext[i] == 'Z')
			{
				state = 68;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 69:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 70:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 71:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 72:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 73:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 74:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 75:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 76:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 77:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			break;
		}
		case 78:
		{
			if (i == N - 1)
			{
				yyval.ival = atof(yytext);
				return "INTEGER";
				break;
			}
			if (yytext[i] == '9')
			{
				state = 78;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 79:
		{
			if (yytext[i] == 'i')
			{
				state = 93;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 80:
		{
			if (yytext[i] == 't')
			{
				state = 94;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 81:
		{
			if (i == N - 1)
			{
				return "IF";
				break;
			}
			break;
		}
		case 82:
		{
			if (yytext[i] == 'a')
			{
				state = 95;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 83:
		{
			if (yytext[i] == 's')
			{
				state = 96;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 84:
		{
			if (yytext[i] == 'n')
			{
				state = 97;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 85:
		{
			if (i == N - 1)
			{
				return "DO";
				break;
			}
			if (yytext[i] == 'u')
			{
				state = 98;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 86:
		{
			if (yytext[i] == 'i')
			{
				state = 99;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 87:
		{
			if (yytext[i] == 'e')
			{
				state = 100;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 88:
		{
			if (yytext[i] == 'i')
			{
				state = 101;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 89:
		{
			if (yytext[i] == 'r')
			{
				state = 102;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 90:
		{
			if (yytext[i] == 's')
			{
				state = 103;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 91:
		{
			if (yytext[i] == 't')
			{
				state = 104;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 92:
		{
			break;
		}
		case 93:
		{
			if (yytext[i] == 'd')
			{
				state = 105;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 94:
		{
			if (i == N - 1)
			{
				return "INT";
				break;
			}
			break;
		}
		case 95:
		{
			if (yytext[i] == 'r')
			{
				state = 106;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 96:
		{
			if (yytext[i] == 'e')
			{
				state = 107;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 97:
		{
			if (yytext[i] == 't')
			{
				state = 108;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 98:
		{
			if (yytext[i] == 'b')
			{
				state = 109;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 99:
		{
			if (yytext[i] == 't')
			{
				state = 110;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 100:
		{
			if (yytext[i] == 'a')
			{
				state = 111;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 101:
		{
			if (yytext[i] == 'l')
			{
				state = 112;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 102:
		{
			if (i == N - 1)
			{
				return "FOR";
				break;
			}
			break;
		}
		case 103:
		{
			if (yytext[i] == 'e')
			{
				state = 113;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 104:
		{
			if (yytext[i] == 'u')
			{
				state = 114;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 105:
		{
			if (i == N - 1)
			{
				return "VOID";
				break;
			}
			break;
		}
		case 106:
		{
			if (i == N - 1)
			{
				return "CHAR";
				break;
			}
			break;
		}
		case 107:
		{
			if (i == N - 1)
			{
				return "CASE";
				break;
			}
			break;
		}
		case 108:
		{
			if (yytext[i] == 'i')
			{
				state = 115;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 109:
		{
			if (yytext[i] == 'l')
			{
				state = 116;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 110:
		{
			if (yytext[i] == 'c')
			{
				state = 117;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 111:
		{
			if (yytext[i] == 'k')
			{
				state = 118;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 112:
		{
			if (yytext[i] == 'e')
			{
				state = 119;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 113:
		{
			if (i == N - 1)
			{
				return "ELSE";
				break;
			}
			break;
		}
		case 114:
		{
			if (yytext[i] == 'r')
			{
				state = 120;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 115:
		{
			if (yytext[i] == 'n')
			{
				state = 121;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 116:
		{
			if (yytext[i] == 'e')
			{
				state = 122;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 117:
		{
			if (yytext[i] == 'h')
			{
				state = 123;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 118:
		{
			if (i == N - 1)
			{
				return "BREAK";
				break;
			}
			break;
		}
		case 119:
		{
			if (i == N - 1)
			{
				return "WHILE";
				break;
			}
			break;
		}
		case 120:
		{
			if (yytext[i] == 'n')
			{
				state = 124;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 121:
		{
			if (yytext[i] == 'u')
			{
				state = 125;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 122:
		{
			if (i == N - 1)
			{
				return "DOUBLE";
				break;
			}
			break;
		}
		case 123:
		{
			if (i == N - 1)
			{
				return "SWITCH";
				break;
			}
			break;
		}
		case 124:
		{
			if (i == N - 1)
			{
				return "RETURN";
				break;
			}
			break;
		}
		case 125:
		{
			if (yytext[i] == 'e')
			{
				state = 126;
				break;
			}
			else
			{
				return "ERROR";
			}
			break;
		}
		case 126:
		{
			if (i == N - 1)
			{
				return "CONTINUE";
				break;
			}
			break;
		}
		}
	}
}

bool isChaDig(char c)
{
	if ((c >= 'a'&&c <= 'z') || (c >= 'A'&&c <= 'Z') || (c >= '0'&&c <= '9'))
		return true;
	else return false;
}


//int main()
//{
//	printf("please input the test program in input.txt\n");
//	system("input.txt");
//	yyin = fopen("input.txt", "r");
//	yyout = fopen("output.txt", "w");
//	char yy[20];
//	yy[0] = '\n';
//	int length = 0;
//	char be = 'a';
//	char c;
//	while (fscanf(yyin, "%c", &c) != EOF)
//	{
//		if (isChaDig(be) != isChaDig(c) || (!isChaDig(c) && !isChaDig(be) && c != be) || (c == '\('&&be == '\(') || (c == '\)'&& be == '\)'))
//		{
//			if (yy[0] != '\n')
//			{
//				for (int i = 0; i < length; i++)
//					fputc(yy[i], yyout);
//				fputc('\t', yyout);
//				string result = analysis(yy, length);
//				for (int i = 0; i < result.size(); i++)
//					fputc(result[i], yyout);
//				fputc('\n', yyout);
//			}
//			if (c != '\t'&&c != ' ')
//			{
//				yy[0] = c;
//				length = 1;
//			}
//			else
//			{
//				yy[0] = '\n';
//			}
//		}
//		else
//		{
//			yy[length] = c;
//			length++;
//		}
//		be = c;
//	}
//	for (int i = 0; i < length; i++)
//		fputc(yy[i], yyout);
//	fputc('\t', yyout);
//	string result = analysis(yy, length);
//	for (int i = 0; i < result.size(); i++)
//		fputc(result[i], yyout);
//	fputc('\n', yyout);
//	fclose(yyin);
//	fclose(yyout);
//	system("output.txt");
//	return 0;
//}