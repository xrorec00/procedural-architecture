#ifndef WHILE_H
#define WHILE_H

#include <memory>

#include "functionPrototype.h"
#include "../block.h"

class While: public Function {
	private:
		std::shared_ptr<Function> _condition;
		std::shared_ptr<Block> _body;

	public:
		While(std::shared_ptr<Function>, std::shared_ptr<Block>);
		~While();

		void operator()();
		int getOutput();

		void clean();
};

#endif