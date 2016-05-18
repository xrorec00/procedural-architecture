#include "rulesCollection.h"

#include <iostream>

#include <utils/globals.h>
#include <utils/log.h>

void RulesCollection::addRule(std::shared_ptr<Rule> rule) {
	_rules.push_back(rule);
}

void RulesCollection::applyRules(std::shared_ptr<SymbolCollection> output, int depth) {
	if (Config::VERBOSE) {
		std::cout<<"\033[1;32m-----Generating output sequence-----\033[0m"<<std::endl;
	}

	Generator::sourceSymbolString = output;

	for (int i = 0; i < depth || Config::ITERATIONS == 0; i++) {
		if (Config::VERBOSE) {
			std::cout<<"\033[1;36mIteration number: "<<i<<" started\033[0m\tSymbol count: "<<Generator::sourceSymbolString->getSize()<<std::endl;
		}
		
		Generator::outSymbolString = std::make_shared<SymbolCollection>();	

		bool ruleUsedInDepth = false;	

		for (int j = 0; j < Generator::sourceSymbolString->getSize() || Generator::outSymbolString->getSize() == 0;) {
			if (Config::VERBOSE) {
				std::cout<<"Cursor position: "<<j<<std::endl;
			}			

			std::vector<std::shared_ptr<Rule>> viableRules;
			Generator::sourceSymbolString->setCursorPosition(j);

			for (auto &rule : _rules) {
				if (j+rule->getLength()>Generator::sourceSymbolString->getSize()) {
					continue;
				}

				std::shared_ptr<Block> condition = rule->getConditionBlock();

				//add symbols to container !!!!!!!!!!!!!!!!!!!!!!!!!!!!! named $0, $1...
				for (int k = 0; k < rule->getLength(); k++) {
					std::shared_ptr<Symbol> s = std::static_pointer_cast<Symbol>(Generator::sourceSymbolString->getSymbol(j,k))->makeCopy(nullptr);
					s->setVarSymName("$" + std::to_string(k));
					condition->addSymbol(s);
				}

				(*condition)();

				int out = condition->getOutput();

				if (out == 1) {
					viableRules.push_back(rule);
				}
			}
			
			if (!viableRules.empty()) {
				ruleUsedInDepth = true;

				if (Config::VERBOSE) {
					for (auto &viableRule : viableRules) {
						std::cout<<"\033[1;33mRule: "<<viableRule->getName()<<" is viable\033[0m"<<std::endl<<std::flush;
					}
				}

				std::shared_ptr<Rule> ruleToBeApplied = viableRules.front();

				if (Config::VERBOSE) {
					std::cout<<std::endl<<"\033[1;33mApplying rule: "<<ruleToBeApplied->getName()<<"\033[0m"<<std::endl<<std::flush;
				}

				std::shared_ptr<Block> action = ruleToBeApplied->getActionBlock();

				for (int k = 0; k < ruleToBeApplied->getLength(); k++) {
					std::shared_ptr<Symbol> s = std::static_pointer_cast<Symbol>(Generator::sourceSymbolString->getSymbol(j,k))->makeCopy(nullptr);
					s->setVarSymName("$"+std::to_string(k));
					action->addSymbol(s);
				}

				(*action)();

				if (Generator::outSymbolString->getSize() == 0) {
					Log::error("Symbol string is empty after rules were used");
				}

				//clean
				ruleToBeApplied->clean();
				
				j += ruleToBeApplied->getLength();
			}
			else if (j < Generator::sourceSymbolString->getSize()) {
				Generator::outSymbolString->addSymbol(Generator::sourceSymbolString->getSymbol(j, 0));
				j++;
			}

			if (!ruleUsedInDepth) {
				if (j == Generator::outSymbolString->getSize()) {
					(*output) = (*Generator::sourceSymbolString);
					if (Config::VERBOSE) {
						std::cout<<"\033[1;36mIterations finished - no rule viable\033[0m"<<std::endl;
					}
					return;
				}
			}
		}

		Generator::sourceSymbolString = Generator::outSymbolString;
	}

	(*output) = (*Generator::outSymbolString);

	if (Config::VERBOSE) {
		std::cout<<"\033[1;36mIterations finished\033[0m"<<std::endl;
	}
}