#include "compare.h"

#include "../varSym/nullary.h"
#include "../manipulation/getter.h"

#include <utils/log.h>

Compare::Compare(std::shared_ptr<Function> function1, std::shared_ptr<Function> function2, CompareType type):Function(COMPARISON) {
	_function1 = function1;
	_function2 = function2;
	_type = type;
}	

Compare::~Compare() {
	_function1.reset();
	_function1 = nullptr;

	_function2.reset();
	_function2 = nullptr;
}

int Compare::getOutput() {
	return output;
}

void Compare::operator()() {
	(*_function1)();
	(*_function2)();

	if (_function1->getName() == Function::COMPARISON && _function2->getName() == Function::COMPARISON) {
		if (_type == AND) {
			output = std::static_pointer_cast<Compare>(_function1)->getOutput() && std::static_pointer_cast<Compare>(_function2)->getOutput();
		}
		else if (_type == OR) {
			output = std::static_pointer_cast<Compare>(_function1)->getOutput() || std::static_pointer_cast<Compare>(_function2)->getOutput();
		}
		else {
			Log::error("Invalid operands in \"&&\" or \"||\" operation.");
		}

		return;
	}

	std::shared_ptr<Nullary> val1;
	if (_function1->getName() == Function::GETTER) {
		val1 = std::static_pointer_cast<Nullary>(std::static_pointer_cast<Variable>(std::static_pointer_cast<Getter>(_function1)->getProperty())->getValue());
	}
	else if (_function1->getName() == Function::NULLARY) {
		val1 = std::static_pointer_cast<Nullary>(_function1);
	}

	std::shared_ptr<Nullary> val2;
	if (_function2->getName() == Function::GETTER) {
		val2 = std::static_pointer_cast<Nullary>(std::static_pointer_cast<Variable>(std::static_pointer_cast<Getter>(_function2)->getProperty())->getValue());
	}
	else if (_function2->getName() == Function::NULLARY) {
		val2 = std::static_pointer_cast<Nullary>(_function2);
	}

	switch (_type) {
		case EQUALITY:
			output = (*val1) == (*val2);
			break;
		case NEQUALITY:
			output = (*val1) != (*val2);
			break;
		case LESS:
			output = (*val1) < (*val2);
			break;
		case LESSEQUAL:
			output = (*val1) <= (*val2);
			break;
		case GREATER:
			output = (*val1) > (*val2);
			break;
		case GREATEREQUAL:
			output = (*val1) >= (*val2);
			break;
	}
}