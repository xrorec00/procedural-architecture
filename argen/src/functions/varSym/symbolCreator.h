#ifndef SYMBOLCREATOR_H
#define SYMBOLCREATOR_H

#include <string>
#include <memory>

#include "varSymPrototype.h"
#include "../block.h"
#include "../manipulation/initializerList.h"
#include "symCollection.h"

class SymCollection;
class InitializerList;
class Block;
class SymbolCreator: public Function {
	public:
		SymbolCreator(std::shared_ptr<Block>, std::shared_ptr<SymCollection>, std::string, std::string, std::shared_ptr<InitializerList>);
		~SymbolCreator();

		std::shared_ptr<Symbol> getSymbol();

		int getOutput();
		void operator()();
	private:
		std::shared_ptr<Block> _parent;
		std::shared_ptr<SymCollection> _symCollection;
		std::string _type;
		std::string _name;
		std::shared_ptr<InitializerList> _initializerList;
};

#endif