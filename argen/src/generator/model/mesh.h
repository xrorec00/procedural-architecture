#ifndef MESH_H
#define MESH_H

#include <memory>
#include <string>
#include <vector>
#include "../../../libs/glm/glm/vec3.hpp"
#include "face.h"

class Mesh {
	private:
		std::string _name;

		std::shared_ptr<std::vector<Face>> _faces;
		std::shared_ptr<std::vector<glm::vec3>> _vertices;
		glm::vec3 _position;
		glm::vec3 _rotation;
	public:
		Mesh(glm::vec3, glm::vec3);
		~Mesh();

		void setName(std::string);
		std::string getName();

		void addVertex(glm::vec3);
		void addFace(int, int, int);
		int getFacesSize();

		std::shared_ptr<std::vector<Face>> getFaces();
		std::shared_ptr<std::vector<glm::vec3>> getVertices();
	private:
		glm::vec3 processVertex(glm::vec3);
};

#endif