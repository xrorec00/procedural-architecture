#ifndef BLOCK_H
#define BLOCK_H

#include <memory>

#include <string>
#include <stack>

#include "basic/functionPrototype.h"
#include "varSym/variable.h"
#include "varSym/symbol.h"

class Rule;
class Symbol;
class Variable;
class Block : public std::enable_shared_from_this<Block>{
	protected:
		std::vector<std::shared_ptr<Function>> _functions;

		std::shared_ptr<Symbol> _property;

		std::shared_ptr<Block> _parent;
		std::shared_ptr<Rule> _rule;
		int _output;

		bool _shouldReturn;
	public:
		Block(std::shared_ptr<Block>);
		Block();
		~Block();

		void addFunction(std::shared_ptr<Function>);
		void addVariable(std::shared_ptr<Variable>);
		void addSymbol(std::shared_ptr<Symbol>);
		std::shared_ptr<Variable> getVariable(std::string);
		std::shared_ptr<Symbol> getSymbol(std::string);
		std::shared_ptr<VarSymPrototype> getPrototype(std::stack<std::string>);

		std::shared_ptr<Block> getParent();
		int getOutput();
		void setParent(std::shared_ptr<Block> parent);
		std::shared_ptr<Symbol> getSymbol();
		int operator()();
		void setRule(std::shared_ptr<Rule>);
		std::shared_ptr<Rule> getRule();

		bool getShouldReturn();

		void clean();
};	

#endif