%{
#include "node.h"
#include <iostream>
#include <string>
extern int yylex();
extern void yyerror(const char* s);

// Global for the main block
NBlock* g_MainBlock = nullptr;

// Disable the warnings that Bison creates
#pragma warning(disable: 4065)
%}

/* Add one union member for each Node* type */
%union {
    Node* node;
	NBlock* block;
	NStatement* statement;
	NNumeric* numeric;
	NBoolean* boolean;
	std::string* string;
	int token;
}

%error-verbose

/* Terminal symbols */
%token <string> TINTEGER
%token <token> TLBRACE TRBRACE TSEMI TLPAREN TRPAREN
%token <token> TMAIN TROTATE TFORWARD TIF TELSE TATTACK TRANGEDATTTACK
%token <token> TISZOMBIE TISHUMAN TISWALL TISPASSABLE TISRANDOM

/* Statements */
%type <block> main_loop block
%type <statement> statement rotate forward ifelse attack ranged_attack
%type <boolean> is_zombie is_human is_wall is_passable is_random
 
/* Expressions */
%type <numeric> numeric

%%

main_loop	: TMAIN TLBRACE block TRBRACE { std::cout << "Main entry point found!" << std::endl; }
;

block		: statement { std::cout << "Single statement" << std::endl; }
			  | block statement { std::cout << "Multiple statements" << std::endl; }
/* TODO: Add support for multiple statements in a block */
;

ifelse		: TIF TLPAREN boolean TRPAREN TLBRACE { std::cout << "IF block\n"; } block TRBRACE
			  TELSE TLBRACE { std::cout << "ELSE block\n"; } block TRBRACE

statement	: rotate TSEMI | forward TSEMI | ifelse | ranged_attack TSEMI | attack TSEMI
			  | boolean TSEMI
;
			
rotate		: TROTATE TLPAREN numeric TRPAREN { std::cout << "Rotate command" << std::endl; }
;

forward		: TFORWARD TLPAREN TRPAREN { std::cout << "Forward command" << std::endl; }
;

is_zombie	: TISZOMBIE TLPAREN numeric TRPAREN { std::cout << "Test zombie command" << std::endl; }
;

is_human	: TISHUMAN TLPAREN numeric TRPAREN { std::cout << "Test human command" << std::endl; }
;

is_wall		: TISWALL TLPAREN TRPAREN { std::cout << "Test wall command" << std::endl; }
;

is_passable	: TISPASSABLE TLPAREN TRPAREN { std::cout << "Test passable command" << std::endl; }
;

is_random	: TISRANDOM TLPAREN TRPAREN { std::cout << "Test random command" << std::endl; }
;

attack		: TATTACK TLPAREN TRPAREN { std::cout << "Attack command" << std::endl; }
;

ranged_attack	: TRANGEDATTTACK TLPAREN TRPAREN { std::cout << "Ranged attack command" << std::endl; }
;
			
numeric		: TINTEGER { std::cout << "Numeric value of " << *($1) << std::endl; }
;

boolean		: is_zombie | is_human | is_wall | is_passable | is_random
;

%%
