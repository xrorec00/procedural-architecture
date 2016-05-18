#include "variable.h"
#include <utils/log.h>
#include "../manipulation/operators.h"

#include <iostream>
#include <sstream>

Variable::Variable(std::shared_ptr<Block> parent, std::string varSymType, std::string varSymName, std::shared_ptr<Function> value):Function(Function::VARIABLE),VarSymPrototype(VarSymPrototype::VARIABLE) {
	_parent = parent;
	_varSymType = varSymType;
	_varSymName = varSymName;
	
	setValue(value);
}

Variable::Variable(std::shared_ptr<Block> parent, std::string varSymType, std::string varSymName):Function(Function::VARIABLE),VarSymPrototype(VarSymPrototype::VARIABLE) {
	_parent = parent;
	_varSymType = varSymType;
	_varSymName = varSymName;

	if (varSymType == "int") {
		_value = std::make_shared<Nullary>(Nullary::Type::INT, std::make_shared<int>(0));
	}
    else if (varSymType == "float") {
    	_value = std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(0.0f));
    }
    else if (varSymType == "string") {
    	_value = std::make_shared<Nullary>(Nullary::Type::STRING, std::make_shared<std::string>(""));
    }
    else {
    	Log::error("Cannot create inital value for variable of type " + varSymType);
    }
}

Variable::Variable():Function(Function::VARIABLE),VarSymPrototype(VarSymPrototype::VARIABLE) {
	_parent = nullptr;
	_varSymType = "";
	_varSymName = "";
	_value = nullptr;
}

Variable::~Variable() {
	if (_value != nullptr) {
		_value.reset();
		_value = nullptr;
	}
	if (_parent != nullptr) {
		_parent.reset();
		_parent = nullptr;
	}
}

int Variable::getOutput() {
	return _value->getOutput();
}

std::shared_ptr<Variable> Variable::makeCopy() {
	std::stringstream stream;
	stream<<"Variable makeCopy()"<<" name: "<<_varSymName<<" type: "<<_varSymType;
	Log::debug(stream.str());

	return std::make_shared<Variable>(*this);
}

void Variable::operator()() {
	Log::debug("Variable::operator()");

	if (_value != nullptr) {
		(*_value)();

		if (_value->getName() == Function::GETTER) {
			std::shared_ptr<VarSymPrototype> property = std::static_pointer_cast<Getter>(_value)->getProperty();

			if (property->getClassName() == VarSymPrototype::VARIABLE) {
				setValue(std::static_pointer_cast<Variable>(property)->getValue());
			}
			else{
				Log::error("Invalid variable value to be set.");
			}
		}
	}

	try {
		_parent->addVariable(shared_from_this());
    } catch(const std::bad_weak_ptr& e) {
        std::cout<<e.what()<<" in Variable::operator()\n";
    }
}

void Variable::printInfo() {
	std::cout<<"Variable: "<<_varSymName<<"\tvalue: ";
	
	if (_value != nullptr) {
		std::static_pointer_cast<Nullary>(getValue())->printInfo();
	}
	else {
		std::cout<<"uninitialized"<<std::endl<<std::flush;
	}
}

std::shared_ptr<Function> Variable::getValue() const {
	Log::debug("Variable::getValue\tType: " + _value->getName());

	if (_value == nullptr) {
		Log::error("Variable getValue - null value.");
	}

	if (_value->getName() == Function::OPERATOR) {
		std::shared_ptr<VarSymPrototype> test = std::static_pointer_cast<Operator>(_value)->getValue();
		std::shared_ptr<Nullary> test2 = std::static_pointer_cast<Nullary>((std::static_pointer_cast<Variable>(test))->getValue());
		return test2;
	}
	else {
		return _value;
	}
}

void Variable::setValue(std::shared_ptr<Function> value) {
	if (_value != nullptr) {
		_value.reset();
	}

	if (value->getName() == Function::NULLARY) {
		if (std::static_pointer_cast<Nullary>(value)->getType() == Nullary::Type::INT && _varSymType=="int") {
			_value = value;
		}
		else if (std::static_pointer_cast<Nullary>(value)->getType() == Nullary::Type::FLOAT && _varSymType=="float") {
			_value = value;
		}
		else if (std::static_pointer_cast<Nullary>(value)->getType() == Nullary::Type::STRING && _varSymType=="string") {
			_value = value;
		}
		else {
			std::cout<<std::static_pointer_cast<Nullary>(value)->getType();
			Log::error("Trying to assign invalid value into variable \"" + _varSymName + "\" of type " + _varSymType);
		}
	}
	else {
		_value = value;
	}
}

void Variable::setVarSymType(std::string varSymType) {
	_varSymType = varSymType;
}

std::string Variable::getVarSymType() const {
	return _varSymType;
}

std::shared_ptr<Variable> Variable::mathOperator(const Variable variable,const Variable var2, OperatorType type) const {
	return mathOperatorNullary(variable, *std::static_pointer_cast<Nullary>(var2.getValue()), type);
}

std::shared_ptr<Variable> Variable::mathOperatorNullary(const Variable variable, Nullary nullary2, OperatorType type) const {
	std::shared_ptr<Variable> newVariable = std::make_shared<Variable>();

	Nullary nullary1 = *std::static_pointer_cast<Nullary>(variable.getValue());
	
	std::shared_ptr<Nullary> value = nullptr;

	switch (type) {
		case PLUS:
			value = nullary1 + nullary2;
			break;
		case MINUS:
			value = nullary1 - nullary2;
			break;
		case MULTIPLY:
			value = nullary1 * nullary2;
			break;
		case DIVIDE:
			value = nullary1 / nullary2;
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
		return nullptr;
	}
}

std::shared_ptr<Variable> Variable::operator +(Variable variable) {
	return mathOperator(*this, variable, VarSymPrototype::PLUS);
}
std::shared_ptr<Variable> Variable::operator -(Variable variable) {
	return mathOperator(*this, variable, VarSymPrototype::MINUS);
}
std::shared_ptr<Variable> Variable::operator *(Variable variable) {
	return mathOperator(*this, variable, VarSymPrototype::MULTIPLY);
}
std::shared_ptr<Variable> Variable::operator /(Variable variable) {
	return mathOperator(*this, variable, VarSymPrototype::DIVIDE);
}

std::shared_ptr<Variable> Variable::operator +(Nullary function) {
	return mathOperatorNullary(*this, function, VarSymPrototype::PLUS);
}
std::shared_ptr<Variable> Variable::operator -(Nullary function) {
	return mathOperatorNullary(*this, function, VarSymPrototype::MINUS);
}
std::shared_ptr<Variable> Variable::operator *(Nullary function) {
	return mathOperatorNullary(*this, function, VarSymPrototype::MULTIPLY);
}
std::shared_ptr<Variable> Variable::operator /(Nullary function) {
	return mathOperatorNullary(*this, function, VarSymPrototype::DIVIDE);
}