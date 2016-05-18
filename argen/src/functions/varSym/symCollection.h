#ifndef SYMCOLLECTION_H
#define SYMCOLLECTION_H

#include <memory>
#include <vector>

#include "symbol.h"
#include "../block.h"

class Symbol;
class Block;
class SymCollection{
	private:
		std::vector<std::shared_ptr<Symbol>> _symbolTemplates;

	public:
		std::shared_ptr<Symbol> makeSymbol(std::string, std::string, std::shared_ptr<Block>);
		void addSym(Block*);
		void addTemplate(std::shared_ptr<Symbol>);
};

#endif