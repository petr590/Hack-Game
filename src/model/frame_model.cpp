#include "frame_model.h"
#include "context/shader.h"
#include "dir_paths.h"
#include "util.h"

#include <fstream>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::string;
	using std::ifstream;
	using std::streamsize;
	using std::numeric_limits;

	using glm::vec3;

	FrameModel::FrameModel(uint32_t color, const char* relativePath):
			color(colorAsVec3(color))
	{
		const string path = string(MODELS_DIR) + relativePath;
		ifstream file(path);

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
				indices.back() -= 1;
				
				file >> indices.emplace_back();
				indices.back() -= 1;
				continue;
			}

			file.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}


	GLuint FrameModel::createVertexArray() {
		GLuint buffers[2], VAO;
		glGenBuffers(2, buffers);
		glGenVertexArrays(1, &VAO);
		
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);	
		glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(vertices[0])), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(indices.size() * sizeof(indices[0])), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
		glEnableVertexAttribArray(0);
		
		glBindVertexArray(0);
		return VAO;
	}

	void FrameModel::draw(Shader& shader) const {
		shader.setModelColor(color);
		VAOModel::draw(shader);
	}
}