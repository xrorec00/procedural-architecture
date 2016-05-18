#ifndef GETTER_H
#define GETTER_H

#include <memory>

#include <string>
#include "../block.h"
#include "../varSym/varSymPrototype.h"

class Block;
class Getter: public Function {
	private:
		std::stack<std::string> nameStack;
		std::shared_ptr<Block> parent;
		std::shared_ptr<VarSymPrototype> _propertyToReturn;
	public:
		Getter(std::shared_ptr<Block>,std::stack<std::string>);
		~Getter();

		int getOutput();
		void operator()();

		std::shared_ptr<VarSymPrototype> getProperty();
};

#endif