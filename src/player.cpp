#include "player.h"
#include "block.h"
#include "bullet.h"
#include "models.h"
#include <algorithm>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::max;
	using std::min;
	using std::vector;
	using std::make_shared;

	using glm::uvec2;
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;
	using glm::quat;

	const float PAD = 0.015f;
	const float BULLET_PERIOD = 0.05f;


	Player::Player(DrawContext& drawContext, DrawContext& bulletDrawContext, float speed, const Camera& camera):
			Entity(drawContext),
			bulletDrawContext(bulletDrawContext),
			model(playerModel),
			speed(speed),
			camera(camera),
			pos(TILE_SIZE * 8, TILE_SIZE / 2, TILE_SIZE * 8) {
		
		this->camera.move(pos);
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

			// case GLFW_KEY_Z:
			// 	printf("(%.2f, %.2f, %.2f)\n", camera.target.x, camera.target.y, camera.target.z);
			// 	printf("(%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);
			// 	camera.move(-pos);
			// 	pos = vec3(0.0f);
			// 	angle = 0.0f;
			// 	break;
		}
	}


	static const float EPSILON = 1e-6f;


	static vec2 resolveCollision(const TickContext& context, const vec2& pos, vec2 offset, const uvec2& mapPos) {
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


	static vec2 moveWithCollisions(const TickContext& context, const vec2& pos, vec2 offset) {
		const vec2 newPos = pos + offset;
		const uvec2 minPos = context.getMapPos(glm::min(pos, newPos) - EPSILON);
		const uvec2 maxPos = context.getMapPos(glm::max(pos, newPos) + EPSILON);

		offset = resolveCollision(context, pos, offset, minPos);

		if (minPos.x != maxPos.x) {
			offset = resolveCollision(context, pos, offset, uvec2(maxPos.x, minPos.y));
		}

		if (minPos.y != maxPos.y) {
			offset = resolveCollision(context, pos, offset, uvec2(minPos.x, maxPos.y));

			if (minPos.x != maxPos.x) {
				offset = resolveCollision(context, pos, offset, maxPos);
			}
		}

		return offset;
	}


	void Player::move(TickContext& context) {
		vec2 offset(
			left ? -1 : right ? 1 : 0,
			up ? -1 : down ? 1 : 0
		);

		if (glm::length(offset) == 0) return;

		offset = glm::normalize(offset) * speed * context.deltaTime;
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


	const float ROTATE_SPEED = 360 * 3;

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

			quat rotateQuat = angleAxis(angle, vec3(0.0f, 1.0f, 0.0f));
			vec3 velocity = rotateQuat * vec3(0.0f, 0.0f, -1.0f);
			vec3 bulletPos = pos + velocity * (TILE_SIZE / 2);

			context.addEntity(make_shared<Bullet>(bulletDrawContext, playerBulletModel, angle, velocity, bulletPos));
		}
	}

	void Player::draw() const {
		Entity::draw();

		mat4 modelMat(1.0f);
		modelMat = translate(modelMat, pos);
		modelMat = rotate(modelMat, angle, vec3(0.0f, 1.0f, 0.0f));
		modelMat = scale(modelMat, vec3(0.1f));
		glUniformMatrix4fv(drawContext.modelLocation, 1, GL_FALSE, value_ptr(modelMat));

		model.draw(drawContext);
	}
}