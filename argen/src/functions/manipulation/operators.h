#ifndef OPERATORS_H
#define OPERATORS_H

#include <memory>

#include "../varSym/variable.h"
#include "../varSym/nullary.h"
#include "getter.h"

class Nullary;
class Variable;
class Symbol;
class Operator:public Function {
	public:
		enum OperatorType {
			PLUS,
			MINUS,
			MULTIPLY,
			DIVIDE
		};
	private:
		std::shared_ptr<Function> _function1;
		std::shared_ptr<Function> _function2;
		std::shared_ptr<VarSymPrototype> _value;
		OperatorType _type;

		std::shared_ptr<VarSymPrototype> handleOperation(std::shared_ptr<Variable>, std::shared_ptr<Variable>);
		std::shared_ptr<VarSymPrototype> handleOperation(std::shared_ptr<Nullary>, std::shared_ptr<Variable>);
		std::shared_ptr<VarSymPrototype> handleOperation(std::shared_ptr<Variable>, std::shared_ptr<Nullary>);
		std::shared_ptr<VarSymPrototype> handleOperation(std::shared_ptr<Nullary>, std::shared_ptr<Nullary>);
		std::shared_ptr<VarSymPrototype> handleOperation(std::shared_ptr<Symbol>, std::shared_ptr<Symbol>);
	public:
		Operator(std::shared_ptr<Function>, std::shared_ptr<Function>, OperatorType);
		~Operator();

		std::shared_ptr<VarSymPrototype> getValue();
		int getOutput();
		void operator()();
};

#endif