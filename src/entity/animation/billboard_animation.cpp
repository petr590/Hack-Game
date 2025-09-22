#include "billboard_animation.h"
#include "entity/player.h"
#include "context/tick_context.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {

	using glm::vec3;
	using glm::vec4;
	using glm::mat3;
	using glm::mat4;

	BillboardAnimation::BillboardAnimation(std::shared_ptr<const EntityWithPos>&& entity, Shader& shader, Model& model, float size, float duration) noexcept:
			Animation(std::move(entity), shader, model, size, duration) {}

	BillboardAnimation::BillboardAnimation(std::shared_ptr<const EntityWithPos>&& entity, Shader& shader, float size, float duration) noexcept:
			Animation(std::move(entity), shader, size, duration) {}
	

	void BillboardAnimation::tick(TickContext& context) {
		Animation::tick(context);
		viewMat3 = mat3(context.player->getCamera().getView());
	}


	mat4 BillboardAnimation::getModelTransform() const {
		mat4 modelMat(glm::transpose(viewMat3));
		modelMat[3] = vec4(pos, 1.0f);

		modelMat = glm::rotate(modelMat, glm::radians(90.0f), vec3(modelMat[0]));
		return glm::scale(modelMat, vec3(size));
	}
}