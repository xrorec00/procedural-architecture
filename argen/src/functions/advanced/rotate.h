#ifndef ROTATE_H
#define ROTATE_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "../manipulation/getter.h"
#include "../varSym/symbol.h"

#include "../../../libs/glm/glm/vec3.hpp"

class Rotate: public Function {
	private:
		std::shared_ptr<Getter> _source;
		std::shared_ptr<Getter> _anchor;
		std::shared_ptr<Getter> _angle;

		std::shared_ptr<Symbol> _rotatedVector;

		glm::vec3 getVectorFromVarSymPrototype(std::shared_ptr<VarSymPrototype>);
	public:
		Rotate(std::shared_ptr<Getter>, std::shared_ptr<Getter>, std::shared_ptr<Getter>);
		~Rotate();

		void operator()();
		int getOutput();
		std::shared_ptr<Symbol> getVector();

		static glm::vec3 rotate(glm::vec3, glm::vec3, glm::vec3);
};

#endif