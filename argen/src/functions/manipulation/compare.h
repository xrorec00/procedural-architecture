#ifndef COMPARE_H
#define COMPARE_H

#include <memory>

#include "../basic/functionPrototype.h"

class Compare:public Function {
	public:
		enum CompareType {
			EQUALITY,
			NEQUALITY,
			LESS,
			LESSEQUAL,
			GREATER,
			GREATEREQUAL,

			AND,
			OR
		};
	private:
		std::shared_ptr<Function> _function1;
		std::shared_ptr<Function> _function2;
		CompareType _type;
	public:
		Compare(std::shared_ptr<Function>, std::shared_ptr<Function>, CompareType);
		~Compare();

		int getOutput();
		void operator()();
};

#endif