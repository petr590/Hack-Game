#include "frame_model.h"
#include "context/draw_context.h"
#include "util.h"

#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::string;
	using std::streamsize;
	using std::numeric_limits;

	using glm::vec3;

	FrameModel::FrameModel(uint32_t color, const string& path): VAOModel(color) {
		std::ifstream file(path);

		if (!file.is_open()) {
			throw std::ios_base::failure("Cannot open file '" + path + "'");
		}

		for (string tag; file >> tag;) {

			if (tag == "v") {
				vec3& vec = vertices.emplace_back();
				file >> vec.x >> vec.y >> vec.z;
				continue;
			}

			if (tag == "l") {
				file >> indices.emplace_back();
				file >> indices.emplace_back();
				continue;
			}

			file.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}


	GLuint FrameModel::getVertexArray() const {
		GLuint VBO, EBO, VAO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);	
		glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(vertices[0])), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(indices.size() * sizeof(indices[0])), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);
		
		glBindVertexArray(0);
		return VAO;
	}

	void FrameModel::draw(Shader& shader, const glm::vec3& color) const {
		glBegin(GL_LINES);
		VAOModel::draw(shader, color);
		glEnd();
	}
}