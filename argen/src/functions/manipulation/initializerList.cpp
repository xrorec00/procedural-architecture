#include "initializerList.h"

#include "operators.h"

InitializerList::InitializerList(std::vector<std::shared_ptr<Function>> initializerList):Function(INITIALIZER) {
	_initializerList = initializerList;
}

InitializerList::~InitializerList() {
	if (_sourceSymbol != nullptr) {
		_sourceSymbol.reset();
		_sourceSymbol = nullptr;
	}
}

void InitializerList::setSourceSymbol(std::shared_ptr<Symbol> sourceSymbol) {
	_sourceSymbol = sourceSymbol;
}

int InitializerList::getOutput() {
	return 0;
}

void InitializerList::operator()() {
	for (int i = 0; i < _initializerList.size(); i++) {
		(*_initializerList[i])();
				
		if (_initializerList[i]->getName()==Function::Name::NULLARY) {
			std::static_pointer_cast<Variable>(_sourceSymbol->getPrototypeByIndex(i))->setValue(_initializerList[i]);
		}
		else if (_initializerList[i]->getName()==Function::Name::OPERATOR) {
			std::static_pointer_cast<Variable>(_sourceSymbol->getPrototypeByIndex(i))->setValue(std::static_pointer_cast<Variable>(std::static_pointer_cast<Operator>(_initializerList[i])->getValue()));
		}
		else{
			std::shared_ptr<VarSymPrototype> varSymPrototype = std::static_pointer_cast<Getter>(_initializerList[i])->getProperty();
			std::static_pointer_cast<Variable>(_sourceSymbol->getPrototypeByIndex(i))->setValue(std::static_pointer_cast<Variable>(varSymPrototype)->getValue());
		}
	}
};