#ifndef OUTPUTEMPTY_H
#define OUTPUTEMPTY_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "../block.h"

class OutputEmpty:public Function {
	private:
		std::shared_ptr<Block> _parent;
	public:
		OutputEmpty(std::shared_ptr<Block>);
		~OutputEmpty();
		
		int getOutput();
		void operator()();
};

#endif