#ifndef MESHADDFACE_H
#define MESHADDFACE_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "../../../libs/glm/glm/vec3.hpp"
#include "../varSym/variable.h"

class MeshAddFace : public Function {
	private:
		std::shared_ptr<Function> _getter1;
		std::shared_ptr<Function> _getter2;
		std::shared_ptr<Function> _getter3;
		std::shared_ptr<Function> _getter4;
	public:
		MeshAddFace(std::shared_ptr<Function>, std::shared_ptr<Function>, std::shared_ptr<Function>);
		MeshAddFace(std::shared_ptr<Function>, std::shared_ptr<Function>, std::shared_ptr<Function>, std::shared_ptr<Function>);
		~MeshAddFace();

		int getOutput();
		void operator()();
	private:
		int getIndex(std::shared_ptr<Function>);
};

#endif