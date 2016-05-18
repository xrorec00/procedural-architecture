#include "outputAdd.h"
#include <utils/log.h>
#include <utils/globals.h>

OutputAdd::OutputAdd(std::shared_ptr<Block> parent, std::shared_ptr<Getter> getter):Function(OUTPUTADD) {
	_parent = parent;
	_getter = getter;
}

OutputAdd::~OutputAdd() {
	_parent.reset();
	_parent = nullptr;

	_getter.reset();
	_getter = nullptr;
}

int OutputAdd::getOutput() {
	return 0;
}

void OutputAdd::operator()() {
	Log::debug("OutputAdd::operator()");

	(*_getter)();

	std::shared_ptr<Symbol> symbol = std::static_pointer_cast<Symbol>(_getter->getProperty());

	Generator::outSymbolString->addSymbol(symbol);
}