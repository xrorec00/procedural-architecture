#include "while.h"

#include <utils/log.h>

While::While(std::shared_ptr<Function> condition, std::shared_ptr<Block> body):Function(WHILE) {
	_condition = condition;
	_body = body;
}

While::~While() {
	_condition.reset();
	_condition = nullptr;
	
	_body.reset();
	_body = nullptr;
}

void While::operator()() {
	Log::debug("While::operator()");

	while (true) {
		(*_condition)();

		if (_condition->getOutput()) {
			(*_body)();

			if (_body->getShouldReturn()) {
				output=_body->getOutput();
				shouldReturn=true;
				break;
			}
		}
		else{
			break;
		}
	}
}

int While::getOutput() {
	return output;
}

void While::clean() {
	Log::debug("While::clean");

	_body->clean();
	output=0;
	shouldReturn=false;
}