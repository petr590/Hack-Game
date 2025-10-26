#include "postprocessing_model.h"
#include "debug.h"

#define GLEW_STATIC
#include <GL/glew.h>

namespace hack_game {

	struct PostprocessingModel::Vertex {
		const glm::vec2 pos;
		const glm::vec2 texCoord;
	};

	PostprocessingModel::PostprocessingModel():
			vertices {
				Vertex { .pos {-1, -1}, .texCoord {0, 0} },
				Vertex { .pos {-1,  1}, .texCoord {0, 1} },
				Vertex { .pos { 1, -1}, .texCoord {1, 0} },
				Vertex { .pos { 1,  1}, .texCoord {1, 1} },
			} {
		
		indices = {
			2, 1, 0,
			1, 2, 3,
		};
	}

	PostprocessingModel::~PostprocessingModel() {}


	GLuint PostprocessingModel::createVertexArray() {
		GLuint VBO, EBO, VAO;
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);
		glGenVertexArrays(1, &VAO);
		
		
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);	
		glBufferData(GL_ARRAY_BUFFER, GLsizeiptr(vertices.size() * sizeof(vertices[0])), vertices.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);	
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, GLsizeiptr(indices.size() * sizeof(indices[0])), indices.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, pos)));
		glEnableVertexAttribArray(0);

		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<GLvoid*>(offsetof(Vertex, texCoord)));
		glEnableVertexAttribArray(1);
		
		glBindVertexArray(0);
		return VAO;
	}
}