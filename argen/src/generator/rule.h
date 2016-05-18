#ifndef RULE_H
#define RULE_H

#include <memory>
#include <string>

#include "../functions/block.h"

class Block;
class Rule {
	private:
		std::string _name;
		int _length;
		std::shared_ptr<Block> _bodyCondition;
		std::shared_ptr<Block> _bodyAction;
		int _output;
	public:
		Rule(std::string, int, std::shared_ptr<Block>, std::shared_ptr<Block>);
		~Rule();

		std::shared_ptr<Block> getConditionBlock();
		std::shared_ptr<Block> getActionBlock();
		void setOutput(int);
		int getOutput();
		std::string getName();
		int getLength();

		void clean();
};

#endif