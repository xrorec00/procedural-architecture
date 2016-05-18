#include "collection.h"

Collection::Collection() {
	symCollection = std::make_shared<SymCollection>();
	rulesCollection = std::make_shared<RulesCollection>();
	interpretationsCollection = std::make_shared<InterpretationsCollection>();
	meshCollection = std::make_shared<MeshCollection>();
}

Collection::~Collection() {
	symCollection.reset();
	symCollection = nullptr;

	rulesCollection.reset();
	rulesCollection = nullptr;

	interpretationsCollection.reset();
	interpretationsCollection = nullptr;

	meshCollection.reset();
	meshCollection = nullptr;	
}