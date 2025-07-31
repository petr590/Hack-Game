#include "model.h"
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <fstream>
#include <cassert>

namespace hack_game {
	using glm::vec3;

	using std::cerr;
	using std::endl;
	using std::string;
	using std::vector;
	using std::ifstream;
	using std::numeric_limits;
	using std::streamsize;

	Model::Model(GLuint color, const string& path): color(color) {
		ifstream file(path);

		if (!file.is_open()) {
			throw std::ios_base::failure("Cannot open file '" + path + "'");
		}


		vector<vec3> vertexes;
		vector<vec3> normals;

		for (string tag; file >> tag;) {

			if (tag == "v") {
				vec3& vec = vertexes.emplace_back();
				file >> vec.x >> vec.y >> vec.z;
				continue;
			}

			if (tag == "vn") {
				vec3& vec = normals.emplace_back();
				file >> vec.x >> vec.y >> vec.z;
				continue;
			}

			if (tag == "f") {
				for (int i = 0; i < 3; i++) {
					uint32_t vi; file >> vi;

					file.ignore(numeric_limits<streamsize>::max(), '/');
					file.ignore(numeric_limits<streamsize>::max(), '/');

					uint32_t ni; file >> ni;

					// TODO optimize
					vertices.emplace_back(vertexes[vi - 1], normals[ni - 1]);
					indices.push_back(vertices.size() - 1);
				}

				if (file.get() != '\n') {
					throw std::invalid_argument("File '" + path + "' contains more, than 3 vertices for one face");
				}

				continue;
			}

			file.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}

	void Model::generateVertexArray() {
		GLuint VBO, EBO, VAO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);	
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vertices[0]), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(indices[0]), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, pos)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);

		this->vertexArray = VAO;
	}

	void Model::draw(GLuint vertexColorLocation) const {
		assert(vertexArray != 0);
		
		if (vertexColorLocation != 0) {
			glm::vec3 vertexColor(
				((color >> 16) & 0xFF) / 255.0f,
				((color >>  8) & 0xFF) / 255.0f,
				((color      ) & 0xFF) / 255.0f
			);
			glUniform3fv(vertexColorLocation, 1, glm::value_ptr(vertexColor));
		}

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}
}