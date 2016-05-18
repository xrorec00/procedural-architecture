%{
#include <iostream>	
#include <generator/collection/collection.h>
#include <functions/functionList.h> //added
#include <bison/bison.h>
#include "../tokenList.h"

extern int lineNum;
%}

%option outfile="lex.cpp" header-file="lex.h"

%option reentrant noyywrap never-interactive nounistd
%option bison-bridge

%%
[ \t]           	{/*+ action: pass*/}
"//".*				{/*+ action: pass*/}
\n 					{++lineNum;/*+ action: pass*/}
rule				{return RULE;}
return				{return RETURN;}
true				{
						yylval->bval = true;
						return BOOL;
					}
false				{
						yylval->bval = false;
						return BOOL;
					}
print				{return PRINT;}
if					{return IF;}
else				{return ELSE;}
for					{return FOR;}
while				{return WHILE;}
symbol				{return SYMBOL;}
int 				{return INT;}
float 				{return FLOAT;}
string 				{return STRING;}
"&&"				{return AND;}
"||"				{return OR;}
"_output.empty"		{return OUTPUTEMPTY;}
"_output.add"		{return OUTPUTADD;}
"_output.get"		{return OUTPUTGET;}
randomInt			{return RANDOMINT;}
randomFloat			{return RANDOMFLOAT;}
rotate				{return ROTATE;}
"_mesh.addFace"		{return MESHADDFACE;}
"_mesh.addVertex"	{return MESHADDVERTEX;}
output 				{return OUTPUT;}
settings			{return SETTINGS;}
[0-9]+\.[0-9]+		{
						yylval->fval = atof(yytext);
						return FLOAT;
					}
[0-9]+				{
						yylval->ival = atoi(yytext);
						return INT;
					}
[$]?[a-zA-Z0-9_]+	{
						yylval->sval = std::string(strdup(yytext));
						return IDENTIFIER;
					}
\".+\"				{
						std::string value = std::string(strdup(yytext));
						yylval->sval = value.substr(1, value.size() - 2);//.c_str();
						return STRING;
					} 
"("					{return LPAR;}
")"					{return RPAR;}
"["					{return LSQBRAC;}
"]"					{return RSQBRAC;}
"{"					{return LBRAC;}
"}"					{return RBRAC;}
";"					{return SEMICOLON;}
":"					{return COLON;}
","					{return COMMA;}
"++"				{return PLUSPLUS;}
"+"					{return PLUS;}
"-"					{return MINUS;}
"/"					{return DIVIDE;}
"*"					{return MULTIPLY;}
"=="				{return EQUALITY;}
"!="				{return NEQUALITY;}
"<"					{return LESS;}
"<="				{return LESSEQUAL;}
">"				 	{return GREATER;}
">="				{return GREATEREQUAL;}
"="					{return EQUAL;}
"."					{return DOT;}
.					{std::cout<<"Chybny vyraz na "<<lineNum<<". radku.\n";/*+ action: invalid token*/}
<<EOF>>				{lineNum=0;yyterminate();}
%%

void yyerror(const char *s) {
    std::cout << s << std::endl;
    exit(1);
}