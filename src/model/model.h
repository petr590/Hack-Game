#ifndef HACK_GAME__MODEL__MODEL_H
#define HACK_GAME__MODEL__MODEL_H

#include "gl_fwd.h"
#include <vector>
#include <glm/vec3.hpp>

namespace hack_game {

	class Shader;

	
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
		virtual void draw(Shader&) const = 0;
		virtual void draw(Shader&, const glm::vec3& color) const = 0;
	};
}

#endif