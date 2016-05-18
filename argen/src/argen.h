#ifndef ARGEN_H
#define ARGEN_H

#include <stdio.h>
#include <memory>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "tokenList.h"
#include "bison/bison.h"
#include "flex/lex.h"

#include "utils/globals.h"
#include "utils/log.h"

#include "generator/collection/collection.h"
#include "functions/functionList.h"

int yyparse (yyscan_t scanner, std::shared_ptr<Collection> collection);

class Argen
{
	private:
		std::shared_ptr<Collection> _collection; 
	public:
		Argen(std::string);
		~Argen();	

		std::shared_ptr<MeshCollection> getMeshCollection();
};

#endif