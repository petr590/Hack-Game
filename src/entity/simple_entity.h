#ifndef HACK_GAME__ENTITY__SIMPLE_ENTITY_H
#define HACK_GAME__ENTITY__SIMPLE_ENTITY_H

#include "entity.h"
#include <glm/mat4x4.hpp>

namespace hack_game {

	class SimpleEntity: public virtual Entity {
	protected:
		Shader& shader;
		const Model& model;

	public:
		SimpleEntity(Shader& shader, const Model& model) noexcept:
				shader(shader), model(model) {}

		GLuint getShaderProgram() const noexcept override;
		void tick(TickContext&) override {}
		void draw() const override;
		virtual glm::mat4 getModelTransform() const;
	};
}

#endif