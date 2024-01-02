%{

#include <iostream>
#include <fstream>

#include "../include/ast/AstNode.hpp"

#define YAC_DEBUG

static void yyerror(const char *msg);
static int yyparse(void);
int yylex(void);

extern FILE *yyin;

%}

%code requires
{
    #include <cstdint>
    #include <cstring>
    #include <iostream>
    #include <string>

    #include "../include/ast/AstNode.hpp"

    typedef struct Token {
        uint64_t line;
        uint64_t numericalValue;
        std::string* stringValue;
    } Token;
}

%union {
    Token token;
    AstNode* node;
}

%token	EQUAL NOT_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%token 	ADD SUB DIV MUL MOD
%token	ASSIGN
%token	PROGRAM PROCEDURE IS IN END
%token	READ WRITE
%token	WHILE DO ENDWHILE
%token  REPEAT UNTIL
%token	IF THEN ELSE ENDIF
%token	VARIABLE NUMBER
%token	LEFT_BRACKET RIGHT_BRACKET LEFT_ARRAY_BRACKET RIGHT_ARRAY_BRACKET
%token	ERROR
%token  SEMICOLON COMMA


%type <Token>	EQUAL NOT_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%type <Token> 	ADD SUB DIV MUL MOD
%type <Token>	ASSIGN
%type <Token>	PROGRAM PROCEDURE IS IN END
%type <Token>	READ WRITE
%type <Token>	WHILE DO ENDWHILE
%type <Token>	REPEAT UNTIL
%type <Token>   IF THEN ELSE ENDIF
%type <Token>	VARIABLE NUMBER
%type <Token>	LEFT_BRACKET RIGHT_BRACKET LEFT_ARRAY_BRACKET RIGHT_ARRAY_BRACKET
%type <Token>	ERROR
%type <Token>	SEMICOLON COMMA

%type <value>  lvalue rvalue value

%%

program_all:
    procedures main {

    }
;

procedures:
    procedures PROCEDURE proc_head IS declarations IN commands END {

    }
|
    procedures PROCEDURE proc_head IS IN commands END {

    }
|
    /* empty */
;

main:
    PROGRAM IS declarations IN commands END {

    }
|
    PROGRAM IS IN commands END {

    }
;

commands:
    commands command {

    }
|
    command {

    }
;

command:
    lvalue ASSIGN expression SEMICOLON {

    }
|
    IF condition THEN commands ELSE commands ENDIF {

    }
|
    IF condition THEN commands ENDIF {

    }
|
    WHILE condition DO commands ENDWHILE {

    }
|
    REPEAT commands UNTIL condition SEMICOLON {

    }
|
    proc_call SEMICOLON {

    }
|
    READ lvalue SEMICOLON {

    }
|
    WRITE value SEMICOLON {

    }
;

proc_head:
    lvalue LEFT_BRACKET args_decl RIGHT_BRACKET {

    }
;

proc_call:
    lvalue LEFT_BRACKET args RIGHT_BRACKET {

    }
;

declarations:
    declarations SEMICOLON VARIABLE {

    }
|
    declarations SEMICOLON VARIABLE LEFT_ARRAY_BRACKET NUMBER RIGHT_ARRAY_BRACKET {

    }
|
    VARIABLE {

    }
|
    VARIABLE LEFT_ARRAY_BRACKET NUMBER RIGHT_ARRAY_BRACKET {

    }
;

args_decl:
    args_decl COMMA lvalue {

    }
|
    args_decl COMMA "T" lvalue {

    }
|
    lvalue {

    }
|
    "T" lvalue {

    }
;

args:
    args COMMA value {

    }
|
    value {

    }
;

expression:
    value {

    }
|
    value ADD value {

    }
|
    value SUB value {

    }
|
    value MUL value {

    }
|
    value DIV value {

    }
|
    value MOD value {

    }
;

condition:
    value EQUAL value {

    }
|
    value NOT_EQUAL value {

    }
|
    value GREATER value {

    }
|
    value LESS value {

    }
|
    value GREATER_EQUAL value {

    }
|
    value LESS_EQUAL value {

    }
;

value:
    lvalue {

    }
|
    rvalue {

    }
;

lvalue:
    VARIABLE {

    }
|
    VARIABLE LEFT_ARRAY_BRACKET NUMBER RIGHT_ARRAY_BRACKET {

    }
|
    VARIABLE LEFT_ARRAY_BRACKET VARIABLE RIGHT_ARRAY_BRACKET {

    }
;

rvalue:
    NUMBER {

    }
;

%%

static void yyerror(const char* msg) {
    std::cerr << "Error : " << msg << " in line " << yylval.token.line << std::endl;
}

int compile(const char* in_file, const char* out_file) {
    yyin = fopen(in_file, "r");
    const int ret = yyparse();
    fclose(yyin);

    std::ofstream outstream;
    outstream.open(out_file);

    return ret;
}