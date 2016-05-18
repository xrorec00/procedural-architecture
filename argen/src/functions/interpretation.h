#ifndef INTERPRETATION_H
#define INTERPRETATION_H

#include <memory>
#include <string>

#include "block.h"
#include "varSym/symbol.h"

class Interpretation : public Block {
	private:
		std::string _symbolType;
	public:
		Interpretation();
		void setSymbolType(std::string);
		std::string getSymbolType();

		void addSourceSymbol(std::shared_ptr<Symbol>);
};

#endif