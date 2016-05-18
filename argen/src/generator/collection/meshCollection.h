#ifndef MESHCOLLECTION_H
#define MESHCOLLECTION_H

#include <memory>
#include <vector>

#include "../model/mesh.h"

class MeshCollection {
	private:
		std::vector<std::shared_ptr<Mesh>> _meshes;
		float _center[6];

		float* _vertices;
		float* _normals;
	public:
		~MeshCollection();

		void addMesh(std::shared_ptr<Mesh> mesh);
		unsigned getVerticesNum();

		void calculateVerticesAndNormals();
		float* getVertices();
		float* getNormals();
		float* getCenter();

		void exportModel();
};

#endif