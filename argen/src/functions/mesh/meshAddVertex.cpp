#include "meshAddVertex.h"
#include <utils/log.h>
#include <utils/globals.h>

#include "../manipulation/getter.h"

MeshAddVertex::MeshAddVertex(std::vector<std::shared_ptr<Function>> getters):Function(MESHADD) {
	_getters = getters;
}


MeshAddVertex::~MeshAddVertex() {

}

int MeshAddVertex::getOutput() {
	return 0;
}

void MeshAddVertex::operator()() {
	Log::debug("MeshAddVertex::operator()");

	for (int i = _getters.size() - 1; i >= 0; i--) {
		std::shared_ptr<Function> function = _getters.at(i);

		(*function)();

		glm::vec3 vertex = getVec3(function);

		MeshGenerator::currentMesh->addVertex(vertex);
	}
}

glm::vec3 MeshAddVertex::getVec3(std::shared_ptr<Function> getter) {
	std::shared_ptr<Symbol> vertex = std::static_pointer_cast<Symbol>(std::static_pointer_cast<Getter>(getter)->getProperty());

	float x = getFloatFromVariable(std::static_pointer_cast<Variable>(vertex->getPrototypeByIndex(0)));
	float y = getFloatFromVariable(std::static_pointer_cast<Variable>(vertex->getPrototypeByIndex(1)));
	float z = getFloatFromVariable(std::static_pointer_cast<Variable>(vertex->getPrototypeByIndex(2)));

	return glm::vec3(x,y,z);
}

float MeshAddVertex::getFloatFromVariable(std::shared_ptr<Variable> variable) {
	std::shared_ptr<Nullary> nullary = std::static_pointer_cast<Nullary>(variable->getValue());

	if (nullary->getType() == Nullary::Type::INT) {
		return (float)(*(std::static_pointer_cast<int>(nullary->getValue())));
	}
	else if (nullary->getType() == Nullary::Type::FLOAT) {
		return (float)(*(std::static_pointer_cast<float>(nullary->getValue())));
	}
	else {
		Log::error("MeshAddVertex invalid type");
	}
}