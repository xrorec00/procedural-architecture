#ifndef OUTPUTADD_H
#define OUTPUTADD_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "../manipulation/getter.h"
#include "../block.h"

class Getter;
class OutputAdd:public Function {
	private:
		std::shared_ptr<Block> _parent;
		std::shared_ptr<Getter> _getter;
	public:
		OutputAdd(std::shared_ptr<Block>, std::shared_ptr<Getter>);
		~OutputAdd();
		
		int getOutput();
		void operator()();
};

#endif