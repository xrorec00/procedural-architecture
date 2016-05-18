#ifndef SYMBOL_H
#define SYMBOL_H

#include <memory>

#include <vector>
#include "../../../libs/glm/glm/vec3.hpp"

#include "varSymPrototype.h"
#include "../block.h"
#include "variable.h"


class Variable;
class Block;
class Symbol : public VarSymPrototype {
	private:
		std::shared_ptr<Block> _parent;
		std::vector<std::shared_ptr<VarSymPrototype>> _varSymPrototypes;
	public:
		Symbol(std::shared_ptr<Block>);
		~Symbol();

		std::shared_ptr<VarSymPrototype> getPrototype(std::stack<std::string>);
		std::shared_ptr<VarSymPrototype> getPrototypeByIndex(int) const;
		std::vector<std::shared_ptr<VarSymPrototype>> getVarSymPrototypes();
		int getVarSymPrototypesSize();

		std::shared_ptr<Symbol> makeCopy(std::shared_ptr<Block>);
		void addVariable(std::shared_ptr<Variable>);
		void addSymbol(std::shared_ptr<Symbol>);	

		void setValue(std::shared_ptr<Symbol>);

		void printInfo();

		glm::vec3 getVec3ByName(std::string);
		static glm::vec3 getVec3(std::shared_ptr<Symbol>, std::string);

		std::shared_ptr<Symbol> mathOperator(const Symbol, OperatorType) const;
		std::shared_ptr<Symbol> operator +(const Symbol);
		std::shared_ptr<Symbol> operator -(const Symbol);
		std::shared_ptr<Symbol> operator *(const Symbol);
		std::shared_ptr<Symbol> operator /(const Symbol);
};

#endif