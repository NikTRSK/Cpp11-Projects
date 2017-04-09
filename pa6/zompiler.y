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
%type <boolean> boolean is_zombie is_human is_wall is_passable is_random
 
/* Expressions */
%type <numeric> numeric

%%

main_loop	: TMAIN TLBRACE block TRBRACE { ($3)->SetMainBlock(); g_MainBlock = $3;
											std::cout << "Main entry point found!" << std::endl; }
;

block		: statement { $$ = new NBlock(); 
						  ($$)->AddStatement($1); std::cout << "Single statement" << std::endl; }
			  | block statement { ($1)->AddStatement($2); 
								  std::cout << "Multiple statements" << std::endl; }
/* TODO: Add support for multiple statements in a block */
;

ifelse		: TIF TLPAREN boolean TRPAREN TLBRACE block TRBRACE
			  TELSE TLBRACE block TRBRACE { $$ = new NIf($3, $6, $10); };

statement	: rotate TSEMI | forward TSEMI | ifelse | ranged_attack TSEMI | attack TSEMI
			  | boolean TSEMI
;
			
rotate		: TROTATE TLPAREN numeric TRPAREN { $$ = new NRotate($3); std::cout << "Rotate command" << std::endl; }
;

forward		: TFORWARD TLPAREN TRPAREN { $$ = new NForward(); std::cout << "Forward command" << std::endl; }
;

is_zombie	: TISZOMBIE TLPAREN numeric TRPAREN { $$ = new NIs_Zombie($3); std::cout << "Test zombie command" << std::endl; }
;

is_human	: TISHUMAN TLPAREN numeric TRPAREN { $$ = new NIs_Human($3); std::cout << "Test human command" << std::endl; }
;

is_wall		: TISWALL TLPAREN TRPAREN { $$ = new NIs_Wall(); std::cout << "Test wall command" << std::endl; }
;

is_passable	: TISPASSABLE TLPAREN TRPAREN { $$ = new NIs_Passable(); std::cout << "Test passable command" << std::endl; }
;

is_random	: TISRANDOM TLPAREN TRPAREN { $$ = new NIs_Random(); std::cout << "Test random command" << std::endl; }
;

attack		: TATTACK TLPAREN TRPAREN { $$ = new NAttack(); std::cout << "Attack command" << std::endl; }
;

ranged_attack	: TRANGEDATTTACK TLPAREN TRPAREN { $$ = new NRanged_Attack(); std::cout << "Ranged attack command" << std::endl; }
;
			
numeric		: TINTEGER { $$ = new NNumeric(*($1)); std::cout << "Numeric value of " << *($1) << std::endl; }
;

boolean		: is_zombie | is_human | is_wall | is_passable | is_random
;

%%
