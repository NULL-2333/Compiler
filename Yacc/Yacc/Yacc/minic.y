%union
{
	int ival;
}
%token <ival> IDENTIFIER FOR WHILE IF ELSE
%token <ival> INT FLOAT LONG SHORT CHAR DOUBLE SIGNED UNSIGNED
%token <ival> EQU LESS GREAT NE
%token <ival> NUM
%%
source_file : type function_name '(' parament_list ')' compound_statement
	    | type function_name '(' ')' compound_statement
	    ;

type : INT
     | FLOAT
     | LONG
     | SHORT
     | CHAR
     | DOUBLE
     | SIGNED
     | UNSIGNED
     ;

function_name : identifier ;

identifier : IDENTIFIER ;

parament_list : type identifier ',' parament_list
              | type identifier
              ;

compound_statement : '{' statement_list '}'
                   ;

statement_list : statement
               | statement_list statement ;

statement : assign_statement
          | selection_statement
          | iteration_statement
          ;

assign_statement : assign_expression ';'
                 | initial_expression ';'
                 ;

assign_expression : identifier operator identifier
                  ;

operator : '+'
         | '-'
         | '*'
         | '/'
         ;

initial_expression : type identifier
                   | type identifier '=' NUM 
                   | type identifier '=' identifier
                   | type identifier '=' assign_expression
                   | identifier '=' assign_expression
                   ;

selection_statement : IF '(' relate_expression ')' compound_statement
                    ;

relate_expression : identifier relation_operator identifier
           ;

relation_operator : EQU
                  | LESS
                  | GREAT
                  | NE
                  ;

iteration_statement : WHILE '(' relate_expression ')' compound_statement
                    ;
