#include "getter.h"
#include <utils/log.h>

std::shared_ptr<VarSymPrototype> Getter::getProperty() {
	return _propertyToReturn;
}

Getter::Getter(std::shared_ptr<Block> parent,std::stack<std::string> nameStack):Function(GETTER) {
	this->parent = parent;
	this->nameStack = nameStack;
}

Getter::~Getter() {
	parent.reset();
	parent = nullptr;
}

int Getter::getOutput() {
	return 0;
}

/*
 * Gets prototype (variable or symbol) from parent block
 */
void Getter::operator()() {
	Log::debug("Getter::operator()");
	_propertyToReturn = parent->getPrototype(nameStack);
}