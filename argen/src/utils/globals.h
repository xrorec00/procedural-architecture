#ifndef GLOBALS_H
#define GLOBALS_H

#include <memory>

#include "../generator/collection/collection.h"
#include "../generator/model/mesh.h"

namespace Config {
	extern bool DEBUG;
	extern bool VERBOSE;
	extern bool EXPORT;
	extern int ITERATIONS;
	extern int SEED;
}

namespace Generator {
	extern std::shared_ptr<SymbolCollection> sourceSymbolString;
	extern std::shared_ptr<SymbolCollection> outSymbolString;
}

namespace MeshGenerator {
	extern std::shared_ptr<Mesh> currentMesh;
}

namespace Global {
	extern std::string path;
}

#endif