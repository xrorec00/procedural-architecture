#include "setter.h"
#include <utils/log.h>
#include "operators.h"
#include "../advanced/rotate.h"

Setter::Setter(std::shared_ptr<Block> parent, std::shared_ptr<Getter> varSym1, std::shared_ptr<Function> varSym2):Function(VARIABLESETTER) {
	_parent = parent;
	_varSym1 = varSym1;
	_varSym2 = varSym2;
}

Setter::~Setter() {
	_parent.reset();
	_parent = nullptr;

	_varSym1.reset();
	_varSym1 = nullptr;

	_varSym2.reset();
	_varSym2 = nullptr;
}

void Setter::operator()() {
	Log::debug("Setter::operator()");

	(*_varSym1)();
	(*_varSym2)();
	std::shared_ptr<VarSymPrototype> variableSymbol1 = _varSym1->getProperty();
	std::shared_ptr<VarSymPrototype> variableSymbol2 = nullptr;

	if (_varSym2->getName() == Function::NULLARY) {
		if (variableSymbol1->getClassName() == VarSymPrototype::VARIABLE) {
			std::static_pointer_cast<Variable>(variableSymbol1)->setValue(_varSym2);
			return;
		}
		else {
			Log::error("Setter () - left operator is not valid");
		}
	}

	if (_varSym2->getName() == Function::GETTER) {
		variableSymbol2 = std::static_pointer_cast<Getter>(_varSym2)->getProperty();		
	}
	else if (_varSym2->getName() == Function::ROTATE) {
		variableSymbol2 = std::static_pointer_cast<Rotate>(_varSym2)->getVector();
	}
	else if (_varSym2->getName() == Function::OPERATOR) {
		variableSymbol2 = std::static_pointer_cast<Operator>(_varSym2)->getValue();
	}
	else {
		Log::error("Setter () - failed (unknown type)");
	}

	if (variableSymbol2!=nullptr) {
		if (variableSymbol1->getClassName() == VarSymPrototype::VARIABLE && variableSymbol2->getClassName() == VarSymPrototype::VARIABLE)
			std::static_pointer_cast<Variable>(variableSymbol1)->setValue(std::static_pointer_cast<Variable>(variableSymbol2)->getValue());
		else if (variableSymbol1->getClassName() == VarSymPrototype::SYMBOL && variableSymbol2->getClassName() == VarSymPrototype::SYMBOL)
			std::static_pointer_cast<Symbol>(variableSymbol1)->setValue(std::static_pointer_cast<Symbol>(variableSymbol2));
		else {
			Log::error("Setter () - failed (unknown type)");
		}
	}
	else{
		Log::error("Setter () - failed");
	}
}

int Setter::getOutput() {
	return 0;
}