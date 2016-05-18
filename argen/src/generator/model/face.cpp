#include "face.h"

#include <iostream>

/*
 * Face of mesh. Made of 3 vertices and contains 3 normals for each vertex.
 */

Face::Face(int v1, int v2, int v3) {
	// Vertices
	_vertices = std::make_shared<std::vector<int>>();
	_vertices->push_back(v1);
	_vertices->push_back(v2);
	_vertices->push_back(v3);
}

Face::~Face() {
	_vertices.reset();
	_vertices = nullptr;
}

std::shared_ptr<std::vector<int>> Face::getVertices() {
	return _vertices;
}
