#ifndef RULESCOLLECTION_H
#define RULESCOLLECTION_H

#include <memory>
#include <vector>

#include "../rule.h"
#include "symbolCollection.h"

class RulesCollection {
	private:
		std::vector<std::shared_ptr<Rule>> _rules;
		std::vector<std::shared_ptr<Rule>> _viableRules;
	public:
		void addRule(std::shared_ptr<Rule>);
		void applyRules(std::shared_ptr<SymbolCollection>,int);
};

#endif