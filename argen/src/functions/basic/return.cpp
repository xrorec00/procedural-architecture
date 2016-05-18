#include "return.h"

Return::Return(std::shared_ptr<Function> expr):Function(RETURN) {
	_body = expr;
}

Return::~Return() {
	_body.reset();
	_body = nullptr;
}

void Return::operator()() {
	(*_body)();
	output = _body->getOutput();
	shouldReturn = true;
}

int Return::getOutput() {
	return output;
}