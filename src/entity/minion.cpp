#include "minion.h"
#include "player.h"
#include "bullet.h"
#include "model/models.h"
#include "context/tick_context.h"
#include "context/draw_context.h"
#include "util.h"
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using std::isnan;
	using std::make_shared;
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;

	static const float BULLET_PERIOD = 2.0f;
	static const float MINION_RADIUS = 0.015f;
	static const float MINION_SPEED = TILE_SIZE * 0.5f;

	Minion::Minion(DrawContext& drawContext, const glm::vec3& pos) noexcept:
			SimpleEntity(drawContext.mainShader, models::minion),
			Damageable(Side::ENEMY, 1),
			bulletShader(drawContext.lightShader),
			pos(pos) {}
	
	
	void Minion::tick(TickContext& context) {
		float newAngle = horizontalAngleBetween(pos, context.player->getPos());
		if (!isnan(newAngle))
			angle = newAngle;
		
		vec2 offset = glm::rotate(context.deltaTime * MINION_SPEED * ANGLE_NORMAL, angle);
		offset = resolveBlockCollision(context, vec2(pos.x, pos.z), offset);
		pos += vec3(offset.x, 0, offset.y);


		time += context.deltaTime;

		if (time >= BULLET_PERIOD) {
			time -= BULLET_PERIOD;

			vec2 velocity = glm::rotate(ANGLE_NORMAL * EnemyBullet::DEFAULT_SPEED, angle);
			
			context.addEntity(make_shared<EnemyBullet>(
				bulletShader, false, vec3(velocity.x, 0, velocity.y), pos
			));
		}
	}


	mat4 Minion::getModelTransform() const {
		mat4 model(1.0f);
		model = glm::translate(model, pos);
		return  glm::rotate(model, angle, vec3(0.0f, -1.0f, 0.0f));
	}


	bool Minion::hasCollision(const vec3& point) const {
		return isPointInsideSphere(point, pos, MINION_RADIUS);
	}
}