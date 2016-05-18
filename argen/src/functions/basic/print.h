#ifndef PRINT_H
#define PRINT_H

#include <memory>

#include "functionPrototype.h"

class Print : public Function {
	private:
		std::shared_ptr<Function> _function;
	public:
		Print(std::shared_ptr<Function>);

		void operator()();
		int getOutput();
};

#endif