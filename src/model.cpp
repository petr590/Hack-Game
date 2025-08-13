#include "model.h"
#include "util.h"
#include <glm/gtc/type_ptr.hpp>
#include <map>
#include <fstream>
#include <cassert>

namespace hack_game {
	using glm::vec3;

	using std::string;
	using std::vector;
	using std::map;
	using std::pair;
	using std::ifstream;
	using std::numeric_limits;
	using std::streamsize;

	vector<Model*> Model::models;

	Model::Model() {
		models.push_back(this);
	}

	// ---------------------------------------- SimpleModel ----------------------------------------

	SimpleModel::SimpleModel(GLuint color, const string& path): color(colorAsVec3(color)) {
		ifstream file(path);

		if (!file.is_open()) {
			throw std::ios_base::failure("Cannot open file '" + path + "'");
		}


		vector<vec3> vertexes;
		vector<vec3> normals;
		map<pair<uint32_t, uint32_t>, uint32_t> verticesMap;

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
					vi--, ni--;

					auto it = verticesMap.find({vi, ni});

					if (it != verticesMap.end()) {
						indices.push_back(it->second);
					} else {
						vertices.emplace_back(vertexes[vi], normals[ni]);
						
						uint32_t index = vertices.size() - 1;
						verticesMap[{vi, ni}] = index;
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


	SimpleModel::SimpleModel(GLuint color, const SimpleModel& model):
		vertices(model.vertices), indices(model.indices), color(colorAsVec3(color)) {}


	void SimpleModel::generateVertexArray() {
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

	void SimpleModel::draw(DrawContext& context) const {
		draw(context, color);
	}

	void SimpleModel::draw(DrawContext& context, const vec3& color) const {
		glUseProgram(context.shaderProgram);
		glUniform3fv(context.modelColorLocation, 1, glm::value_ptr(color));

		assert(vertexArray != 0);

		glBindVertexArray(vertexArray);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
		glBindVertexArray(0);
	}


	// ---------------------------------------- CompositeModel ----------------------------------------

	CompositeModel::CompositeModel(std::initializer_list<Model*> models):
			models(models) {}
	
	void CompositeModel::generateVertexArray() {}

	void CompositeModel::draw(DrawContext& context) const {
		for (const Model* model : models) {
			model->draw(context);
		}
	}

	void CompositeModel::draw(DrawContext& context, const vec3& color) const {
		for (const Model* model : models) {
			model->draw(context, color);
		}
	}
}