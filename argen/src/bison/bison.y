%{
#include <iostream>
#include <stdio.h>

#include "../tokenList.h"
    
#include <bison/bison.h>
#include <flex/lex.h>
#include <utils/globals.h>


#include <cstdlib>
#include <stack>

int lineNum = 1;
void yyerror(yyscan_t scanner,std::shared_ptr<Collection> collection, const char *s) {
    std::cout<<s<<" on line: "<<lineNum<<std::endl;
    exit(1);
}

int debugNum=0;
void debugMsg() {
    std::cout<<"+-----Debug message: "<<debugNum<<"-----+"<<std::endl<<std::flush;
    debugNum++;
}

std::shared_ptr<Symbol> newSymbol;

std::vector<std::shared_ptr<Function>> functionVector;

std::stack<std::shared_ptr<Block>> blockStack;

std::stack<std::shared_ptr<Symbol>> symbolStack;

std::stack<std::string> nameStack;

%}



%code requires {
    #ifndef YY_TYPEDEF_YY_SCANNER_T
    #define YY_TYPEDEF_YY_SCANNER_T

    typedef void* yyscan_t;

    #endif
}

%output  "bison.cpp"
%defines "bison.h"

%define api.pure
%lex-param {
    yyscan_t scanner
}
%parse-param {
    yyscan_t scanner
}
%parse-param {
    std::shared_ptr<Collection> collection
}

// Tokens

%token SETTINGS

%token <ival> INT
%token <fval> FLOAT
%token <sval> STRING
%token <bval> BOOL

%token COLON
%token TRUE
%token FALSE
%token DOT
%token SEMICOLON
%token COMMA
%token EQUAL
%token EQUALITY
%token LPAR
%token RPAR
%token LBRAC
%token RBRAC
%token LSQBRAC
%token RSQBRAC

%token RULE
%token SYMBOL

%token OUTPUT
%token OUTPUTEMPTY
%token OUTPUTADD
%token OUTPUTGET
%token ROTATE
%token MESHADDFACE
%token MESHADDVERTEX
%token RANDOMINT
%token RANDOMFLOAT

%token RETURN
%token IF
%token ELSE
%token FOR
%token WHILE
%token PRINT

%type <ival> ruleLength;

%token <sval> IDENTIFIER
%type <sval> type

%type <functionPtr> codeLine
%type <functionPtr> if
%type <containerPtr> else

%type <functionPtr> for
%type <functionPtr> while

%type <functionPtr> variable
%type <functionPtr> print
%type <functionPtr> return

%type <functionPtr> output
%type <functionPtr> mesh
%type <functionPtr> multipleFunctions

%type <functionPtr> expr
%type <functionPtr> genericNullary
%type <functionPtr> rotate
%type <functionPtr> multilevelIdentifier
%type <functionPtr> multilevelIdentifierMaker

%type <functionPtr> increment

%type <functionPtr> comparison
%type <functionPtr> comparisonIdentifierOrNullary
%type <initializerListPtr> initializerList

%left <operator> AND
%left <operator> OR

%left <operator> EQUALITY
%left <operator> NEQUALITY
%left <operator> LESS
%left <operator> LESSEQUAL
%left <operator> GREATER
%left <operator> GREATEREQUAL
%left <operator> PLUSPLUS

%left <operator> PLUS
%left <operator> MINUS
%left <operator> DIVIDE
%left <operator> MULTIPLY

// Rules
//
// "rule body begin/end" is for script that parses rules for documentation

%%
//+----------------------------------------------------------------+
//|---------------------------- MAIN ------------------------------|
//+----------------------------------------------------------------+
main
    : settings main
    | symbol main
    | rule main
    | %empty

//+----------------------------------------------------------------+
//|                           SETTINGS                             |
settings
    : SETTINGS LBRAC settingsItemsOrEmpty RBRAC

settingsItemsOrEmpty
    : settingsItems
    | %empty

settingsItems
    : settingsItems settingsItem
    | settingsItem

settingsItem
    : IDENTIFIER EQUAL BOOL SEMICOLON
    //+ rule body begin
    {
        if ($1 == std::string("DEBUG")) {
            Config::DEBUG=$3;
        }
        else if ($1 == std::string("VERBOSE")) {
            Config::VERBOSE = $3;
        }
        else if ($1 == std::string("EXPORT")) {
            Config::EXPORT = $3;
        }
        else {
            std::cout<<"Error in settings."<<std::endl;
            exit(1);
        }
    }
    //+ rule body end
    | IDENTIFIER EQUAL INT SEMICOLON
    //+ rule body begin
    {
        if ($1 == std::string("ITERATIONS")) {
            Config::ITERATIONS = $3;
        }
        else if ($1 == std::string("SEED")) {
            Config::SEED = $3;
        }
        else {
            std::cout<<"Error in settings."<<std::endl;
            exit(1);
        }
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                            SYMBOL                              |
symbol
    : SYMBOL createSymbol setSymbolType LBRAC symVariablesOrEmpty RBRAC symbolInterpretation
    //+ rule body begin
    {
        collection->symCollection->addTemplate(symbolStack.top());
        symbolStack.pop();
    }
    //+ rule body end
    | SYMBOL createSymbol setSymbolType SEMICOLON
    //+ rule body begin
    {
        collection->symCollection->addTemplate(symbolStack.top());
        symbolStack.pop();
    }
    //+ rule body end

setSymbolType
    : IDENTIFIER
    //+ rule body begin
    {
        symbolStack.top()->setVarSymType($1);
    }
    //+ rule body end

createSymbol
    : %empty
    //+ rule body begin
    {
        std::shared_ptr<Symbol> symbol;
        symbol = std::make_shared<Symbol>(nullptr);
        symbolStack.push(symbol);
    }
    //+ rule body end

symVariablesOrEmpty
    : symVariables
    | %empty

symVariables
    : symVariables variable
    //+ rule body begin
    {
        if ($2->getName() == Function::VARIABLE) {
            symbolStack.top()->addVariable(std::static_pointer_cast<Variable>($2));
        }
        else if ($2->getName() == Function::SYMBOLCREATOR) {
            // SymbolCreator symbolCreator = std::static_pointer_cast<SymbolCreator>($2);
            // symbolStack.top()->addSymbol(symbolCreator->getSymbol());
            // symbolCreator.reset();
            SymbolCreator symbolCreator = *std::static_pointer_cast<SymbolCreator>($2);
            symbolStack.top()->addSymbol(symbolCreator.getSymbol());
        }
        else {
            std::cout<<"Invalid item in symbol declaration. Item type: "<<$2->getName()<<std::endl;
            exit(0);
        }
    }
    //+ rule body end
    | variable
    //+ rule body begin
    {
        if ($1->getName() == Function::VARIABLE) {
            symbolStack.top()->addVariable(std::static_pointer_cast<Variable>($1));
        }
        else if ($1->getName() == Function::SYMBOLCREATOR) {
            // SymbolCreator symbolCreator = std::static_pointer_cast<SymbolCreator>($1);
            // symbolStack.top()->addSymbol(symbolCreator->getSymbol());
            // symbolCreator.reset();
            SymbolCreator symbolCreator = *std::static_pointer_cast<SymbolCreator>($1);
            symbolStack.top()->addSymbol(symbolCreator.getSymbol());
        }
        else {
            std::cout<<"Invalid item in symbol declaration. Item type: "<<$1->getName()<<std::endl;
            exit(0);
        }
    }
    //+ rule body end

symbolInterpretation
    : createInterpretation LBRAC codeLinesOrEmpty RBRAC
    //+ rule body begin
    {
        std::string symbolType = symbolStack.top()->getVarSymType();
        
        std::static_pointer_cast<Interpretation>(blockStack.top())->setSymbolType(symbolType);
        collection->interpretationsCollection->addInterpretation(std::static_pointer_cast<Interpretation>(blockStack.top()));
        blockStack.pop();
    }
    //+ rule body end
    | %empty

createInterpretation
    : %empty
    //+ rule body begin
    {
        std::shared_ptr<Interpretation> container = std::make_shared<Interpretation>();
        blockStack.push(container);
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                              RULE                              |
rule
    : RULE IDENTIFIER ruleLength LBRAC createBlock codeLinesOrEmpty RBRAC LBRAC createBlock codeLinesOrEmpty RBRAC
    //+ rule body begin
    {
        if ($3 < 0) {
            std::cout<<"length < 0"<<std::endl;
            exit(0);
        }

        std::shared_ptr<Block> bodyAction = blockStack.top();
        blockStack.pop();

        std::shared_ptr<Block> bodyCondition = blockStack.top();
        blockStack.pop();
        
        std::shared_ptr<Rule> rule = std::make_shared<Rule>($2, $3, bodyCondition, bodyAction);
        bodyCondition->setRule(rule);
        bodyAction->setRule(rule);
        collection->rulesCollection->addRule(rule);
    }
    //+ rule body end

ruleLength
    : %empty              {$$ = 1; }
    | LSQBRAC INT RSQBRAC {$$ = $2;}

createBlock
    : %empty
    //+ rule body begin
    {
        std::shared_ptr<Block> container;

        if (blockStack.empty()) {
            container = std::make_shared<Block>(nullptr);
        }
        else {
            container = std::make_shared<Block>(blockStack.top());
        }

        blockStack.push(container);
    }
    //+ rule body end

codeLinesOrEmpty
	: codeLines
	| %empty

codeLines
    : codeLine 				{blockStack.top()->addFunction($1);}
    | codeLine codeLines 	{blockStack.top()->addFunction($1);}

codeLine
    : variable 	          {$$ = $1;}
    | if                  {$$ = $1;}
    | for                 {$$ = $1;}
    | while               {$$ = $1;}
    | print 	          {$$ = $1;}
    | return 	          {$$ = $1;}
    | output              {$$ = $1;}
    | mesh                {$$ = $1;}
    | increment SEMICOLON {$$ = $1;} //TODO reconsider

//+----------------------------------------------------------------+
//|-------------------------- FUNCTIONS ---------------------------|
//+----------------------------------------------------------------+
//|              VARIABLE DECLARATION/INITIALIZATION               |
variable
    :type IDENTIFIER SEMICOLON
    //+ rule body begin
    {
        if (!blockStack.empty()) {
            $$ = std::make_shared<Variable>(blockStack.top(), $1, $2); //vyresit typ
        }
        else {
            $$ = std::make_shared<Variable>(nullptr, $1, $2);
        }
    }
    //+ rule body end
    | type IDENTIFIER EQUAL expr SEMICOLON
    //+ rule body begin
    {
        if (!blockStack.empty()) {
            $$ = std::make_shared<Variable>(blockStack.top(), $1, $2, $4);
        }
        else {
            $$ = std::make_shared<Variable>(nullptr, $1, $2, $4);
        }
    }
    //+ rule body end
    | multilevelIdentifier EQUAL expr SEMICOLON
    //+ rule body begin
    {
        if (!blockStack.empty()) {
            $$ = std::make_shared<Setter>(blockStack.top(), std::static_pointer_cast<Getter>($1), $3);
        }
        else {
            $$ = nullptr;
        }
    }
    //+ rule body end
    | IDENTIFIER IDENTIFIER SEMICOLON
    //+ rule body begin
    {
        if (!blockStack.empty()) {
            $$ = std::make_shared<SymbolCreator>(blockStack.top(), collection->symCollection, $1, $2, nullptr);
        }
        else {
            $$ = std::make_shared<SymbolCreator>(nullptr, collection->symCollection, $1, $2, nullptr);    
        }
    }
    //+ rule body end
    | IDENTIFIER IDENTIFIER EQUAL initializerList SEMICOLON
    //+ rule body begin
    {
        if (!blockStack.empty()) {
            $$ = std::make_shared<SymbolCreator>(blockStack.top(), collection->symCollection, $1, $2, $4);
        }
        else {
            $$ = std::make_shared<SymbolCreator>(nullptr, collection->symCollection, $1, $2, $4);    
        }
    }
    //+ rule body end
    | OUTPUTGET LPAR INT RPAR
    //+ rule body begin
    {
        $$ = std::make_shared<OutputEmpty>(blockStack.top());
    }
    //+ rule body end

type
    : INT    {$$ = "int";   }
    | FLOAT  {$$ = "float"; }
    | STRING {$$ = "string";}

//+----------------------------------------------------------------+
//|                               IF                               |
if
    : IF LPAR expr RPAR LBRAC createBlock codeLinesOrEmpty RBRAC else
    //+ rule body begin
    {
        std::shared_ptr<Block> bodyTrue = blockStack.top();
        blockStack.pop();

        if ($9!=nullptr) {
            $9->setParent(bodyTrue->getParent());
        }

        $$ = std::make_shared<If>($3, bodyTrue, $9);
    }
    //+ rule body end
else
    : ELSE LBRAC createBlock codeLinesOrEmpty RBRAC
    //+ rule body begin
    {
        std::shared_ptr<Block> bodyFalse = blockStack.top();
        blockStack.pop();
        $$ = bodyFalse;
    }
    //+ rule body end
    | %empty { $$ = nullptr; }

//+----------------------------------------------------------------+
//|                              FOR                               |
for
    : FOR createBlock LPAR variable expr SEMICOLON increment RPAR LBRAC codeLinesOrEmpty RBRAC
    //+ rule body begin
    {
        std::shared_ptr<Block> body = blockStack.top();
        blockStack.pop();

        $$ = std::make_shared<For>($4, $5, $7, body);
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                             WHILE                              |
while
    : WHILE createBlock LPAR expr RPAR LBRAC codeLinesOrEmpty RBRAC
    //+ rule body begin
    {
        std::shared_ptr<Block> body = blockStack.top();
        blockStack.pop();

        $$ = std::make_shared<While>($4, body);
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                             PRINT                              |
print
    : PRINT LPAR expr RPAR SEMICOLON
    //+ rule body begin
    {
        $$ = std::make_shared<Print>($3);
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                             RETURN                             |
return
    : RETURN expr SEMICOLON
    //+ rule body begin
    {
        $$ = std::make_shared<Return>($2);
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                             OUTPUT                             |
output 
    : OUTPUTADD LPAR IDENTIFIER RPAR SEMICOLON
    //+ rule body begin
    {
        nameStack.push($3);
        std::shared_ptr<Getter> getter = std::make_shared<Getter>(blockStack.top(), nameStack);
        nameStack.pop();
        $$ = std::make_shared<OutputAdd>(blockStack.top(), getter);
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                              MESH                              |
mesh
    : MESHADDFACE LPAR expr COMMA expr COMMA expr RPAR SEMICOLON
    //+ rule body begin
    {
        $$ = std::make_shared<MeshAddFace>($3, $5, $7);
    }
    //+ rule body end
    | MESHADDFACE LPAR expr COMMA expr COMMA expr COMMA expr RPAR SEMICOLON
    //+ rule body begin
    {
        $$ = std::make_shared<MeshAddFace>($3, $5, $7, $9);
    }
    //+ rule body end
    | MESHADDVERTEX LPAR multipleFunctions RPAR SEMICOLON
    //+ rule body begin
    {
        $$ = std::make_shared<MeshAddVertex>(functionVector);

        std::vector<std::shared_ptr<Function>> newfunctionVector;
        functionVector.swap(newfunctionVector);
    }
    //+ rule body end

multipleFunctions
    : multilevelIdentifier COMMA multipleFunctions
    //+ rule body begin
    {
        functionVector.push_back($1);
    }
    //+ rule body end
    | multilevelIdentifier
    //+ rule body begin
    {
        functionVector.push_back($1);
    }
    //+ rule body end

//+----------------------------------------------------------------+
//|                           EXPRESSION                           |
expr
    : multilevelIdentifier  {$$ = $1;                                                                           }
    | genericNullary        {$$ = $1;                                                                           }
    | OUTPUT 				{$$ = nullptr;                                                                      }
    | OUTPUTEMPTY LPAR RPAR {$$ = std::make_shared<OutputEmpty>(blockStack.top());                              }
    | rotate                {$$ = $1;                                                                           }
    | comparison            {$$ = $1;                                                                           }
    | expr DIVIDE expr      {$$ = std::make_shared<Operator>($1, $3, Operator::DIVIDE);                         }
    | expr MULTIPLY expr    {$$ = std::make_shared<Operator>($1, $3, Operator::MULTIPLY);                       }
    | expr PLUS expr        {$$ = std::make_shared<Operator>($1, $3, Operator::PLUS);                           }
    | expr MINUS expr       {$$ = std::make_shared<Operator>($1, $3, Operator::MINUS);                          }
    | expr AND expr         {$$ = std::make_shared<Compare>($1, $3, Compare::AND);                              }
    | expr OR expr          {$$ = std::make_shared<Compare>($1, $3, Compare::OR);                               }
    | LPAR expr RPAR        {$$ = $2;                                                                           }
    | increment             {$$ = $1;                                                                           }
    | RANDOMFLOAT LPAR FLOAT RPAR
    //+ rule body begin
    {
        std::shared_ptr<Nullary> nullary = std::make_shared<Nullary>(Nullary::Type::FLOAT, nullptr);
        nullary->setRandom(true, $3);
        $$ = nullary;
    }
    //+ rule body end
    | RANDOMINT LPAR INT RPAR
    //+ rule body begin
    {
        std::shared_ptr<Nullary> nullary = std::make_shared<Nullary>(Nullary::Type::INT, nullptr);
        nullary->setRandom(true, (float)$3);
        $$ = nullary;
    }
    //+ rule body end

rotate
    : ROTATE LPAR multilevelIdentifier COMMA multilevelIdentifier COMMA multilevelIdentifier RPAR
    //+ rule body begin
    {
        $$ = std::make_shared<Rotate>(std::static_pointer_cast<Getter>($3), std::static_pointer_cast<Getter>($5), std::static_pointer_cast<Getter>($7));
    }
    //+ rule body end

increment
    : multilevelIdentifier PLUSPLUS
    //+ rule body begin
    {
        std::shared_ptr<Nullary> one = std::make_shared<Nullary>(Nullary::Type::INT, std::make_shared<int>(1));
        std::shared_ptr<Operator> incrementOperator = std::make_shared<Operator>($1, one, Operator::PLUS);
        std::shared_ptr<Setter> setter = std::make_shared<Setter>(blockStack.top(), std::static_pointer_cast<Getter>($1),incrementOperator);
        $$ = setter;
    }
    //+ rule body end

genericNullary
    : INT                   {$$ = std::make_shared<Nullary>(Nullary::Type::INT, std::make_shared<int>($1));            }
    | FLOAT                 {$$ = std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>($1));        }
    | STRING                {$$ = std::make_shared<Nullary>(Nullary::Type::STRING, std::make_shared<std::string>($1)); }
    | BOOL                  {$$ = std::make_shared<Nullary>(Nullary::Type::BOOL, std::make_shared<bool>($1));          }

multilevelIdentifier
    : multilevelIdentifierMaker
    //+ rule body begin
    {
        std::shared_ptr<Getter> getter = std::make_shared<Getter>(blockStack.top(), nameStack);

        std::stack<std::string> newNameStack;
        nameStack.swap(newNameStack);

        $$ = getter;
    }
    //+ rule body end

multilevelIdentifierMaker
    : IDENTIFIER                                { nameStack.push($1); }
    | IDENTIFIER DOT multilevelIdentifierMaker  { nameStack.push($1); }

comparison
    : comparisonIdentifierOrNullary EQUALITY        comparisonIdentifierOrNullary       {$$ = std::make_shared<Compare>($1, $3, Compare::EQUALITY);     }
    | comparisonIdentifierOrNullary NEQUALITY       comparisonIdentifierOrNullary       {$$ = std::make_shared<Compare>($1, $3, Compare::NEQUALITY);    }
    | comparisonIdentifierOrNullary LESS            comparisonIdentifierOrNullary       {$$ = std::make_shared<Compare>($1, $3, Compare::LESS);         }
    | comparisonIdentifierOrNullary LESSEQUAL       comparisonIdentifierOrNullary       {$$ = std::make_shared<Compare>($1, $3, Compare::LESSEQUAL);    }
    | comparisonIdentifierOrNullary GREATER         comparisonIdentifierOrNullary       {$$ = std::make_shared<Compare>($1, $3, Compare::GREATER);      }
    | comparisonIdentifierOrNullary GREATEREQUAL    comparisonIdentifierOrNullary       {$$ = std::make_shared<Compare>($1, $3, Compare::GREATEREQUAL); }

comparisonIdentifierOrNullary
    : multilevelIdentifier {$$ = $1;}
    | genericNullary       {$$ = $1;}

initializerList
    : LBRAC expr COMMA expr COMMA expr RBRAC
    //+ rule body begin
    {
        std::vector<std::shared_ptr<Function>> expressions;
        expressions.push_back($2);
        expressions.push_back($4);
        expressions.push_back($6);
        $$ = std::make_shared<InitializerList>(expressions);
    }
    //+ rule body end
%%