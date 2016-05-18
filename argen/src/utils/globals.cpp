#include "globals.h"

#include <string>

namespace Config {
	bool DEBUG = false;
	bool VERBOSE = false;
	bool EXPORT = false;
	int ITERATIONS = 0;
	int SEED = 0;
}

namespace Generator {
	std::shared_ptr<SymbolCollection> sourceSymbolString = nullptr;
	std::shared_ptr<SymbolCollection> outSymbolString = nullptr;
}

namespace MeshGenerator {
	std::shared_ptr<Mesh> currentMesh = nullptr;
}

namespace Global {
	std::string path;
}
