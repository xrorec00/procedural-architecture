#include "operators.h"

#include <utils/log.h>

Operator::Operator(std::shared_ptr<Function> function1, std::shared_ptr<Function> function2, OperatorType type):Function(OPERATOR) {
	Log::debug("Operator::Operator");

	_function1 = function1;
	_function2 = function2;
	_type = type;
}

Operator::~Operator() {
	_function1.reset();
	_function1 = nullptr;

	_function2.reset();
	_function2 = nullptr;
}

std::shared_ptr<VarSymPrototype> Operator::getValue() {
	Log::debug("Operator::getValue()");

	return _value;
}

int Operator::getOutput() {
	Log::debug("Operator::getOutput()");

	return 0;
}

void Operator::operator()() {
	Log::debug("Operator::operator()");

	(*_function1)();
	(*_function2)();

	std::shared_ptr<VarSymPrototype> val1VarSymPrototype = nullptr;
	std::shared_ptr<VarSymPrototype> val2VarSymPrototype = nullptr;

	if (_function1->getName() == Function::Name::GETTER) {
		val1VarSymPrototype = (std::static_pointer_cast<Getter>(_function1))->getProperty();
	}
	if (_function2->getName() == Function::Name::GETTER) {
		val2VarSymPrototype = (std::static_pointer_cast<Getter>(_function2))->getProperty();
	}

	if (_function1->getName() == Function::Name::OPERATOR) {
		val1VarSymPrototype = (std::static_pointer_cast<Operator>(_function1))->getValue();
	}
	if (_function2->getName() == Function::Name::OPERATOR) {
		val2VarSymPrototype = (std::static_pointer_cast<Operator>(_function2))->getValue();
	}

	if (val1VarSymPrototype != nullptr &&
		val2VarSymPrototype != nullptr &&
		val1VarSymPrototype->getClassName() == VarSymPrototype::VARIABLE &&
		val2VarSymPrototype->getClassName() == VarSymPrototype::VARIABLE) {
		_value = handleOperation(std::static_pointer_cast<Variable>(val1VarSymPrototype), std::static_pointer_cast<Variable>(val2VarSymPrototype));
	}
	else if (_function1->getName() == Function::Name::NULLARY &&
			val2VarSymPrototype != nullptr &&
			val2VarSymPrototype->getClassName() == VarSymPrototype::VARIABLE) {
		_value = handleOperation(std::static_pointer_cast<Nullary>(_function1), std::static_pointer_cast<Variable>(val2VarSymPrototype));
	}
	else if (val1VarSymPrototype != nullptr &&
			val1VarSymPrototype->getClassName() == VarSymPrototype::VARIABLE &&
			_function2->getName() == Function::Name::NULLARY) {
		_value = handleOperation(std::static_pointer_cast<Variable>(val1VarSymPrototype), std::static_pointer_cast<Nullary>(_function2));
	}
	else if (_function1->getName() == Function::Name::NULLARY && _function2->getName() == Function::Name::NULLARY) {
		_value = handleOperation(std::static_pointer_cast<Nullary>(_function1), std::static_pointer_cast<Nullary>(_function2));
	}
	else if (val1VarSymPrototype != nullptr &&
			val2VarSymPrototype != nullptr &&
			val1VarSymPrototype->getClassName() == VarSymPrototype::SYMBOL && val2VarSymPrototype->getClassName() == VarSymPrototype::SYMBOL) {
		_value = handleOperation(std::static_pointer_cast<Symbol>(val1VarSymPrototype), std::static_pointer_cast<Symbol>(val2VarSymPrototype));
	}
	else {
		Log::error("Invalid operands.");
	}
}

std::shared_ptr<VarSymPrototype> Operator::handleOperation(std::shared_ptr<Variable> var1, std::shared_ptr<Variable> var2) {
	switch (_type) {
		case PLUS:
			return (*var1) + (*var2);
		case MINUS:
			return (*var1) - (*var2);
		case MULTIPLY:
			return (*var1) * (*var2);
		case DIVIDE:
			return (*var1) / (*var2);
	}
}

std::shared_ptr<VarSymPrototype> Operator::handleOperation(std::shared_ptr<Nullary> nul, std::shared_ptr<Variable> var) {
	return handleOperation(nul, std::static_pointer_cast<Nullary>(var->getValue()));
}

std::shared_ptr<VarSymPrototype> Operator::handleOperation(std::shared_ptr<Variable> var, std::shared_ptr<Nullary> nul) {
	switch (_type) {
		case PLUS:
			return (*var) + (*nul);
		case MINUS:
			return (*var) - (*nul);
		case MULTIPLY:
			return (*var) * (*nul);
		case DIVIDE:
			return (*var) / (*nul);
	}
}

std::shared_ptr<VarSymPrototype> Operator::handleOperation(std::shared_ptr<Nullary> nullary1, std::shared_ptr<Nullary> nullary2) {
	std::shared_ptr<Variable> newVariable = std::make_shared<Variable>();

	std::shared_ptr<Nullary> value = nullptr;

	switch (_type) {
		case PLUS:
			value = (*nullary1) + (*nullary2);
			break;
		case MINUS:
			value = (*nullary1) - (*nullary2);
			break;
		case MULTIPLY:
			value = (*nullary1) * (*nullary2);
			break;
		case DIVIDE:
			value = (*nullary1) / (*nullary2);
			break;
	}

	if (value != nullptr) {
		if (value->getType() == Nullary::Type::INT) {
			newVariable->setVarSymType("int");
		}
		else if (value->getType() == Nullary::Type::FLOAT) {
			newVariable->setVarSymType("float");
		}
		else if (value->getType() == Nullary::Type::STRING) {
			newVariable->setVarSymType("string");
		}
		else {
			Log::error("Invalid operands.");
		}

		newVariable->setValue(value);

		return newVariable;
	}
	else {
		Log::error("Invalid operands when handling operation.");
	}
}

std::shared_ptr<VarSymPrototype> Operator::handleOperation(std::shared_ptr<Symbol> sym1, std::shared_ptr<Symbol> sym2) {
	switch (_type) {
		case PLUS:
			return (*sym1) + (*sym2);
		case MINUS:
			return (*sym1) - (*sym2);
		case MULTIPLY:
			return (*sym1) * (*sym2);
		case DIVIDE:
			return (*sym1) / (*sym2);
	}
}