#define YYSTYPE token_list

#include <string>

#include <memory>
#include "functions/functionList.h"
#include "generator/collection/collection.h"

class token_list {
    public:
        int ival;
        bool bval;
        float fval;
        std::string sval;
        std::shared_ptr<Function> functionPtr;
        std::shared_ptr<Block> containerPtr;
        std::shared_ptr<InitializerList> initializerListPtr;
};