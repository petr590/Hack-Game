#ifndef HACK_GAME_MODEL_H
#define HACK_GAME_MODEL_H

#include "vertex.h"
#include "draw_context.h"
#define GLEW_STATIC
#include <GL/glew.h>
#include <string>
#include <vector>

namespace hack_game {
	
	class Model {
		static std::vector<Model*> models;
	
	public:
		static const std::vector<Model*>& getModels() noexcept {
			return models;
		}

		Model();
		Model(const Model&) = delete;
		Model& operator=(const Model&) = delete;

		virtual ~Model() = default;

		virtual void generateVertexArray() = 0;
		virtual void draw(DrawContext&) const = 0;
		virtual void draw(DrawContext&, const glm::vec3& color) const = 0;
	};


	class SimpleModel: public Model {
		std::vector<Vertex> vertices;
		std::vector<GLuint> indices;
		GLuint vertexArray = 0;
	
	public:
		const glm::vec3 color;

		SimpleModel(GLuint color, const std::string& path);
		SimpleModel(GLuint color, const SimpleModel&);

		void generateVertexArray() override;
		void draw(DrawContext&) const override;
		void draw(DrawContext&, const glm::vec3& color) const override;
	};


	class CompositeModel: public Model {
		std::vector<Model*> models;

	public:
		CompositeModel(std::initializer_list<Model*> models);

		void generateVertexArray() override;
		void draw(DrawContext&) const override;
		void draw(DrawContext&, const glm::vec3& color) const override;
	};
}

#endif