#ifndef INTERPRETATIONCOLLECTION_H
#define INTERPRETATIONCOLLECTION_H

#include <memory>
#include <vector>

#include "../../functions/interpretation.h"

class InterpretationsCollection {
	private:
		std::vector<std::shared_ptr<Interpretation>> _interpretations;
	public:
		void addInterpretation(std::shared_ptr<Interpretation>);
		std::vector<std::shared_ptr<Interpretation>> getInterpretations();
};

#endif