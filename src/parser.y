%{
    #include <iostream>
    #include <fstream>

    #include "../include/ParserResult.hpp"

    #include "../include/ast/AstNode.hpp"
    #include "../include/ast/AstValue.hpp"
    #include "../include/ast/AstLeftValue.hpp"
    #include "../include/ast/AstRightValue.hpp"
    #include "../include/ast/AstNumber.hpp"
    #include "../include/ast/AstVariable.hpp"
    #include "../include/ast/AstArray.hpp"
    #include "../include/ast/AstExpression.hpp"
    #include "../include/ast/AstCondition.hpp"
    #include "../include/ast/AstArgsList.hpp"
    #include "../include/ast/AstArgsDeclaration.hpp"
    #include "../include/ast/AstDeclarations.hpp"
    #include "../include/ast/AstProcedureHeader.hpp"
    #include "../include/ast/AstProcedure.hpp"
    #include "../include/ast/AstProcedures.hpp"
    #include "../include/ast/AstCommand.hpp"
    #include "../include/ast/AstAssignment.hpp"
    #include "../include/ast/AstIf.hpp"
    #include "../include/ast/AstWhile.hpp"
    #include "../include/ast/AstRepeat.hpp"
    #include "../include/ast/AstProcedureCall.hpp"
    #include "../include/ast/AstRead.hpp"
    #include "../include/ast/AstWrite.hpp"
    #include "../include/ast/AstCommands.hpp"
    #include "../include/ast/AstMain.hpp"
    #include "../include/ast/AstProgram.hpp"

    #undef YAC_DEBUG

    static void yyerror(const char *msg);
    static int yyparse(void);
    int yylex(void);

    extern FILE* yyin;

    static ParserResult parserResult { ParserResultCode::SUCCESS, nullptr, nullptr };

    static std::shared_ptr<AstValue> castAstValue(AstValue* value) {
        switch(value->getNodeType()) {
            case AstNode::NodeType::VARIABLE:
                return std::make_shared<AstVariable>(*dynamic_cast<AstVariable*>(value));
            case AstNode::NodeType::ARRAY:
                return std::make_shared<AstArray>(*dynamic_cast<AstArray*>(value));
            case AstNode::NodeType::NUMBER:
                return std::make_shared<AstNumber>(*dynamic_cast<AstNumber*>(value));
            default:
                return nullptr;
        }
    }

    static std::shared_ptr<AstLeftValue> castAstLValue(AstLeftValue* value) {
            switch(value->getNodeType()) {
                case AstNode::NodeType::VARIABLE:
                    return std::make_shared<AstVariable>(*dynamic_cast<AstVariable*>(value));
                case AstNode::NodeType::ARRAY:
                    return std::make_shared<AstArray>(*dynamic_cast<AstArray*>(value));
                default:
                    return nullptr;
            }
        }

    static std::shared_ptr<AstCommand> castAstCommand(AstCommand* command) {
        switch(command->getCommandType()) {
            case AstCommand::CommandType::ASSIGNMENT:
                return std::make_shared<AstAssignment>(*dynamic_cast<AstAssignment*>(command));
            case AstCommand::CommandType::IF:
                return std::make_shared<AstIf>(*dynamic_cast<AstIf*>(command));
            case AstCommand::CommandType::WHILE:
                return std::make_shared<AstWhile>(*dynamic_cast<AstWhile*>(command));
            case AstCommand::CommandType::REPEAT:
                return std::make_shared<AstRepeat>(*dynamic_cast<AstRepeat*>(command));
            case AstCommand::CommandType::PROCEDURE_CALL:
                return std::make_shared<AstProcedureCall>(*dynamic_cast<AstProcedureCall*>(command));
            case AstCommand::CommandType::READ:
                return std::make_shared<AstRead>(*dynamic_cast<AstRead*>(command));
            case AstCommand::CommandType::WRITE:
                return std::make_shared<AstWrite>(*dynamic_cast<AstWrite*>(command));
            case AstCommand::CommandType::UNDEFINED:
                return nullptr;
        }
    }
%}

%code requires
{
    #include <cstdint>
    #include <cstring>
    #include <iostream>
    #include <fstream>
    #include <string>

    #include "../include/ParserResult.hpp"

    #include "../include/ast/AstNode.hpp"
    #include "../include/ast/AstValue.hpp"
    #include "../include/ast/AstLeftValue.hpp"
    #include "../include/ast/AstRightValue.hpp"
    #include "../include/ast/AstNumber.hpp"
    #include "../include/ast/AstVariable.hpp"
    #include "../include/ast/AstArray.hpp"
    #include "../include/ast/AstExpression.hpp"
    #include "../include/ast/AstCondition.hpp"
    #include "../include/ast/AstArgsList.hpp"
    #include "../include/ast/AstArgsDeclaration.hpp"
    #include "../include/ast/AstDeclarations.hpp"
    #include "../include/ast/AstProcedureHeader.hpp"
    #include "../include/ast/AstProcedure.hpp"
    #include "../include/ast/AstProcedures.hpp"
    #include "../include/ast/AstCommand.hpp"
    #include "../include/ast/AstAssignment.hpp"
    #include "../include/ast/AstIf.hpp"
    #include "../include/ast/AstWhile.hpp"
    #include "../include/ast/AstRepeat.hpp"
    #include "../include/ast/AstProcedureCall.hpp"
    #include "../include/ast/AstRead.hpp"
    #include "../include/ast/AstWrite.hpp"
    #include "../include/ast/AstCommands.hpp"
    #include "../include/ast/AstMain.hpp"
    #include "../include/ast/AstProgram.hpp"

    typedef struct Token {
        uint64_t line;
        uint64_t numericalValue;
        std::string* stringValue;
    } Token;
}

%union {
    Token token;
    AstValue* value;
    AstLeftValue* lvalue;
    AstRightValue* rvalue;
    AstVariable* variable;
    AstArray* array;
    AstNumber* number;
    AstExpression* expression;
    AstCondition* condition;
    AstArgsList* args;
    AstArgsDeclaration* args_declaration;
    AstDeclarations* declarations;
    AstProcedureHeader* procedure_header;
    AstProcedure* procedure;
    AstProcedures* procedures;
    AstCommand* command;
    AstAssignment* assignmentCommand;
    AstIf* ifCommand;
    AstWhile* whileCommand;
    AstRepeat* repeatCommand;
    AstProcedureCall* procedureCallCommand;
    AstRead* readCommand;
    AstWrite* writeCommand;
    AstCommands* commands;
    AstMain* main;
    AstProgram* program;
}

%start program

%token	EQUAL NOT_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%token 	ADD SUB DIV MUL MOD
%token	ASSIGN
%token	PROGRAM PROCEDURE IS IN END
%token	READ WRITE
%token	WHILE DO ENDWHILE
%token  REPEAT UNTIL
%token	IF THEN ELSE ENDIF
%token	VARIABLE NUMBER
%token  ARRAY_TYPE
%token	LEFT_BRACKET RIGHT_BRACKET LEFT_ARRAY_BRACKET RIGHT_ARRAY_BRACKET
%token	ERROR
%token  SEMICOLON COMMA

%type <token>	EQUAL NOT_EQUAL LESS GREATER LESS_EQUAL GREATER_EQUAL
%type <token> 	ADD SUB DIV MUL MOD
%type <token>	ASSIGN
%type <token>	PROGRAM PROCEDURE IS IN END
%type <token>	READ WRITE
%type <token>	WHILE DO ENDWHILE
%type <token>	REPEAT UNTIL repeat_begin
%type <token>   IF THEN ELSE ENDIF
%type <token>	VARIABLE NUMBER
%type <token>   ARRAY_TYPE
%type <token>	LEFT_BRACKET RIGHT_BRACKET LEFT_ARRAY_BRACKET RIGHT_ARRAY_BRACKET
%type <token>	ERROR
%type <token>	SEMICOLON COMMA

%type <rvalue>                  rvalue
%type <lvalue>                  lvalue
%type <value>                   value
%type <expression>              expression
%type <condition>               condition
%type <args>                    args
%type <args_declaration>        args_declaration
%type <declarations>            declarations
%type <procedure_header>        procedure_header
%type <procedures>              procedures
%type <procedure>               simple_procedure_begin complex_procedure_begin procedure_end
%type <command>                 command
%type <ifCommand>               if_begin if_else if_end if_else_end
%type <whileCommand>            while_begin while_end
%type <repeatCommand>           repeat_end
%type <procedureCallCommand>    procedure_call
%type <commands>                commands
%type <main>                    main
%type <program>                 program

%%

program:
    procedures main {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: program parsed successfully" << std::endl;
    #endif

        $$ = new AstProgram(std::make_shared<AstProcedures>(*$1), std::make_shared<AstMain>(*$2));

        parserResult.ast = std::shared_ptr<AstProgram>($$);
    }
;

procedures:
    procedures complex_procedure_begin procedure_end {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: procedure(head, declarations, commands) parsed successfully" << std::endl;
    #endif

        $$->addProcedure(std::make_shared<AstProcedure>($2->getHeader(), $2->getDeclarations(), $3->getCommands()));
    }
|
    procedures simple_procedure_begin procedure_end {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: procedure(head, commands) parsed successfully" << std::endl;
    #endif

        $$->addProcedure(std::make_shared<AstProcedure>($2->getHeader(), $2->getDeclarations(), $3->getCommands()));
    }
|
    %empty {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: empty procedures parsed successfully" << std::endl;
    #endif

        $$ = new AstProcedures();
    }
;

simple_procedure_begin:
    PROCEDURE procedure_header IS IN {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: simple_procedure_begin parsed successfully" << std::endl;
    #endif

        $$ = new AstProcedure(std::make_shared<AstProcedureHeader>(*$2), std::make_shared<AstDeclarations>(), nullptr);
    }
;

complex_procedure_begin:
    PROCEDURE procedure_header IS declarations IN {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: complex_procedure_begin parsed successfully" << std::endl;
    #endif

        $$ = new AstProcedure(std::make_shared<AstProcedureHeader>(*$2), std::make_shared<AstDeclarations>(*$4), nullptr);
    }
;

procedure_end:
    commands END {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: procedure_end parsed successfully" << std::endl;
    #endif

        $$ = new AstProcedure(nullptr, nullptr, std::make_shared<AstCommands>(*$1));
    }
;

main:
    PROGRAM IS declarations IN commands END {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: main(declarations, commands) parsed successfully" << std::endl;
    #endif

        $$ = new AstMain(std::make_shared<AstDeclarations>(*$3), std::make_shared<AstCommands>(*$5));
    }
|
    PROGRAM IS IN commands END {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: main(commands) parsed successfully" << std::endl;
    #endif

        $$ = new AstMain(nullptr, std::make_shared<AstCommands>(*$4));
    }
;

commands:
    commands command {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: commands(commands, command) parsed successfully" << std::endl;
    #endif

        $$->addCommand(castAstCommand($2));
    }
|
    command {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: commands(command) parsed successfully" << std::endl;
    #endif

        $$ = new AstCommands(castAstCommand($1));
    }
;

command:
    lvalue ASSIGN expression SEMICOLON {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(assign;) parsed successfully" << std::endl;
    #endif

        $$ = new AstAssignment(castAstLValue($1), std::make_shared<AstExpression>(*$3));
    }
|
    if_begin if_else if_else_end {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(if then else endif) parsed successfully" << std::endl;
    #endif
        $$ = new AstIf($1->getCondition(), $2->getCommands(), $3->getElseCommands());
    }
|
    if_begin if_end {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(if then endif) parsed successfully" << std::endl;
    #endif
        $$ = new AstIf($1->getCondition(), $2->getCommands(), nullptr);
    }
|
    while_begin while_end {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(while do endwhile) parsed successfully" << std::endl;
    #endif

        $$ = new AstWhile($1->getCondition(), $2->getCommands());
    }
|
    repeat_begin repeat_end {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(repeat until;) parsed successfully" << std::endl;
    #endif

        $$ = $2;
    }
|
    procedure_call SEMICOLON {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(procedure_call;) parsed successfully" << std::endl;
    #endif

        $$ = $1;
    }
|
    READ lvalue SEMICOLON {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(read;) parsed successfully" << std::endl;
    #endif

        $$ = new AstRead(castAstLValue($2));
    }
|
    WRITE value SEMICOLON {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: command(write;) parsed successfully" << std::endl;
    #endif

        $$ = new AstWrite(castAstValue($2));
    }
;

if_begin:
    IF condition THEN {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: if_end parsed successfully" << std::endl;
    #endif

        $$ = new AstIf(std::make_shared<AstCondition>(*$2), nullptr, nullptr);
    }
;

if_else:
    commands ELSE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: if_else parsed successfully" << std::endl;
    #endif

        $$ = new AstIf(nullptr, std::make_shared<AstCommands>(*$1), nullptr);
    }
;

if_end:
    commands ENDIF {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: if_end parsed successfully" << std::endl;
    #endif

        $$ = new AstIf(nullptr, std::make_shared<AstCommands>(*$1), nullptr);
    }
;

if_else_end:
    commands ENDIF {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: if_else_end parsed successfully" << std::endl;
    #endif

        $$ = new AstIf(nullptr, nullptr, std::make_shared<AstCommands>(*$1));
    }
;

while_begin:
    WHILE condition DO {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: while_begin parsed successfully" << std::endl;
    #endif

        $$ = new AstWhile(std::make_shared<AstCondition>(*$2), nullptr);
    }
;

while_end:
    commands ENDWHILE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: while_end parsed successfully" << std::endl;
    #endif

        $$ = new AstWhile(nullptr, std::make_shared<AstCommands>(*$1));
    }
;

repeat_begin:
    REPEAT {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: repeat_begin parsed successfully" << std::endl;
    #endif
    }
;

repeat_end:
    commands UNTIL condition SEMICOLON {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: repeat_end parsed successfully" << std::endl;
    #endif

        $$ = new AstRepeat(std::make_shared<AstCondition>(*$3), std::make_shared<AstCommands>(*$1));
    }
;

procedure_header:
    VARIABLE LEFT_BRACKET args_declaration RIGHT_BRACKET {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: procedure_header(variable, (args_declaration)) parsed successfully" << std::endl;
    #endif

        $$ = new AstProcedureHeader(*$1.stringValue, std::make_shared<AstArgsDeclaration>(*$3));
    }
;

procedure_call:
    VARIABLE LEFT_BRACKET args RIGHT_BRACKET {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: procedure_call(variable, (args)) parsed successfully" << std::endl;
    #endif

        $$ = new AstProcedureCall(*$1.stringValue, std::make_shared<AstArgsList>(*$3));
    }
;

declarations:
    declarations COMMA VARIABLE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: declarations(declarations, variable) parsed successfully" << std::endl;
    #endif

        $$->addDeclaration(std::make_shared<AstVariable>(*$3.stringValue));
    }
|
    declarations COMMA VARIABLE LEFT_ARRAY_BRACKET NUMBER RIGHT_ARRAY_BRACKET {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: declarations(declarations, variable[NUMBER]) parsed successfully" << std::endl;
    #endif

        $$->addDeclaration(std::make_shared<AstArray>(*$3.stringValue, std::make_shared<AstNumber>($5.numericalValue)));
    }
|
    VARIABLE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: declarations(variable) parsed successfully" << std::endl;
    #endif

        $$ = new AstDeclarations(std::make_shared<AstVariable>(*$1.stringValue));
    }
|
    VARIABLE LEFT_ARRAY_BRACKET NUMBER RIGHT_ARRAY_BRACKET {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: declarations(variable[NUMBER]) parsed successfully" << std::endl;
    #endif

        $$ = new AstDeclarations(std::make_shared<AstArray>(*$1.stringValue, std::make_shared<AstNumber>($3.numericalValue)));
    }
;

args_declaration:
    args_declaration COMMA VARIABLE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: args_declaration(args_declaration, variable) parsed successfully" << std::endl;
    #endif

        $$->addArg(std::make_shared<AstVariable>(*$3.stringValue));
    }
|
    args_declaration COMMA ARRAY_TYPE VARIABLE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: args_declaration(args_declaration, T variable) parsed successfully" << std::endl;
    #endif

        $$->addArg(std::make_shared<AstArray>(*$4.stringValue, nullptr));
    }
|
    VARIABLE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: args_declaration(variable) parsed successfully" << std::endl;
    #endif

        $$ = new AstArgsDeclaration(std::make_shared<AstVariable>(*$1.stringValue));
    }
|
    ARRAY_TYPE VARIABLE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: args_declaration(T variable) parsed successfully" << std::endl;
    #endif

        $$ = new AstArgsDeclaration(std::make_shared<AstArray>(*$2.stringValue, nullptr));
    }
;

args:
    args COMMA lvalue {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: args(args, lvalue) parsed successfully" << std::endl;
    #endif

        $$->addArg(castAstLValue($3));
    }
|
    lvalue {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: args(lvalue) parsed successfully" << std::endl;
    #endif

        $$ = new AstArgsList(castAstLValue($1));
    }
;

expression:
    value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: expression(value) parsed successfully" << std::endl;
    #endif
        $$ = new AstExpression(AstExpression::ExpressionType::VALUE, castAstValue($1));
    }
|
    value ADD value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: expression(value + value) parsed successfully" << std::endl;
    #endif

        $$ = new AstExpression(AstExpression::ExpressionType::ADD, castAstValue($1), castAstValue($3));
    }
|
    value SUB value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: expression(value - value) parsed successfully" << std::endl;
    #endif


        $$ = new AstExpression(AstExpression::ExpressionType::SUB, castAstValue($1), castAstValue($3));
    }
|
    value MUL value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: expression(value * value) parsed successfully" << std::endl;
    #endif

        $$ = new AstExpression(AstExpression::ExpressionType::MUL, castAstValue($1), castAstValue($3));
    }
|
    value DIV value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: expression(value / value) parsed successfully" << std::endl;
    #endif

        $$ = new AstExpression(AstExpression::ExpressionType::DIV, castAstValue($1), castAstValue($3));
    }
|
    value MOD value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: expression(value % value) parsed successfully" << std::endl;
    #endif

        $$ = new AstExpression(AstExpression::ExpressionType::MOD, castAstValue($1), castAstValue($3));
    }
;

condition:
    value EQUAL value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: condition(value == value) parsed successfully" << std::endl;
    #endif

        $$ = new AstCondition(AstCondition::ConditionType::EQUAL, castAstValue($1), castAstValue($3));
    }
|
    value NOT_EQUAL value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: condition(value != value) parsed successfully" << std::endl;
    #endif

        $$ = new AstCondition(AstCondition::ConditionType::NOT_EQUAL, castAstValue($1), castAstValue($3));
    }
|
    value GREATER value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: condition(value > value) parsed successfully" << std::endl;
    #endif

        $$ = new AstCondition(AstCondition::ConditionType::GREATER, castAstValue($1), castAstValue($3));
    }
|
    value LESS value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: condition(value < value) parsed successfully" << std::endl;
    #endif

        $$ = new AstCondition(AstCondition::ConditionType::LESS, castAstValue($1), castAstValue($3));
    }
|
    value GREATER_EQUAL value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: condition(value >= value) parsed successfully" << std::endl;
    #endif

        $$ = new AstCondition(AstCondition::ConditionType::GREATER_EQUAL, castAstValue($1), castAstValue($3));
    }
|
    value LESS_EQUAL value {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: condition(value <= value) parsed successfully" << std::endl;
    #endif

        $$ = new AstCondition(AstCondition::ConditionType::LESS_EQUAL, castAstValue($1), castAstValue($3));
    }
;

value:
    lvalue {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: value(lvalue) parsed successfully" << std::endl;
    #endif
    }
|
    rvalue {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: value(rvalue) parsed successfully" << std::endl;
    #endif
    }
;

lvalue:
    VARIABLE {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: lvalue(variable) parsed successfully" << std::endl;
    #endif

        $$ = new AstVariable(*$1.stringValue);
    }
|
    VARIABLE LEFT_ARRAY_BRACKET NUMBER RIGHT_ARRAY_BRACKET {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: lvalue(variable[NUMBER]) parsed successfully" << std::endl;
    #endif

        $$ = new AstArray(*$1.stringValue, std::make_shared<AstNumber>($3.numericalValue));
    }
|
    VARIABLE LEFT_ARRAY_BRACKET VARIABLE RIGHT_ARRAY_BRACKET {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: lvalue(variable[variable]) parsed successfully" << std::endl;
    #endif

        $$ = new AstArray(*$1.stringValue, std::make_shared<AstVariable>(*$3.stringValue));
    }
;

rvalue:
    NUMBER {
    #ifdef YAC_DEBUG
        std::cout << "Yac debug: rvalue(number) parsed successfully" << std::endl;
    #endif

        $$ = new AstNumber($1.numericalValue);
    }
;

%%

static void yyerror(const char* msg) {
    std::cerr << "Error : " << msg << " in line " << yylval.token.line << std::endl;
}

ParserResult parse(const char* inputFileName) {
    yyin = fopen(inputFileName, "r");
    if(yyin == nullptr) {
        std::cerr << "Error : cannot open file " << inputFileName << std::endl;
        parserResult.result = ParserResultCode::FILE_NOT_FOUND;
        return parserResult;
    }
    else {
        parserResult.result = ParserResultCode::SUCCESS;
    }

    const int result = yyparse();
    if(result != 0) {
        parserResult.result = ParserResultCode::SYNTAX_ERROR;
    }

    fclose(yyin);

    return parserResult;
}