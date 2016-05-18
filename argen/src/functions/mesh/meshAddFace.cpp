#include "meshAddFace.h"
#include <utils/log.h>
#include <utils/globals.h>

#include "../manipulation/getter.h"
#include "../manipulation/operators.h"

MeshAddFace::MeshAddFace(std::shared_ptr<Function> getter1, std::shared_ptr<Function> getter2, std::shared_ptr<Function> getter3):Function(MESHADD) {
	_getter1 = getter1;
	_getter2 = getter2;
	_getter3 = getter3;
	_getter4 = nullptr;
}

MeshAddFace::MeshAddFace(std::shared_ptr<Function> getter1, std::shared_ptr<Function> getter2, std::shared_ptr<Function> getter3, std::shared_ptr<Function> getter4):Function(MESHADD) {
	_getter1 = getter1;
	_getter2 = getter2;
	_getter3 = getter3;
	_getter4 = getter4;
}

MeshAddFace::~MeshAddFace() {
	_getter1.reset();
	_getter1 = nullptr;

	_getter2.reset();
	_getter2 = nullptr;

	_getter3.reset();
	_getter3 = nullptr;

	if (_getter4 != nullptr){
		_getter4.reset();
		_getter4 = nullptr;
	}
}

int MeshAddFace::getOutput() {
	return 0;
}

/*
 * Gets indexes of vertices and adds face made out of these to current mesh
 */
void MeshAddFace::operator()() {
	Log::debug("MeshAddFace::operator()");

	(*_getter1)();
	(*_getter2)();
	(*_getter3)();

	int vertexIndex1 = getIndex(_getter1);
	int vertexIndex2 = getIndex(_getter2);
	int vertexIndex3 = getIndex(_getter3);

	MeshGenerator::currentMesh->addFace(vertexIndex1, vertexIndex2, vertexIndex3);

	if (_getter4 != nullptr) {
		(*_getter4)();
		int vertexIndex4 = getIndex(_getter4);
		
		MeshGenerator::currentMesh->addFace(vertexIndex1, vertexIndex3, vertexIndex4);
	}
}

/*
 * Gets vertice index from getter function
 */
int MeshAddFace::getIndex(std::shared_ptr<Function> getter) {
	std::shared_ptr<Function> function = getter;

	if (function->getName() == Function::GETTER) {
		std::shared_ptr<VarSymPrototype> varSymPrototype = (std::static_pointer_cast<Getter>(function))->getProperty();

		if (varSymPrototype->getClassName() == VarSymPrototype::SYMBOL) {
			Log::error("MeshAddFace invalid type of argument - symbol");
		}
		else {
			function = std::static_pointer_cast<Variable>(varSymPrototype)->getValue();
		}
	}

	if (function->getName() == Function::Name::OPERATOR) {
		std::shared_ptr<VarSymPrototype> varSymPrototype = (std::static_pointer_cast<Operator>(function))->getValue();

		if (varSymPrototype->getClassName() == VarSymPrototype::SYMBOL) {
			Log::error("MeshAddFace invalid type of argument - symbol");
		}
		else {
			function = std::static_pointer_cast<Variable>(varSymPrototype)->getValue();
		}
	}

	if (function->getName() == Function::NULLARY) {
		std::shared_ptr<Nullary> nullary = std::static_pointer_cast<Nullary>(function);

		if (nullary->getType() == Nullary::Type::INT) {
			return *(std::static_pointer_cast<int>(nullary->getValue()));
		}
		else {
			Log::error("MeshAddFace invalid type of argument");
		}
	}
	else {
		Log::error("MeshAddFace invalid argument");
	}
}