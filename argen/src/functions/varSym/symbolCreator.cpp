#include "symbolCreator.h"
#include "../../utils/log.h"

SymbolCreator::SymbolCreator(std::shared_ptr<Block> parent, std::shared_ptr<SymCollection> symCollection, std::string type, std::string name, std::shared_ptr<InitializerList> initializerList):Function(SYMBOLCREATOR) {
	_parent = parent;
	_symCollection = symCollection;
	_type = type;
	_name = name;
	_initializerList = initializerList;
}

SymbolCreator::~SymbolCreator() {
	_parent.reset();
	_parent = nullptr;
	
	_symCollection.reset();
	_symCollection = nullptr;
	
	_initializerList.reset();
	_initializerList = nullptr;
}

int SymbolCreator::getOutput() {

}

//needed for symbol template
std::shared_ptr<Symbol> SymbolCreator::getSymbol() {
	Log::debug("SymbolCreator::operator()");

	std::shared_ptr<Symbol> symbol = _symCollection->makeSymbol(_type, _name, _parent);

	if (_initializerList != nullptr) {
		_initializerList->setSourceSymbol(symbol);
		(*_initializerList)();
	}

	if (symbol == nullptr) {
		Log::error("Error in symbol template declaration.");
	}

	return symbol;
}

void SymbolCreator::operator()() {
	Log::debug("SymbolCreator::operator()");

	std::shared_ptr<Symbol> symbol = _symCollection->makeSymbol(_type, _name, _parent);

	if (_initializerList != nullptr) {
		_initializerList->setSourceSymbol(symbol);
		(*_initializerList)();
	}

	_parent->addSymbol(symbol);
}