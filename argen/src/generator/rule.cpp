#include <generator/rule.h>

Rule::Rule(std::string name, int length, std::shared_ptr<Block> condition, std::shared_ptr<Block> action) {
	_name = name;
	_length = length;
	_bodyCondition = condition;
	_bodyAction = action;
	_output = 0;
}

Rule::~Rule() {
	_bodyCondition.reset();
	_bodyCondition = nullptr;
	
	_bodyAction.reset();
	_bodyAction = nullptr;
}

std::shared_ptr<Block> Rule::getConditionBlock() {
	return _bodyCondition;
}

std::shared_ptr<Block> Rule::getActionBlock() {
	return _bodyAction;
}

void Rule::setOutput(int output) {
	_output = output;
}

int Rule::getOutput() {
	return _output;
}

std::string Rule::getName() {
	return _name;
}

int Rule::getLength() {
	return _length;
}

void Rule::clean() {
	_bodyCondition->clean();
	_bodyAction->clean();
}