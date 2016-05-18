#include "if.h"

If::If(std::shared_ptr<Function> cond, std::shared_ptr<Block> bodyTrue, std::shared_ptr<Block> bodyFalse):Function(IF) {
	_condition = cond;
	_bodyTrue = bodyTrue;
	_bodyFalse = bodyFalse;
}

If::~If() {
	_condition.reset();
	_condition = nullptr;

	_bodyTrue.reset();
	_bodyTrue = nullptr;

	if (_bodyFalse != nullptr) {
		_bodyFalse.reset();
		_bodyFalse = nullptr;
	}
}

void If::setCondition(std::shared_ptr<Function> expr){

}

void If::operator()() {
	(*_condition)();
	if (_condition->getOutput()) {
		(*_bodyTrue)();

		if (_bodyTrue->getShouldReturn()) {
			output = _bodyTrue->getOutput();
			shouldReturn = true;
		}
	}
	else{
		if (_bodyFalse != nullptr) {
			(*_bodyFalse)();

			if (_bodyFalse->getShouldReturn()) {
				output = _bodyFalse->getOutput();
				shouldReturn = true;
			}
		}
	}
}

int If::getOutput() {
	return output;
}

void If::clean() {
	_bodyTrue->clean();
	_bodyFalse->clean();
}