#include "player.h"
#include "enemy.h"
#include "block.h"
#include "bullet.h"
#include "aabb.h"
#include "../model/models.h"
#include "../context/draw_context.h"
#include "../context/tick_context.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::max;
	using std::min;
	using std::pow;
	using std::sqrt;
	using std::isnan;
	using std::make_shared;

	using glm::uvec2;
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;
	using glm::quat;

	const float PAD = 0.015f;
	const float BULLET_PERIOD = 0.1f;


	Player::Player(DrawContext& drawContext, DrawContext& bulletDrawContext, float speed, const Camera& camera):
			drawContext(drawContext),
			bulletDrawContext(bulletDrawContext),
			speed(speed),
			camera(camera),
			pos(TILE_SIZE * 8, 0.0f, TILE_SIZE * 8) {
		
		this->camera.move(pos);
	}

	GLuint Player::getShaderProgram() const {
		return drawContext.shaderProgram;
	}

	void Player::updateAngle(float newTargetAngle) {
		targetAngle = newTargetAngle;

		if (angle > targetAngle) {
			if (angle - targetAngle > glm::radians(180.f)) {
				angle -= glm::radians(360.f);
			}

		} else {
			if (targetAngle - angle > glm::radians(180.f)) {
				angle += glm::radians(360.f);
			}
		}
	}

	void Player::onKey(int key, int action) {
		if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

		bool value = action == GLFW_PRESS;
		switch (key) {
			case GLFW_KEY_W: up = value; break;
			case GLFW_KEY_S: down = value; break;
			case GLFW_KEY_A: left = value; break;
			case GLFW_KEY_D: right = value; break;

			case GLFW_KEY_LEFT_SHIFT: fire = value; break;

			case GLFW_KEY_UP:    updateAngle(glm::radians(0.f));    break;
			case GLFW_KEY_LEFT:  updateAngle(glm::radians(90.f));   break;
			case GLFW_KEY_DOWN:  updateAngle(glm::radians(180.f));  break;
			case GLFW_KEY_RIGHT: updateAngle(glm::radians(270.f));  break;
		}
	}


	static const float EPSILON = 1e-6f;


	static vec2 resolveBlockCollision(const TickContext& context, const vec2& pos, vec2 offset, const uvec2& mapPos) {
		if (context.map[mapPos] == nullptr) {
			return offset;
		}

		AABB block = context.map[mapPos]->getHitbox();

		vec2 newPos = pos + offset;
		
		if (offset.x != 0 && block.containsInclusive(vec2(newPos.x, pos.y))) {
			if (offset.x > 0) offset.x = max(0.0f, offset.x + (block.min.x - newPos.x - EPSILON));
			else              offset.x = min(0.0f, offset.x + (block.max.x - newPos.x + EPSILON));
		}

		if (offset.y != 0 && block.containsInclusive(vec2(pos.x, newPos.y))) {
			if (offset.y > 0) offset.y = max(0.0f, offset.y + (block.min.y - newPos.y - EPSILON));
			else              offset.y = min(0.0f, offset.y + (block.max.y - newPos.y + EPSILON));
		}

		return offset;
	}


	static constexpr vec2 vec3ToVec2(const vec3& pos) {
		return vec2(pos.x, pos.z);
	}


	static constexpr float INF = std::numeric_limits<float>().infinity();
	static constexpr float NaN = std::numeric_limits<float>().quiet_NaN();

	/**
	 * Ищет точку пересечения отрезка и окружности. Если таких две, возвращает любую из них.
	 * @param[in] pos Первая точка отрезка
	 * @param[in] offset Смещение от первой точки отрезка до второй
	 * @param[in] center Центр окружности
	 * @param[in] center Радиус окружности
	 * @return Точку пересечения отрезка и окружности. Если её нет, возвращает `vec2(NaN)`.
	 *         Точка может быть за пределами отрезка максимум на @ref EPSILON по обоим осям x и y.
	 */
	static constexpr vec2 getIntersectPoint(const vec2& pos, const vec2& offset, const vec2& center, float radius) {
		const float cx = center.x;
		const float cy = center.y;

		if (offset.x == 0) {
			const float A = 1;
			const float B = -2 * cy;
			const float C = pow(pos.x - cx, 2) + cy*cy - radius*radius;

			const float D = B*B - 4*A*C;
			if (D < 0) {
				return vec2(NaN);
			}

			const float sqrtD = sqrt(D);

			const float y1 = (-B + sqrtD) / (2*A);
			const float y2 = (-B - sqrtD) / (2*A);

			const vec2 newPos = pos + offset;

			if (y1 >= min(pos.y, newPos.y) && y1 <= max(pos.y, newPos.y)) {
				return vec2(pos.x, y1);
			}

			if (y2 >= min(pos.y, newPos.y) && y2 <= max(pos.y, newPos.y)) {
				return vec2(pos.x, y2);
			}

			return vec2(NaN);
		}

		const float a = offset.x == 0 ? INF : offset.y / offset.x;
		const float b = pos.y - a * pos.x;

		const float A = a*a + 1;
		const float B = 2 * (a * (b - cy) - cx);
		const float C = pow(b - cy, 2) + cx*cx - radius*radius;

		const float D = B*B - 4*A*C;
		if (D < 0) {
			return vec2(NaN);
		}

		const float sqrtD = sqrt(D);

		const float x1 = (-B + sqrtD) / (2*A);
		const float x2 = (-B - sqrtD) / (2*A);

		const float y1 = a * x1 + b;
		const float y2 = a * x2 + b;

		const vec2 newPos = pos + offset;
		const float minX = min(pos.x, newPos.x) - EPSILON;
		const float minY = min(pos.y, newPos.y) - EPSILON;
		const float maxX = max(pos.x, newPos.x) + EPSILON;
		const float maxY = max(pos.y, newPos.y) + EPSILON;

		if (x1 >= minX && x1 <= maxX &&
			y1 >= minY && y1 <= maxY) {
			return vec2(x1, y1);
		}

		if (x1 != x2 &&
			x2 >= minX && x2 <= maxX &&
			y2 >= minY && y2 <= maxY) {
			return vec2(x2, y2);
		}

		return vec2(NaN);
	}

	static constexpr bool all_isnan(const vec2& v) {
		return isnan(v.x) && isnan(v.y);
	}

	static_assert(getIntersectPoint(vec2(1, 1), vec2(0, -2), vec2(0, 0), 1) == vec2(1, 0));
	static_assert(all_isnan(getIntersectPoint(vec2(2, 1), vec2(0, -2), vec2(0, 0), 1)));

	static_assert(getIntersectPoint(vec2(0, 0), vec2(1, 1), vec2(0, 0), 1) == vec2(sqrt(0.5f), sqrt(0.5f)));
	static_assert(all_isnan(getIntersectPoint(vec2(1, 1), vec2(2, 2), vec2(0, 0), 1)));

	static_assert(!all_isnan(getIntersectPoint(vec2(0.5802, 0.2530), vec2(0.0000, 0.0042), vec2(0.5750, 0.2750), 0.0200)));
	static const vec2 V = getIntersectPoint(vec2(0.5802, 0.2530), vec2(0.0000, 0.0042), vec2(0.5750, 0.2750), 0.0200);


	static vec2 resolveEnemyCollision(const TickContext& context, const vec2& pos, vec2 offset) {
		const vec2 newPos = pos + offset;

		const vec2 enemyPos = vec3ToVec2(context.enemy->getPos());
		const vec2 diff = newPos - enemyPos;

		const float DIST = Enemy::RADIUS + PAD;

		if (glm::dot(diff, diff) <= DIST * DIST) {
			vec2 point = getIntersectPoint(pos, offset, enemyPos, DIST + EPSILON);

			if (!isnan(point.x) && !isnan(point.y)) {
				offset = point - pos;
			}
		}

		return offset;
	}


	static vec2 moveWithCollisions(const TickContext& context, const vec2& pos, vec2 offset) {
		const vec2 newPos = pos + offset;
		const uvec2 minPos = context.getMapPos(glm::min(pos, newPos) - EPSILON);
		const uvec2 maxPos = context.getMapPos(glm::max(pos, newPos) + EPSILON);

		offset = resolveBlockCollision(context, pos, offset, minPos);

		if (minPos.x != maxPos.x) {
			offset = resolveBlockCollision(context, pos, offset, uvec2(maxPos.x, minPos.y));
		}

		if (minPos.y != maxPos.y) {
			offset = resolveBlockCollision(context, pos, offset, uvec2(minPos.x, maxPos.y));

			if (minPos.x != maxPos.x) {
				offset = resolveBlockCollision(context, pos, offset, maxPos);
			}
		}

		offset = resolveEnemyCollision(context, pos, offset);
		return offset;
	}


	void Player::move(TickContext& context) {
		vec2 offset(
			left ? -1 : right ? 1 : 0,
			up ? -1 : down ? 1 : 0
		);

		if (offset.x == 0 && offset.y == 0) return;

		offset = glm::normalize(offset) * (speed * context.deltaTime);
		offset = moveWithCollisions(context, vec2(pos.x, pos.z), offset);

		vec3 offset3d(offset.x, 0.0f, offset.y);
		pos += offset3d;

		vec3 oldPos = pos;

		const float endX = context.map.width() * TILE_SIZE;
		const float endZ = context.map.height() * TILE_SIZE;

		pos = glm::clamp(pos,
				vec3(       PAD, -INFINITY,        PAD),
				vec3(endX - PAD,  INFINITY, endZ - PAD)
		);

		offset3d += pos - oldPos;
		camera.move(offset3d);
	}


	const float ROTATE_SPEED = 360 * 3; // deg/sec
	const float BULLET_SPEED = 15 * TILE_SIZE; // m/sec

	void Player::tick(TickContext& context) {
		move(context);

		if (angle != targetAngle) {
			float delta = glm::radians(ROTATE_SPEED) * context.deltaTime;

			if (angle > targetAngle) {
				angle = max(angle - delta, targetAngle);
			} else {
				angle = min(angle + delta, targetAngle);
			}
		}

		timeSinceLastBullet += context.deltaTime;

		if (fire && timeSinceLastBullet >= BULLET_PERIOD) {
			timeSinceLastBullet = fmodf(timeSinceLastBullet, BULLET_PERIOD);

			quat rotateQuat = glm::angleAxis(angle, vec3(0.0f, 1.0f, 0.0f));
			vec3 velocity = rotateQuat * vec3(0.0f, 0.0f, -1.0f) * BULLET_SPEED;
			vec3 bulletPos = pos + velocity * (TILE_SIZE * 0.5f);

			context.addEntity(make_shared<PlayerBullet>(
				bulletDrawContext, angle, velocity, bulletPos
			));
		}
	}



	void Player::draw() const {
		glUseProgram(drawContext.shaderProgram);

		mat4 modelMat(1.0f);
		modelMat = translate(modelMat, pos);
		modelMat = rotate(modelMat, angle, vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(drawContext.modelUniform, 1, GL_FALSE, glm::value_ptr(modelMat));

		if (hitpoints >= 3) models::player3hp.draw(drawContext);
		if (hitpoints == 2) models::player2hp.draw(drawContext);
		if (hitpoints <= 1) models::player1hp.draw(drawContext);
	}

	void Player::damage(TickContext& context, int32_t damage) {
		hitpoints -= damage;
	}
}
