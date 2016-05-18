#ifndef FOR_H
#define FOR_H

#include <memory>

#include "functionPrototype.h"
#include "../block.h"

class For : public Function {
	private:
		std::shared_ptr<Function> _preEvaluation;
		std::shared_ptr<Function> _condition;
		std::shared_ptr<Function> _postEvaluation;
		std::shared_ptr<Block> _body;

	public:
		For(std::shared_ptr<Function>, std::shared_ptr<Function>, std::shared_ptr<Function>, std::shared_ptr<Block>);
		~For();
		
		void operator()();
		int getOutput();

		void clean();
};

#endif