#ifndef MESHADDVERTEX_H
#define MESHADDVERTEX_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "../../../libs/glm/glm/vec3.hpp"
#include "../varSym/variable.h"

class MeshAddVertex : public Function {
	private:
		std::vector<std::shared_ptr<Function>> _getters;
	public:
		MeshAddVertex(std::vector<std::shared_ptr<Function>>);
		~MeshAddVertex();

		int getOutput();
		void operator()();
	private:
		glm::vec3 getVec3(std::shared_ptr<Function>);
		float getFloatFromVariable(std::shared_ptr<Variable>);
};

#endif