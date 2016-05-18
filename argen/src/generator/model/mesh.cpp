#include "mesh.h"
#include "../../functions/advanced/rotate.h"
#include "../../utils/log.h"

#include <iostream>

Mesh::Mesh(glm::vec3 position, glm::vec3 rotation) {
	_position = position;
	_rotation = rotation;
	_faces = std::make_shared<std::vector<Face>>();
	_vertices = std::make_shared<std::vector<glm::vec3>>();

	_name = "placeholder";
}

Mesh::~Mesh() {
	_faces.reset();
	_faces = nullptr;
}

void Mesh::setName(std::string name) {
	_name = name;
}

std::string Mesh::getName() {
	return _name;
}

void Mesh::addVertex(glm::vec3 vertex) {
	_vertices->push_back(processVertex(vertex));
}

void Mesh::addFace(int v1, int v2, int v3) {
	if (v1 < _vertices->size() && v1 < _vertices->size() && v1 < _vertices->size()) {
		_faces->push_back(Face(v1, v2, v3));
	}
	else {
		Log::error("Invalid vertex index.");
	}
}

int Mesh::getFacesSize() {
	return _faces->size();
}

std::shared_ptr<std::vector<Face>> Mesh::getFaces() {
	return _faces;
}

std::shared_ptr<std::vector<glm::vec3>> Mesh::getVertices() {
	return _vertices;
}

glm::vec3 Mesh::processVertex(glm::vec3 vertex) {
	glm::vec3 origin;
	glm::vec3 rotatedVector = Rotate::rotate(vertex, origin, _rotation);
	glm::vec3 processedVector(
		rotatedVector.x+_position.x,
		rotatedVector.y+_position.y,
		rotatedVector.z+_position.z
	);

	return processedVector;
}