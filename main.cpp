#include <memory>
#include <string>

#include "display/display.h"

#include "argen/src/argen.h"

#include <iostream>

int main(int argc, char* argv[]) {
	std::string fileName = "";

	for (int i = 1; i < argc; i++) {
		if (fileName == "") {
			fileName = std::string(argv[i]);
		}
		else {
			std::cerr<<"File is already set"<<std::endl;
			exit(1);
		}
	}

	if (fileName == "") {
		std::cerr<<"File name is missing"<<std::endl;
		exit(1);
	}

	Argen argen(fileName);
	std::shared_ptr<MeshCollection> meshCollection = argen.getMeshCollection();

	std::cout<<"\033[1;36mCreating visualizer\033[0m"<<std::endl;

	meshCollection->calculateVerticesAndNormals();

	Display display(
		meshCollection->getVerticesNum(),
		meshCollection->getVertices(),
		meshCollection->getCenter(),
		meshCollection->getNormals()
	);

	return 0;
}