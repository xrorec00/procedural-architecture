#ifndef IF_H
#define IF_H

#include <memory>

#include "functionPrototype.h"
#include "../block.h"

class If: public Function {
	private:
		std::shared_ptr<Function> _condition;
		std::shared_ptr<Block> _bodyTrue;
		std::shared_ptr<Block> _bodyFalse;

	public:
		If(std::shared_ptr<Function>, std::shared_ptr<Block>, std::shared_ptr<Block>);
		~If();
		
		void setCondition(std::shared_ptr<Function> expr);
		void operator()();
		int getOutput();

		void clean();
};

#endif