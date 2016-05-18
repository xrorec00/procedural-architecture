#include "symbol.h"

#include <iostream>

#include <utils/log.h>

Symbol::Symbol(std::shared_ptr<Block> parent):VarSymPrototype(SYMBOL) {
	_parent=parent;
}

Symbol::~Symbol() {
	if (_parent != nullptr) {
		_parent.reset();
		_parent = nullptr;
	}
}

std::shared_ptr<VarSymPrototype> Symbol::getPrototype(std::stack<std::string> nameStack) {
	std::string name = nameStack.top();
	nameStack.pop();

	std::stack<std::string> newNameStack(nameStack);

	Log::debug("Symbol getPrototype: " + name);

	std::shared_ptr<VarSymPrototype> prototype = nullptr;

	while (1) {
		for (auto &varSymPrototype : _varSymPrototypes) {
			if (varSymPrototype->getVarSymName() == name) {
				prototype = varSymPrototype;
			}
		}

		if (nameStack.empty()) {
			if (prototype != nullptr) {
				return std::static_pointer_cast<VarSymPrototype>(prototype);
			}
			else {
				break;
			}
		}
		else if (prototype != nullptr) {
			return std::static_pointer_cast<Symbol>(prototype)->getPrototype(newNameStack);
		}
		else {
			return nullptr;
		}
	}

	Log::debug("Symbol getPrototype: " + name + " - not found");

	return nullptr;
}

std::shared_ptr<VarSymPrototype> Symbol::getPrototypeByIndex(int index) const {
	return _varSymPrototypes[index];
}

std::vector<std::shared_ptr<VarSymPrototype>> Symbol::getVarSymPrototypes() {
	return _varSymPrototypes;
}

std::shared_ptr<Symbol> Symbol::makeCopy(std::shared_ptr<Block> parent) {
	Log::debug("Symbol::makeCopy");

	std::shared_ptr<Symbol> copySymbol = std::make_shared<Symbol>(nullptr);
	copySymbol->setVarSymType(_varSymType);
	copySymbol->setVarSymName(_varSymName);

	for (auto varSymPrototype : _varSymPrototypes) {
		if (varSymPrototype->getClassName() == VarSymPrototype::VARIABLE) {
			copySymbol->_varSymPrototypes.push_back(std::static_pointer_cast<Variable>(varSymPrototype)->makeCopy());
		}
		else if (varSymPrototype->getClassName() == VarSymPrototype::SYMBOL) {
			copySymbol->_varSymPrototypes.push_back(std::static_pointer_cast<Symbol>(varSymPrototype)->makeCopy(nullptr));
		}
		else {
			std::cout<<"x"<<varSymPrototype->getClassName()<<"x"<<std::flush;
			Log::error("Error making symbol. Unknown type: " + varSymPrototype->getClassName());
		}
	}

	return copySymbol;
}

void Symbol::addVariable(std::shared_ptr<Variable> variable) {
	_varSymPrototypes.push_back(variable);
}

void Symbol::addSymbol(std::shared_ptr<Symbol> symbol) {
	_varSymPrototypes.push_back(symbol);
}

void Symbol::setValue(std::shared_ptr<Symbol> symbol) {
	std::vector<std::shared_ptr<VarSymPrototype>> newVarSymPrototypes;

	Log::debug("Symbol::setValue");

	for (auto varSymPrototype : symbol->getVarSymPrototypes()) {
		if (varSymPrototype->getClassName() == VarSymPrototype::VARIABLE) {
			newVarSymPrototypes.push_back(std::static_pointer_cast<Variable>(varSymPrototype)->makeCopy());
		}
		else {
			newVarSymPrototypes.push_back(std::static_pointer_cast<Symbol>(varSymPrototype)->makeCopy(nullptr));
		}
	}

	_varSymPrototypes.swap(newVarSymPrototypes);
}

glm::vec3 Symbol::getVec3ByName(std::string vecName) {
	glm::vec3 positionVec;

	for (int i = 0; i < _varSymPrototypes.size(); i++) {
		if (_varSymPrototypes[i]->getClassName() == VarSymPrototype::SYMBOL && _varSymPrototypes[i]->getVarSymName() == vecName) {
			return Symbol::getVec3(std::static_pointer_cast<Symbol>(_varSymPrototypes[i]), vecName);
		}
	}

	Log::error("Cannot handle vec3 " + vecName);
}

glm::vec3 Symbol::getVec3(std::shared_ptr<Symbol> symbol, std::string vecName) {
	glm::vec3 vector;

	for (int j = 0; j < 3; j++) {
		std::shared_ptr<Variable> variable = std::static_pointer_cast<Variable>(symbol->getPrototypeByIndex(j));
		std::shared_ptr<Nullary> nullary = std::static_pointer_cast<Nullary>(variable->getValue());

		if (nullary->getType() == Nullary::INT) {
			vector[j] = (float)(*(std::static_pointer_cast<int>(nullary->getValue())));
		}
		else if (nullary->getType() == Nullary::FLOAT) {
			vector[j] = *(std::static_pointer_cast<float>(nullary->getValue()));
		}
		else {
			Log::error("Invalid item in vector " + vecName);
		}
	}

	return vector;
}

std::shared_ptr<Symbol> Symbol::mathOperator(const Symbol symbol, OperatorType type) const {
	if (_varSymType != symbol.getVarSymType()) {
		Log::error("Cannot perform operation with symbols of type " + _varSymType + " and " + symbol.getVarSymType());
	}

	std::shared_ptr<Symbol> newSymbol = std::make_shared<Symbol>(nullptr);

	newSymbol->setVarSymType(_varSymType);

	for (int i = 0; i < _varSymPrototypes.size(); i++) {
		if (_varSymPrototypes[i]->getClassName() == VarSymPrototype::VARIABLE) {
			std::shared_ptr<Variable> variable = nullptr;

			switch(type) {
				case PLUS:
					variable = *std::static_pointer_cast<Variable>(_varSymPrototypes[i]) + *std::static_pointer_cast<Variable>(symbol.getPrototypeByIndex(i));
					break;
				case MINUS:
					variable = *std::static_pointer_cast<Variable>(_varSymPrototypes[i]) - *std::static_pointer_cast<Variable>(symbol.getPrototypeByIndex(i));
					break;
				case MULTIPLY:
					variable = *std::static_pointer_cast<Variable>(_varSymPrototypes[i]) * *std::static_pointer_cast<Variable>(symbol.getPrototypeByIndex(i));
					break;
				case DIVIDE:
					variable = *std::static_pointer_cast<Variable>(_varSymPrototypes[i]) / *std::static_pointer_cast<Variable>(symbol.getPrototypeByIndex(i));
					break;
			}
			
			variable->setVarSymName(_varSymPrototypes[i]->getVarSymName());
			variable->setVarSymType(_varSymPrototypes[i]->getVarSymType());
			newSymbol->addVariable(variable);
		}
	}

	return newSymbol;
}

void Symbol::printInfo() {
	std::cout<<"Symbol: "<<_varSymName<<std::endl;

	for (auto varSymPrototype:_varSymPrototypes) {
		if (varSymPrototype->getClassName()==VarSymPrototype::VARIABLE) {
			std::static_pointer_cast<Variable>(varSymPrototype)->printInfo();
		}
		else {
			std::static_pointer_cast<Symbol>(varSymPrototype)->printInfo();
		}
	}
}

int Symbol::getVarSymPrototypesSize() {
	return _varSymPrototypes.size();
}

std::shared_ptr<Symbol> Symbol::operator +(const Symbol symbol) {
	return mathOperator(symbol,VarSymPrototype::PLUS);
}
std::shared_ptr<Symbol> Symbol::operator -(const Symbol symbol) {
	return mathOperator(symbol,VarSymPrototype::MINUS);
}
std::shared_ptr<Symbol> Symbol::operator *(const Symbol symbol) {
	return mathOperator(symbol,VarSymPrototype::MULTIPLY);
}
std::shared_ptr<Symbol> Symbol::operator /(const Symbol symbol) {
	return mathOperator(symbol,VarSymPrototype::DIVIDE);
}