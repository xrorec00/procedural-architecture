#include "functionPrototype.h"

Function::Function(Name name) {
	this->name = name;
	shouldReturn = false;
}

bool Function::getShouldReturn() {
	return shouldReturn;
}

Function::Name Function::getName() {
	return name;
}