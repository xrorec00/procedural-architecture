#ifndef RETURN_H
#define RETURN_H

#include <memory>

#include "functionPrototype.h"
#include "../../generator/rule.h"

class Return: public Function {
	private:
		std::shared_ptr<Function> _body;
		std::shared_ptr<Rule> rule;

	public:
		Return(std::shared_ptr<Function>);
		~Return();
		
		void operator()();
		int getOutput();
};

#endif