#include "print.h"
#include "../varSym/varSymPrototype.h"
#include "../manipulation/operators.h"
#include "../manipulation/compare.h"
#include <utils/log.h>

Print::Print(std::shared_ptr<Function> function):Function(PRINT) {
	_function = function;
}

void Print::operator()() {
	Log::debug("Print::operator() ()\tFunction type: " + _function->getName());

	(*_function)();

	VarSymPrototype* varSymPrototype;
	if (_function->getName() == Function::Name::NULLARY) {
		std::static_pointer_cast<Nullary>(_function)->printInfo();
	}
	else if (_function->getName() == Function::Name::OPERATOR) {
		std::static_pointer_cast<Operator>(_function)->getValue()->printInfo();
	}
	else if (_function->getName() == Function::Name::VARIABLE) {
		std::static_pointer_cast<Variable>(_function)->printInfo();
	}
	else if (_function->getName() == Function::Name::GETTER) {
		std::static_pointer_cast<Getter>(_function)->getProperty()->printInfo();
	}
	else if (_function->getName() == Function::Name::COMPARISON) {
		int output = std::static_pointer_cast<Compare>(_function)->getOutput();
	}
	else{
		Log::error("Invalid print argument.");
	}
}

int Print::getOutput() {
	return 0;
}