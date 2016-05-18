#ifndef FACE_H
#define FACE_H

#include <memory>
#include <vector>
#include "../../../libs/glm/glm/vec3.hpp"

class Face {
	private:
		std::shared_ptr<std::vector<int>> _vertices;
	public:
		Face(int, int, int);
		~Face();

		std::shared_ptr<std::vector<int>> getVertices();
};

#endif