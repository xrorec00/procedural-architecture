#include "for.h"

#include <utils/log.h>

For::For(std::shared_ptr<Function> preEvaluation, std::shared_ptr<Function> condition, std::shared_ptr<Function> postEvaluation, std::shared_ptr<Block> body):Function(FOR) {
	_preEvaluation = preEvaluation;
	_condition = condition;
	_postEvaluation = postEvaluation;
	_body = body;
}

For::~For() {
	_preEvaluation.reset();
	_preEvaluation = nullptr;
	
	_condition.reset();
	_condition = nullptr;
	
	_postEvaluation.reset();
	_postEvaluation = nullptr;
	
	_body.reset();
	_body = nullptr;
	
}

void For::operator()() {
	Log::debug("For::operator()");

	(*_preEvaluation)();

	while (true) {
		(*_condition)();

		if (_condition->getOutput()) {
			(*_body)();

			if (_body->getShouldReturn()) {
				output=_body->getOutput();
				shouldReturn=true;
				break;
			}
			else {
				(*_postEvaluation)();
			}
		}
		else{
			break;
		}
	}
}

int For::getOutput() {
	return output;
}

void For::clean() {
	Log::debug("For::clean");

	_body->clean();
	output=0;
	shouldReturn=false;
}