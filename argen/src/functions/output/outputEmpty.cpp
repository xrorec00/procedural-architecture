#include "outputEmpty.h"
#include <utils/globals.h>

OutputEmpty::OutputEmpty(std::shared_ptr<Block> parent):Function(OUTPUTEMPTY) {
	_parent = parent;
}

OutputEmpty::~OutputEmpty() {
	if (_parent != nullptr) {
		_parent.reset();
		_parent = nullptr;
	}
}

void OutputEmpty::operator()() {
	if (Generator::sourceSymbolString->empty()) {
		output = 1;
	}
	else {
		output = 0;
	}
}

int OutputEmpty::getOutput() {
	return output;
}