#include "interpretation.h"

Interpretation::Interpretation():Block(nullptr) {

}

void Interpretation::setSymbolType(std::string type) {
	_symbolType = type;
}

std::string Interpretation::getSymbolType() {
	return _symbolType;
}

void Interpretation::addSourceSymbol(std::shared_ptr<Symbol> source) {
	_property = source->makeCopy(nullptr);
}