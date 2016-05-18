#include "symbolCollection.h"

#include <iostream>
#include <sstream>

#include "../../utils/log.h"

SymbolCollection::SymbolCollection() {
	_cursor = 0;
}

void SymbolCollection::addSymbol(std::shared_ptr<Symbol> symbolToAdd) {
	_symbols.push_back(symbolToAdd);
}

std::shared_ptr<Symbol> SymbolCollection::getSymbol(int offset,int index) {
	if(offset+index<_symbols.size()) {
		return _symbols[offset + index];
	}
	else {
		std::stringstream stream;
		stream<<"SymbolCollection: trying to access symbol at index "<<(offset+index)<<" but length is "<<_symbols.size();
		Log::error(stream.str());
	}
}

void SymbolCollection::printInfo() {
	std::cout<<"-----Generated symbols-----"<<std::endl;

	for (auto &symbol : _symbols) {
		symbol->printInfo();
		std::cout<<std::endl;
	}
}

bool SymbolCollection::empty() {
	return _symbols.empty();
}

int SymbolCollection::getSize() {
	return _symbols.size();
}

int SymbolCollection::getCursorPosition() {
	return _cursor;
}

void SymbolCollection::setCursorPosition(int cursorPosition) {
	_cursor = cursorPosition;
}