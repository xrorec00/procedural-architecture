#include "meshCollection.h"

#include <limits>
#include <glm/glm.hpp>

#include <utils/globals.h>
#include <utils/log.h>

#include <string>
#include <fstream>

MeshCollection::~MeshCollection() {
	
}

void MeshCollection::addMesh(std::shared_ptr<Mesh> mesh) {
	_meshes.push_back(mesh);
}

unsigned MeshCollection::getVerticesNum() {
	unsigned num = 0;

	for (auto &mesh : _meshes) {
		num += (mesh->getFacesSize()*3);
	}

	return num;
}

void MeshCollection::calculateVerticesAndNormals() {
	unsigned verticesNum = getVerticesNum();

	_vertices = new float[verticesNum*3];
	_normals = new float[verticesNum*3];
	unsigned i = 0;

	float maxX = std::numeric_limits<float>::min();
	float minX = std::numeric_limits<float>::max();

	float maxY = std::numeric_limits<float>::min();
	float minY = std::numeric_limits<float>::max();

	float maxZ = std::numeric_limits<float>::min();
	float minZ = std::numeric_limits<float>::max();

	for (auto &mesh : _meshes) {
		std::shared_ptr<std::vector<glm::vec3>> vertices = mesh->getVertices();

		for (auto &face : *(mesh->getFaces())) {
			std::shared_ptr<std::vector<int>> verticesIndexes = face.getVertices();

			glm::vec3 vec1 = vertices->at(verticesIndexes->at(1))-vertices->at(verticesIndexes->at(0));
			glm::vec3 vec2 = vertices->at(verticesIndexes->at(2))-vertices->at(verticesIndexes->at(0));
			glm::vec3 normal = glm::cross(vec1,vec2);
			normal = glm::normalize(normal);

			for (int j = 0; j < verticesIndexes->size(); j++) {
				glm::vec3 vertex = vertices->at(verticesIndexes->at(j));

				_vertices[i] = vertex.x;
				_normals[i] = normal.x;
				i++;
				_vertices[i] = vertex.y;
				_normals[i] = normal.y;
				i++;
				_vertices[i] = vertex.z;
				_normals[i] = normal.z;
				i++;

				if (vertex.x < minX) {
					minX = vertex.x;
				}
				else if (vertex.x > maxX) {
					maxX = vertex.x;
				}
				if (vertex.y < minY) {
					minY = vertex.y;
				}
				else if (vertex.y > maxY) {
					maxY = vertex.y;
				}
				if (vertex.z < minZ) {
					minZ = vertex.z;
				}
				else if (vertex.z > maxZ) {
					maxZ = vertex.z;
				}
			}


		}
	}

	_center[0] = minX;
	_center[1] = maxX;
	_center[2] = minY;
	_center[3] = maxY;
	_center[4] = minZ;
	_center[5] = maxZ;
}

float* MeshCollection::getVertices() {
	if (_vertices == nullptr) {
		Log::error("MeshCollection calculateVerticesAndNormals has not been called - \"vertices\" is null");
	}
	else {
		return _vertices;
	}
}

float* MeshCollection::getNormals() {
	if (_normals == nullptr) {
		Log::error("MeshCollection calculateVerticesAndNormals has not been called - \"normals\" is null");
	}
	else {
		return _normals;
	}
}

float* MeshCollection::getCenter() {
	return _center;
}

void MeshCollection::exportModel(){
	std::string filePath = Global::path + "export.obj";

	std::ifstream inputTest(filePath);

	if (inputTest) {
		int i = 0;

		while (1) {
			filePath = Global::path + "export_" + std::to_string(i) + ".obj";

			std::ifstream inputTest(filePath);

			if (!inputTest) {
				break;
			}

			i++;
		}
	}

	std::ofstream output(filePath);

	if (output.is_open()) {
	    unsigned verticesCount = 1;
	    unsigned meshCount = 1;

		for (auto &mesh : _meshes) {
			output<<"g "<<mesh->getName()<<std::endl<<std::endl;

			std::string faces;
			std::string vertices;

			std::shared_ptr<std::vector<glm::vec3>> meshVertices = mesh->getVertices();

			for (auto &vertex : *meshVertices) {
				vertices += "v " + std::to_string(vertex.x) + " " + std::to_string(vertex.y) + " " + std::to_string(vertex.z) + "\n";
			}

			for (auto &face : *(mesh->getFaces())) {
				std::shared_ptr<std::vector<int>> verticesIndexes = face.getVertices();

				faces += "f " + std::to_string(verticesCount+verticesIndexes->at(0)) + " " + std::to_string(verticesCount+verticesIndexes->at(1)) + " "  + std::to_string(verticesCount+verticesIndexes->at(2)) + "\n";
			}

			verticesCount += meshVertices->size();
			meshCount++;

			output<<vertices<<std::endl;
			output<<faces<<std::endl;
		}
		
		output.close();
	}
	else {
		Log::error("Error creating file " + filePath);
	}	
}