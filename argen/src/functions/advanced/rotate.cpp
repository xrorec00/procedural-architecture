#define PI 3.14159265

#include "rotate.h"

#include <utils/log.h>

#include <cmath>

#include <glm/glm.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

Rotate::Rotate(std::shared_ptr<Getter> source, std::shared_ptr<Getter> anchor, std::shared_ptr<Getter> angle):Function(ROTATE) {
	_source = source;
	_anchor = anchor;
	_angle = angle;
}

Rotate::~Rotate() {
	_source.reset();
	_source = nullptr;

	_anchor.reset();
	_anchor = nullptr;

	_angle.reset();
	_angle = nullptr;

	if (_rotatedVector != nullptr) {
		_rotatedVector.reset();
		_rotatedVector = nullptr;
	}
}

glm::vec3 Rotate::getVectorFromVarSymPrototype(std::shared_ptr<VarSymPrototype> varSym) {
	if (varSym->getClassName() == VarSymPrototype::SYMBOL) {
		if (varSym->getVarSymType() == "vec3") {
			return Symbol::getVec3(std::static_pointer_cast<Symbol>(varSym), varSym->getVarSymName());
		}
		else {
			Log::error("Item in rotate function is not vector");
		}
	}
	else {
		Log::error("Item in rotate function is not symbol-like object");
	}	
}

/*
 * Calculates rotation
 */
void Rotate::operator()() {
	Log::debug("Rotate::operator()");

	(*_source)();
	(*_anchor)();
	(*_angle)();

	glm::vec3 source = getVectorFromVarSymPrototype(_source->getProperty());
	glm::vec3 anchor = getVectorFromVarSymPrototype(_anchor->getProperty());
	glm::vec3 angle = getVectorFromVarSymPrototype(_angle->getProperty());

	glm::vec3 rotatedVector = Rotate::rotate(source, anchor, angle);

	_rotatedVector = std::make_shared<Symbol>(nullptr);
	_rotatedVector->setVarSymType("vec3");
	_rotatedVector->setVarSymName("");

	std::shared_ptr<Variable> v1 = 	
		std::make_shared<Variable>(nullptr, "float", "x", std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(rotatedVector.x)));
	std::shared_ptr<Variable> v2 = 
		std::make_shared<Variable>(nullptr, "float", "y", std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(rotatedVector.y)));
	std::shared_ptr<Variable> v3 = 
		std::make_shared<Variable>(nullptr, "float", "z", std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(rotatedVector.z)));

	_rotatedVector->addVariable(v1);
	_rotatedVector->addVariable(v2);
	_rotatedVector->addVariable(v3);
}

int Rotate::getOutput() {

}

/*
 * Returns rotated vector
 */
std::shared_ptr<Symbol> Rotate::getVector() {
	return _rotatedVector;
}

/*
 * Rotates source vector around anchor vector by certain angle
 */
glm::vec3 Rotate::rotate(glm::vec3 source, glm::vec3 anchor, glm::vec3 angle) {
	glm::lowp_mat4 rotateMatrix = glm::lowp_mat4(
		cos(angle.z*PI/180.0f), -sin(angle.z*PI/180.0f), 0, 0,
		sin(angle.z*PI/180.0f), cos(angle.z*PI/180.0f), 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	);

	glm::vec4 point = glm::vec4(source.x, source.y, source.z, 1);
	glm::vec4 rotated = point*rotateMatrix;

	glm::vec3 rotatedVec3(rotated.x, rotated.y, rotated.z);
	
	return rotatedVec3;
}