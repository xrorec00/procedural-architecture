#include "varSymPrototype.h"

VarSymPrototype::VarSymPrototype(VarSymPrototype::ClassName name) {
	_className = name;
}

VarSymPrototype::ClassName VarSymPrototype::getClassName() {
	return _className;
}


std::string VarSymPrototype::getVarSymName() const {
	return _varSymName;
}

void VarSymPrototype::setVarSymName(std::string name) {
	_varSymName = name;
}


std::string VarSymPrototype::getVarSymType() const {
	return _varSymType;
}

void VarSymPrototype::setVarSymType(std::string type) {
	_varSymType = type;
}
