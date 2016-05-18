#include "nullary.h"

#include <iostream>
#include <stdlib.h>
#include <sstream>

#include <utils/log.h>

Nullary::Nullary(Type type, std::shared_ptr<void> value):Function(NULLARY) {
	_isRandom = false;

	_type = type;
	_value = value;
}

Nullary::~Nullary() {
	_isRandom = false;

	if (_type == Nullary::Type::INT) {
		std::shared_ptr<int> value = std::static_pointer_cast<int>(_value);
		value.reset();
		_value = nullptr;
	}
	else if (_type == Nullary::Type::FLOAT) {
		std::shared_ptr<float> value = std::static_pointer_cast<float>(_value);
		value.reset();
		_value = nullptr;
	}
	else if (_type == Nullary::Type::STRING) {
		std::shared_ptr<std::string> value = std::static_pointer_cast<std::string>(_value);
		value.reset();
		_value = nullptr;
	}
	else {
		std::stringstream stream;
		stream<<"Invalid nullary type: "<<_type<<" in destructor.";
		Log::error(stream.str());
	}
}

void Nullary::setRandom(bool b, float maxValue) {
	_isRandom = b;
	_randomMaxValue = maxValue;
}

void Nullary::operator()() {
	if (!_randomValueSet && _isRandom) {
		float value = ((float)rand() / RAND_MAX) * _randomMaxValue;

		if (_type == Nullary::Type::INT) {
			_value = std::make_shared<int>((int)value);
		}
		else if (_type == Nullary::Type::FLOAT) {
			_value = std::make_shared<float>(value);
		}
		else {
			std::stringstream stream;
			stream<<"Invalid nullary type: "<<_type<<" , cannot be randomized.";
			Log::error(stream.str());
		}

		_randomValueSet = true;
	}
}

int Nullary::getOutput() {
	if (_type == Nullary::Type::BOOL) {
		if (*(std::static_pointer_cast<bool>(_value))) {
			return 1;
		}
		else {
			return 0;
		}
	} else {
		Log::error("Error getting value from nullary.");
	}
}

std::shared_ptr<void> Nullary::getValue() const{
	return _value;
}

Nullary::Type Nullary::getType() const {
	return _type;
}

void Nullary::printInfo() {
	if (_value == nullptr) {
		Log::error("Nullary: print failed. Value not assigned.");
	}

	if (_type == Nullary::Type::INT) {
		std::cout<<*(std::static_pointer_cast<int>(_value))<<std::endl<<std::flush;
	}
	else if (_type == Nullary::Type::FLOAT) {
		std::cout<<*(std::static_pointer_cast<float>(_value))<<std::endl<<std::flush;
	}
	else if (_type == Nullary::Type::STRING) {
		std::cout<<*(std::static_pointer_cast<std::string>(_value))<<std::endl<<std::flush;
	}
	else {
		Log::error("Invalid nullary type: ");
	}
}

std::shared_ptr<Nullary> Nullary::mathOperatorNullary(const Nullary nullary, OperatorType type) {
	if (_type == Nullary::INT && nullary.getType() == Nullary::INT) {
		int val1 = *std::static_pointer_cast<int>(_value);
		int val2 = *std::static_pointer_cast<int>(nullary.getValue());

		switch (type) {
			case PLUS:
				return std::make_shared<Nullary>(Nullary::Type::INT, std::make_shared<int>(val1 + val2));
			case MINUS:
				return std::make_shared<Nullary>(Nullary::Type::INT, std::make_shared<int>(val1 - val2));
			case MULTIPLY:
				return std::make_shared<Nullary>(Nullary::Type::INT, std::make_shared<int>(val1 * val2));
			case DIVIDE:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 / val2));
		}
	}
	else if (_type == Nullary::FLOAT && nullary.getType() == Nullary::INT) {
		float val1 = *std::static_pointer_cast<float>(_value);
		int val2 = *std::static_pointer_cast<int>(nullary.getValue());

		switch (type) {
			case PLUS:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 + val2));
			case MINUS:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 - val2));
			case MULTIPLY:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 * val2));
			case DIVIDE:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 / val2));
		}
	}
	else if (_type == Nullary::INT && nullary.getType() == Nullary::FLOAT) {
		int val1 = *std::static_pointer_cast<int>(_value);
		float val2 = *std::static_pointer_cast<float>(nullary.getValue());

		switch (type) {
			case PLUS:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 + val2));
			case MINUS:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 - val2));
			case MULTIPLY:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 * val2));
			case DIVIDE:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 / val2));
		}
	}
	else if (_type == Nullary::FLOAT && nullary.getType() == Nullary::FLOAT) {
		float val1 = *std::static_pointer_cast<float>(_value);
		float val2 = *std::static_pointer_cast<float>(nullary.getValue());

		switch (type) {
			case PLUS:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 + val2));
			case MINUS:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 - val2));
			case MULTIPLY:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 * val2));
			case DIVIDE:
				return std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(val1 / val2));
		}
	}
	else if (_type == Nullary::STRING && nullary.getType() == Nullary::STRING) {
		std::string val1 = *std::static_pointer_cast<std::string>(_value);
		std::string val2 = *std::static_pointer_cast<std::string>(nullary.getValue());

		switch (type) {
			case PLUS:
				return std::make_shared<Nullary>(Nullary::Type::STRING, std::make_shared<std::string>(val1 + val2));
			default:
				Log::error("Invalid string operation");
		}
	}
	else {
		Log::error("Invalid operands");
	}
}

std::shared_ptr<Nullary> Nullary::mathOperator(const Variable variable, OperatorType type) {
	Nullary nullary = *std::static_pointer_cast<Nullary>(variable.getValue());
	return mathOperatorNullary(nullary, type);
}

int Nullary::compareOperator(const Nullary nullary, OperatorType type) {
	int val1, val2;

	if (_type == Nullary::INT && nullary.getType() == Nullary::INT) {
		int val1 = *std::static_pointer_cast<int>(_value);
		int val2 = *std::static_pointer_cast<int>(nullary.getValue());

		switch (type) {
			case EQUAL:
				return val1 == val2;
			case NOTEQUAL:
				return val1 != val2;
			case LESS:
				return val1 < val2;
			case LESSEQUAL:
				return val1 <= val2;
			case GREATER:
				return val1 > val2;
			case GREATEREQUAL:
				return val1 >= val2;
		}
	}
	else if (_type == Nullary::FLOAT && nullary.getType() == Nullary::INT) {
		float val1 = *std::static_pointer_cast<float>(_value);
		int val2 = *std::static_pointer_cast<int>(nullary.getValue());

		switch (type) {
			case EQUAL:
				return val1 == val2;
			case NOTEQUAL:
				return val1 != val2;
			case LESS:
				return val1 < val2;
			case LESSEQUAL:
				return val1 <= val2;
			case GREATER:
				return val1 > val2;
			case GREATEREQUAL:
				return val1 >= val2;
		}
	}
	else if (_type == Nullary::INT && nullary.getType() == Nullary::FLOAT) {
		int val1 = *std::static_pointer_cast<int>(_value);
		float val2 = *std::static_pointer_cast<float>(nullary.getValue());

		switch (type) {
			case EQUAL:
				return val1 == val2;
			case NOTEQUAL:
				return val1 != val2;
			case LESS:
				return val1 < val2;
			case LESSEQUAL:
				return val1 <= val2;
			case GREATER:
				return val1 > val2;
			case GREATEREQUAL:
				return val1 >= val2;
		}
	}
	else if (_type == Nullary::FLOAT && nullary.getType() == Nullary::FLOAT) {
		float val1 = *std::static_pointer_cast<float>(_value);
		float val2 = *std::static_pointer_cast<float>(nullary.getValue());

		switch (type) {
			case EQUAL:
				return val1 == val2;
			case NOTEQUAL:
				return val1 != val2;
			case LESS:
				return val1 < val2;
			case LESSEQUAL:
				return val1 <= val2;
			case GREATER:
				return val1 > val2;
			case GREATEREQUAL:
				return val1 >= val2;
		}
	}
	else if (_type == Nullary::STRING && nullary.getType() == Nullary::STRING) {
		std::string val1 = *std::static_pointer_cast<std::string>(_value);
		std::string val2 = *std::static_pointer_cast<std::string>(nullary.getValue());

		switch (type) {
			case EQUAL:
				return val1 == val2;
			case NOTEQUAL:
				return val1 != val2;
			default:
			Log::error("nullary: invalid string comparison");
		}
	}

	return 0;
}

std::shared_ptr<Nullary> Nullary::operator +(const Nullary nullary) {
	return mathOperatorNullary(nullary, OperatorType::PLUS);
}

std::shared_ptr<Nullary> Nullary::operator -(const Nullary nullary) {
	return mathOperatorNullary(nullary, OperatorType::MINUS);
}

std::shared_ptr<Nullary> Nullary::operator *(const Nullary nullary) {
	return mathOperatorNullary(nullary, OperatorType::MULTIPLY);
}

std::shared_ptr<Nullary> Nullary::operator /(const Nullary nullary) {
	return mathOperatorNullary(nullary, OperatorType::DIVIDE);
}

std::shared_ptr<Nullary> Nullary::operator +(const Variable variable) {
	return mathOperator(variable, OperatorType::PLUS);
}

std::shared_ptr<Nullary> Nullary::operator -(const Variable variable) {
	return mathOperator(variable, OperatorType::MINUS);
}

std::shared_ptr<Nullary> Nullary::operator *(const Variable variable) {
	return mathOperator(variable, OperatorType::MULTIPLY);
}

std::shared_ptr<Nullary> Nullary::operator /(const Variable variable) {
	return mathOperator(variable, OperatorType::DIVIDE);
}

bool Nullary::operator ==(const Nullary nullary) {
	return compareOperator(nullary, OperatorType::EQUAL);
}

bool Nullary::operator !=(const Nullary nullary) {
	return compareOperator(nullary, OperatorType::NOTEQUAL);
}

bool Nullary::operator <(const Nullary nullary) {
	return compareOperator(nullary, OperatorType::LESS);
}

bool Nullary::operator <=(const Nullary nullary) {
	return compareOperator(nullary, OperatorType::LESSEQUAL);
}

bool Nullary::operator >(const Nullary nullary) {
	return compareOperator(nullary, OperatorType::GREATER);
}

bool Nullary::operator >=(const Nullary nullary) {
	return compareOperator(nullary, OperatorType::GREATEREQUAL);
}
