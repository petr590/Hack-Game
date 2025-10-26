#include "block.h"
#include "model/models.h"
#include "shader/shader.h"
#include "level/level.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::max;
	using std::shared_ptr;
	using std::make_shared;

	using glm::uvec2;
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	Block::Block(Shader& shader, const ColoredModel& model, hp_t hitpoints, const uvec2& pos):
			SimpleEntity(shader, model),
			Damageable(Side::ENEMY, hitpoints),
			coloredModel(model),
			pos(pos) {}
	

	shared_ptr<Block> Block::breakable(Shader& shader, const uvec2& pos) {
		return make_shared<Block>(shader, models::breakableCube, 3, pos);
	}
	
	shared_ptr<Block> Block::unbreakable(Shader& shader, const uvec2& pos) {
		return make_shared<Block>(shader, models::unbreakableCube, Damageable::MAX_HP, pos);
	}


	AABB Block::getHitbox() const {
		vec2 center = (vec2(pos) + 0.5f) * TILE_SIZE;

		return AABB(
			center - TILE_SIZE / 2,
			center + TILE_SIZE / 2
		);
	}


	bool Block::hasCollision(const vec3& pos) const {
		return getHitbox().containsInclusive(vec2(pos.x, pos.z));
	}


	const float DAMAGE_ANIMATION_DURATION = 0.25f;

	void Block::damage(Level& level, hp_t damage) {
		Damageable::damage(level, damage);

		if (!invulnerable() && damageAnimationTime <= 0) {
			damageAnimationTime = DAMAGE_ANIMATION_DURATION;
		}
	}

	void Block::onDestroy(Level& level) {
		level.map[pos] = nullptr;
	}

	void Block::tick(Level& level) {
		if (damageAnimationTime > 0) {
			damageAnimationTime -= level.getDeltaTime();

			if (damageAnimationTime <= 0 && destroyed()) {
				level.removeEntity(shared_from_this());
			}
		}
	}

	void Block::draw() const {
		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, vec3(
			(pos.x + 0.5f) * TILE_SIZE,
			0.0f,
			(pos.y + 0.5f) * TILE_SIZE
		));

		shader.setModel(modelMat);

		float brightness = max(0.0f, damageAnimationTime * (1.25f / DAMAGE_ANIMATION_DURATION));
		coloredModel.draw(shader, coloredModel.getColor() + brightness);
	}
}