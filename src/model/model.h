#ifndef HACK_GAME__MODEL__MODEL_H
#define HACK_GAME__MODEL__MODEL_H

#include "../gl_fwd.h"
#include <vector>
#include <glm/vec3.hpp>

namespace hack_game {

	class DrawContext;

	struct Vertex {
		const glm::vec3 pos;
		const glm::vec3 normal;

		constexpr Vertex(const glm::vec3 pos, const glm::vec3 normal) noexcept:
			pos(pos), normal(normal) {}
	};

	
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
}

#endif
