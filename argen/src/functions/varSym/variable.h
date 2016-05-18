#ifndef VARIABLE_H
#define VARIABLE_H

#include <memory>

#include "../basic/functionPrototype.h"
#include "varSymPrototype.h"
#include "nullary.h"
#include "../block.h"

class Nullary;
class Block;
class Variable:public Function, public VarSymPrototype, public std::enable_shared_from_this<Variable> {
	private:
		std::shared_ptr<Function> _value;
		std::shared_ptr<Block> _parent;
	public:
		Variable(std::shared_ptr<Block>, std::string, std::string name, std::shared_ptr<Function>);
		Variable(std::shared_ptr<Block>, std::string, std::string name);
		Variable();
		~Variable();
		
		int getOutput();
		std::shared_ptr<Function> getValue() const;
		void setValue(std::shared_ptr<Function>);
		void setVarSymType(std::string);
		std::string getVarSymType() const;

		std::shared_ptr<Variable> makeCopy();

		void operator()();

		void printInfo();

		std::shared_ptr<Variable> mathOperatorNullary(const Variable, Nullary, OperatorType) const;
		std::shared_ptr<Variable> mathOperator(const Variable,const Variable, OperatorType) const;
		std::shared_ptr<Variable> operator +(Variable);
		std::shared_ptr<Variable> operator -(Variable);
		std::shared_ptr<Variable> operator *(Variable);
		std::shared_ptr<Variable> operator /(Variable);

		std::shared_ptr<Variable> operator +(Nullary);
		std::shared_ptr<Variable> operator -(Nullary);
		std::shared_ptr<Variable> operator *(Nullary);
		std::shared_ptr<Variable> operator /(Nullary);
};

#endif