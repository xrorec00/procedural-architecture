#ifndef SETTER_H
#define SETTER_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "../block.h"
#include "getter.h"

class Getter;
class Setter : public Function {
	private:
		std::string _varSymName;
		std::shared_ptr<Block> _parent;
		std::shared_ptr<Function> _targetValue;
		std::shared_ptr<Getter> _varSym1;
		std::shared_ptr<Function> _varSym2;	
	public:
		Setter(std::shared_ptr<Block>, std::shared_ptr<Getter>, std::shared_ptr<Function>);
		~Setter();

		int getOutput();

		void operator()();
};

#endif