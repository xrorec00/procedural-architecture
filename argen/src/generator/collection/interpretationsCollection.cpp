#include "interpretationsCollection.h"

void InterpretationsCollection::addInterpretation(std::shared_ptr<Interpretation> interpretation) {
	_interpretations.push_back(interpretation);
}

std::vector<std::shared_ptr<Interpretation>> InterpretationsCollection::getInterpretations() {
	return _interpretations;
}