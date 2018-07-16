%{
%}
%union
{
	int ival;
	float fval;
}
%token <ival> DIGIT
%type <ival>  expr0 expr 
%left '+' '-'
%left '*' '/'
%%
expr0  :  expr   {$$=$1;cout<<"����������Ϊ��"<<$$<<endl;}
       ;
expr :    expr '-' expr  {$$=$1-$3;}
       |  expr '+' expr  {$$=$1+$3;}
       |  expr '*' expr  {$$=$1*$3;}
       |  expr '/' expr  {$$=$1/$3;}
       |  DIGIT      {$1=yyval.ival;$$=$1;} ;
%%