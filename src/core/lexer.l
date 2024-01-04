%{

#define NO_YY_WRAP
#define YY_NO_UNPUT
#define YY_NO_INPUT

#include <cstdint>
#include <cstdio>

#include "parser.hpp"

#undef LEX_DEBUG

uint64_t linesCounter = 1;

static int createToken(int token);
static int createVariable(int token);
static int createNumber(int token);

%}

WHITE_CHARS                 [ \t]

%%

#.*$                       {
                            #ifdef LEX_DEBUG
                                std::cout << "Lex debug: comment [" << linesCounter << "] `" << yytext << "`\n";
                            #endif
                                ++linesCounter;
                            }
{WHITE_CHARS}+              {
                            #ifdef LEX_DEBUG
                                std::cout << "Lex debug: white chars [" << linesCounter << "] `" << yytext << "`\n";
                            #endif
                            }
^{WHITE_CHARS}*\n           {
                            #ifdef LEX_DEBUG
                                std::cout << "Lex debug: white chars with new line [" << linesCounter << "] `" << yytext << "`\n";
                            #endif
                                ++linesCounter;
                            }

\n                          {
                            #ifdef LEX_DEBUG
                                std::cout << "Lex debug: new line [" << linesCounter << "] `" << yytext << "`\n";
                            #endif
                                ++linesCounter;
                            }

\;                          {   return createToken(SEMICOLON);                          }
\,                          {   return createToken(COMMA);                              }

\+                          {   return createToken(ADD);                                }
\-                          {   return createToken(SUB);                                }
\*                          {   return createToken(MUL);                                }
\/                          {   return createToken(DIV);                                }
\%                          {   return createToken(MOD);                                }

\:\=                        {   return createToken(ASSIGN);                             }

\=                          {   return createToken(EQUAL);                              }
\!\=                        {   return createToken(NOT_EQUAL);                          }
\<                          {   return createToken(LESS);                               }
\>                          {   return createToken(GREATER);                            }
\<\=                        {   return createToken(LESS_EQUAL);                         }
\>\=                        {   return createToken(GREATER_EQUAL);                      }

"PROGRAM"                   {   return createToken(PROGRAM);                            }

"PROCEDURE"                 {   return createToken(PROCEDURE);                          }
"IS"                        {   return createToken(IS);                                 }
"IN"                        {   return createToken(IN);                                 }
"END"                       {   return createToken(END);                                }

"READ"                      {   return createToken(READ);                               }
"WRITE"                     {   return createToken(WRITE);                              }

"WHILE"                     {   return createToken(WHILE);                              }
"DO"                        {   return createToken(DO);                                 }
"ENDWHILE"                  {   return createToken(ENDWHILE);                           }

"REPEAT"                    {   return createToken(REPEAT);                             }
"UNTIL"                     {   return createToken(UNTIL);                              }

"IF"                        {   return createToken(IF);                                 }
"THEN"                      {   return createToken(THEN);                               }
"ELSE"                      {   return createToken(ELSE);                               }
"ENDIF"                     {   return createToken(ENDIF);                              }

"T"                         {   return createToken(ARRAY_TYPE);                         }

\[                          {   return createToken(LEFT_ARRAY_BRACKET);                 }
\]                          {   return createToken(RIGHT_ARRAY_BRACKET);                }

\(                          {   return createToken(LEFT_BRACKET);                       }
\)                          {   return createToken(RIGHT_BRACKET);                      }

[_a-z]+                     {   return createVariable(VARIABLE);                        }
[0-9]+                      {   return createNumber(NUMBER);                            }
.                           {   return createToken(ERROR);                              }

%%

static int createToken(int token) {
#ifdef LEX_DEBUG
    std::cout << "Lex debug: general token [" << linesCounter << "] `" << yytext << "`\n";
#endif

    yylval.token.line = linesCounter - 1;
    yylval.token.numericalValue = 0;
    yylval.token.stringValue = nullptr;

    return token;
}

static int createVariable(int token) {
#ifdef LEX_DEBUG
    std::cout << "Lex debug: variable token [" << linesCounter << "] `" << yytext << "`\n";
#endif

    yylval.token.line = linesCounter - 1;
    yylval.token.numericalValue = 0;
    yylval.token.stringValue = new std::string(yytext);

    return token;
}

static int createNumber(int token) {
#ifdef LEX_DEBUG
    std::cout << "Lex debug: number token [" << linesCounter << "] `" << yytext << "`\n";
#endif

    yylval.token.line = linesCounter - 1;
    yylval.token.numericalValue = strtoull(yytext, nullptr, 10);
    yylval.token.stringValue = nullptr;

    return token;
}

int yywrap() {
    return 1;
}
