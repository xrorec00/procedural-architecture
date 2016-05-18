#ifndef NULLARY_H
#define NULLARY_H

#include <memory>

#include "variable.h"

class Variable;
class Nullary:public Function {
	public:
		enum Type{
			INT,
			FLOAT,
			STRING,
			BOOL
		};
	private:
		std::shared_ptr<void> _value;
		Type _type;

		bool _isRandom;
		bool _randomValueSet;
		float _randomMaxValue;

		enum OperatorType{
			PLUS,
			MINUS,
			MULTIPLY,
			DIVIDE,

			EQUAL,
			NOTEQUAL,
			LESS,
			LESSEQUAL,
			GREATER,
			GREATEREQUAL
		};
	public: 
		std::shared_ptr<Nullary> mathOperatorNullary(const Nullary, OperatorType);
		std::shared_ptr<Nullary> mathOperator(const Variable, OperatorType);
		int compareOperator(const Nullary, OperatorType);
	public:
		Nullary(Type,std::shared_ptr<void>);
		~Nullary();

		Type getType() const;
		int getOutput();
		std::shared_ptr<void> getValue() const;
		void operator()();
		void printInfo();

		void setRandom(bool, float);

		std::shared_ptr<Nullary> operator +(const Nullary);
		std::shared_ptr<Nullary> operator -(const Nullary);
		std::shared_ptr<Nullary> operator *(const Nullary);
		std::shared_ptr<Nullary> operator /(const Nullary);

		std::shared_ptr<Nullary> operator +(const Variable);
		std::shared_ptr<Nullary> operator -(const Variable);
		std::shared_ptr<Nullary> operator *(const Variable);
		std::shared_ptr<Nullary> operator /(const Variable);

		bool operator ==(const Nullary);
		bool operator !=(const Nullary);
		bool operator <(const Nullary);
		bool operator <=(const Nullary);
		bool operator >(const Nullary);
		bool operator >=(const Nullary);
};

#endif