#include "simple_model.h"
#include "context/draw_context.h"
#include "util.h"

#include <fstream>
#include <map>

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::string;
	using std::vector;
	using std::map;
	using std::pair;
	using std::streamsize;
	using std::numeric_limits;

	using glm::vec3;


	SimpleModel::SimpleModel(uint32_t color, const string& path): VAOModel(color) {
		std::ifstream file(path);

		if (!file.is_open()) {
			throw std::ios_base::failure("Cannot open file '" + path + "'");
		}

		vector<vec3> positions;
		vector<vec3> normals;
		map<pair<uint32_t, uint32_t>, uint32_t> verticesMap; // Ключ: {позиция, нормаль}, значение: индекс

		for (string tag; file >> tag;) {

			if (tag == "v") {
				vec3& vec = positions.emplace_back();
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
					uint32_t posIndex; file >> posIndex;

					file.ignore(numeric_limits<streamsize>::max(), '/');
					file.ignore(numeric_limits<streamsize>::max(), '/');

					uint32_t normalIndex; file >> normalIndex;
					posIndex--, normalIndex--;

					auto it = verticesMap.find({posIndex, normalIndex});

					if (it != verticesMap.end()) {
						indices.push_back(it->second);

					} else {
						vertices.emplace_back(positions[posIndex], normals[normalIndex]);
						
						const uint32_t index = vertices.size() - 1;
						verticesMap[{posIndex, normalIndex}] = index;
						indices.push_back(index);
					}
				}

				if (file.get() != '\n') {
					throw std::invalid_argument("File '" + path + "' contains more, than 3 vertices for one face");
				}

				continue;
			}

			file.ignore(numeric_limits<streamsize>::max(), '\n');
		}
	}


	SimpleModel::SimpleModel(uint32_t color, const SimpleModel& model):
		VAOModel(color, model),
		vertices(model.vertices) {}


	GLuint SimpleModel::getVertexArray() const {
		GLuint VBO, EBO, VAO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);	
		glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(vertices[0])), &vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(indices.size() * sizeof(indices[0])), &indices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, pos)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
		return VAO;
	}
}
