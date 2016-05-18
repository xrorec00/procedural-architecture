#ifndef COLLECTION_H
#define COLLECTION_H

#include <memory>
#include "../../functions/varSym/symCollection.h"
#include "rulesCollection.h"
#include "interpretationsCollection.h"
#include "meshCollection.h"

class Collection {
	public:
		std::shared_ptr<RulesCollection> rulesCollection;
		std::shared_ptr<SymCollection> symCollection;
		std::shared_ptr<InterpretationsCollection> interpretationsCollection;
		std::shared_ptr<MeshCollection> meshCollection;

		Collection();
		~Collection();
};

#endif