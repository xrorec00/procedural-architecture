#ifndef INITIALIZER_LIST_H
#define INITIALIZER_LIST_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "../varSym/symbol.h"
#include "../varSym/variable.h"

class Symbol;
class InitializerList: public Function {
	private:
		std::vector<std::shared_ptr<Function>> _initializerList;
		std::shared_ptr<Symbol> _sourceSymbol;
	public:
		InitializerList(std::vector<std::shared_ptr<Function>>);
		~InitializerList();

		void setSourceSymbol(std::shared_ptr<Symbol> sourceSymbol);

		int getOutput();
		void operator()();
};

#endif