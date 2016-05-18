#include "block.h"
#include <utils/log.h>

#include "basic/if.h"
#include "basic/for.h"

#include <sstream>

Block::Block(std::shared_ptr<Block> parent) {
	_parent = parent;
	_output = 0;
	_shouldReturn = false;

	_property = std::make_shared<Symbol>(nullptr);
}

Block::Block() {

}

Block::~Block() {
	if (_property != nullptr) {
		_property.reset();
		_property = nullptr;
	}
}

void Block::addFunction(std::shared_ptr<Function> function) {
	_functions.insert(_functions.begin(),function);
}

std::shared_ptr<Symbol> Block::getSymbol() {
	return _property;
}

int Block::operator()() {
	Log::debug("Block::operator()");

	for (auto &func : _functions) {
		(*func)();
		if (func->getShouldReturn()) {
			_output = func->getOutput();
			Log::debug("Block::operator()\tshouldReturn: " + _output);
			_shouldReturn = true;

			return 0;
		}
	}

	_output = 0;
	return 0;
}

void Block::setRule(std::shared_ptr<Rule> rule) {
	_rule = rule;
}

std::shared_ptr<Rule> Block::getRule() {
	return _rule;
}

int Block::getOutput() {
	Log::debug("Block::getOutput: " + _output);
	return _output;
}

void Block::addVariable(std::shared_ptr<Variable> variable) {
	_property->addVariable(variable);
}

void Block::addSymbol(std::shared_ptr<Symbol> symbol) {
	_property->addSymbol(symbol);
}

std::shared_ptr<VarSymPrototype> Block::getPrototype(std::stack<std::string> nameStack) {
	std::shared_ptr<VarSymPrototype> prototype = nullptr;

	std::string topTest = nameStack.top();

	prototype = _property->getPrototype(nameStack);
	
	if (prototype == nullptr) {
		if (_parent != nullptr) {
			return _parent->getPrototype(nameStack);
		}
	}
	else {
		return prototype;
	}

	Log::error("Variable or symbol " + topTest + " was not found.");
}

std::shared_ptr<Block> Block::getParent() {
	return _parent;
}

void Block::setParent(std::shared_ptr<Block> parent) {
	_parent = parent;
}

bool Block::getShouldReturn() {
	return _shouldReturn;
}

void Block::clean() {
	Log::debug("Block::clean()");

	try {
		_property = std::make_shared<Symbol>(shared_from_this());
    } catch(const std::bad_weak_ptr& e) {
    	std::stringstream stream;
		stream<<e.what()<<" in Block::clean\n";
		Log::error(stream.str());
    }
}