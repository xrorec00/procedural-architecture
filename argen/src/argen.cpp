#include "argen.h"
#include <stdlib.h>

Argen::Argen(std::string fileName) {
	size_t pathPosition = fileName.find_last_of("/\\");
  	std::string path = fileName.substr(0, pathPosition);

  	if (path!="") {
  		path+="/";
  	}

  	Global::path = path;

	std::ifstream inputFile(fileName);
	std::vector<std::string> filesToHandle;

	if (inputFile) {
		std::string line;

		while (getline(inputFile, line)) {
		    if (line.substr(0,9) == "#include ") {
		    	filesToHandle.push_back(line.substr(9));
		    }
		    else {
		    	std::stringstream stream;
				stream<<"Invalid line \""<<line<<"\" in file \""<<fileName<<"\".";
				Log::error(stream.str());
		    }
		}
	}
	else {
		std::stringstream stream;
		stream<<"File "<<fileName<<" does not exist";
		Log::error(stream.str());
	}

	for (std::string fileTest : filesToHandle) {
		std::ifstream inputTest(path+fileTest);

		if (!inputTest) {
			std::stringstream stream;
			stream<<"File "<<fileTest<<" does not exist";
			Log::error(stream.str());
		}
	}

	_collection = std::make_shared<Collection>();

	yyscan_t scanner;
	yylex_init(&scanner);

	for (std::string fileTest : filesToHandle) {
		std::string fullFilePath = path + fileTest;

		std::cout<<"Processing file: "<<fileTest<<std::endl;

		std::ifstream inputTest(fullFilePath);

		FILE* filePointer = fopen(fullFilePath.c_str(), "rb");

		yyset_in(filePointer, scanner);
		yyparse(scanner,_collection);

		fclose(filePointer);
	}

	yylex_destroy(scanner);

	if (Config::VERBOSE) {
		if (Config::DEBUG) {
			std::cout<<"DEBUG: true"<<std::endl;
		}
		else {
			std::cout<<"DEBUG: false"<<std::endl;
		}

		if (Config::VERBOSE) {
			std::cout<<"VERBOSE: true"<<std::endl;
		}
		else {
			std::cout<<"VERBOSE: false"<<std::endl;
		}

		if (Config::EXPORT) {
			std::cout<<"EXPORT: true"<<std::endl;
		}
		else {
			std::cout<<"EXPORT: false"<<std::endl;
		}

		std::cout<<"ITERATIONS: "<<Config::ITERATIONS<<std::endl;
		std::cout<<"SEED: "<<Config::SEED<<std::endl;
	}

	// Apply seed value
	srand(Config::SEED);

	std::shared_ptr<SymbolCollection> output = std::make_shared<SymbolCollection>();
	_collection->rulesCollection->applyRules(output,Config::ITERATIONS);

	// Config::VERBOSE=false;
	// output->printInfo();
	// Config::VERBOSE=true;

	if (Config::VERBOSE) {
		std::cout<<"\033[1;36mGenerating meshes started\033[0m"<<std::endl;
	}

	for (int i = 0; i < output->getSize(); i++) {
		bool symbolProcessed = false;
		std::shared_ptr<Symbol> symbolToProcess = output->getSymbol(0,i);
		std::string symbolType = symbolToProcess->getVarSymType();

		for (auto &interpretation : _collection->interpretationsCollection->getInterpretations()) {
			if (symbolType == interpretation->getSymbolType()) {
				// MeshGenerator::currentMesh = new Mesh(symbolToProcess->getVec3ByName("_position"), symbolToProcess->getVec3ByName("_rotation"));
				MeshGenerator::currentMesh =
					std::make_shared<Mesh>(symbolToProcess->getVec3ByName("_position"), symbolToProcess->getVec3ByName("_rotation"));
				MeshGenerator::currentMesh->setName(symbolToProcess->getVarSymName());

				if (Config::VERBOSE) {
					std::cout<<"Processing symbol: "<<symbolType<<std::endl;
				}

				interpretation->addSourceSymbol(symbolToProcess);
				(*interpretation)();

				symbolProcessed = true;

				_collection->meshCollection->addMesh(MeshGenerator::currentMesh);
				MeshGenerator::currentMesh = nullptr;

				break;
			}
		}
		if (!symbolProcessed) {
			if (Config::VERBOSE) {
				std::cout<<"Processing symbol: "<<symbolType<<" failed (no interpretation found)"<<std::endl;
			}
		}
	}
	
	if (Config::VERBOSE) {
		std::cout<<"\033[1;36mGenerating meshes finished\033[0m"<<std::endl;
	}

	if (Config::EXPORT) {
		_collection->meshCollection->exportModel();
	}
}

Argen::~Argen() {
	Generator::sourceSymbolString.reset();
	Generator::sourceSymbolString = nullptr;

	Generator::outSymbolString.reset();
	Generator::outSymbolString = nullptr;

	MeshGenerator::currentMesh.reset();
	MeshGenerator::currentMesh = nullptr;


	_collection.reset();
	_collection = nullptr;
}

std::shared_ptr<MeshCollection> Argen::getMeshCollection() {
	return _collection->meshCollection;
}