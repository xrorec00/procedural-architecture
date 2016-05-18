#include "symCollection.h"
#include <utils/log.h>

void SymCollection::addTemplate(std::shared_ptr<Symbol> symbol) {
	Log::debug("SymCollection addTemplate: "+symbol->getVarSymName());

	std::shared_ptr<Symbol> _position = makeSymbol("vec3","_position",nullptr);
	symbol->addSymbol(_position);

	std::shared_ptr<Symbol> _rotation = makeSymbol("vec3","_rotation",nullptr);
	symbol->addSymbol(_rotation);

	std::shared_ptr<Variable> _type = std::make_shared<Variable>(nullptr, "string", "_type", std::make_shared<Nullary>(Nullary::Type::STRING,std::make_shared<std::string>(symbol->getVarSymType())));
	symbol->addVariable(_type);

	_symbolTemplates.push_back(symbol);
}

std::shared_ptr<Symbol> SymCollection::makeSymbol(std::string type, std::string name, std::shared_ptr<Block> parent) {
	Log::debug("SymCollection makeSymbol (type:" + type + ", name:" + name + ")");

	std::shared_ptr<Symbol> newSymbol = nullptr;

	if (type == "vec3") {
		newSymbol = std::make_shared<Symbol>(parent);
		newSymbol->setVarSymType("vec3");
		newSymbol->setVarSymName(name);

		std::shared_ptr<Variable> v1 = std::make_shared<Variable>(nullptr, "float", "x", std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(0.0f)));
		std::shared_ptr<Variable> v2 = std::make_shared<Variable>(nullptr, "float", "y", std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(0.0f)));
		std::shared_ptr<Variable> v3 = std::make_shared<Variable>(nullptr, "float", "z", std::make_shared<Nullary>(Nullary::Type::FLOAT, std::make_shared<float>(0.0f)));

		newSymbol->addVariable(v1);
		newSymbol->addVariable(v2);
		newSymbol->addVariable(v3);

		return newSymbol;
	}

	for (auto &symbolTemplate : _symbolTemplates) {
		if (symbolTemplate->getVarSymType() == type) {
			newSymbol = symbolTemplate->makeCopy(parent);
			newSymbol->setVarSymName(name);
			newSymbol->setVarSymType(type);

			
			std::shared_ptr<Variable> _name = std::make_shared<Variable>(nullptr, "string", "_name", std::make_shared<Nullary>(Nullary::Type::STRING, std::make_shared<std::string>(name)));
			newSymbol->addVariable(_name);

			return newSymbol;
		}
	}

	Log::error("Symbol " + type + " does not exist");
}