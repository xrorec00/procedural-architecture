#ifndef SYMBOLCOLLECTION_H
#define SYMBOLCOLLECTION_H

#include <memory>
#include <vector>

#include "../../functions/varSym/symbol.h"

class SymbolCollection {
	protected:
		std::vector<std::shared_ptr<Symbol>> _symbols;
		int _cursor;
	public:
		SymbolCollection();
		
		void addSymbol(std::shared_ptr<Symbol>);
		std::shared_ptr<Symbol> getSymbol(int, int);

		void printInfo();
		bool empty();

		int getSize();
		int getCursorPosition();
		void setCursorPosition(int);
};

#endif