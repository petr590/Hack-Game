#include "player.h"
#include "bullet.h"
#include "models.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::max;
	using std::min;
	using std::clamp;
	using std::vector;
	using std::make_shared;

	using glm::ivec2;
	using glm::vec2;
	using glm::vec3;
	using glm::mat4;
	using glm::quat;
	using glm::length;
	using glm::radians;
	using glm::angleAxis;
	using glm::normalize;
	using glm::translate;
	using glm::rotate;
	using glm::scale;

	const float PLAYER_RADIUS = 0.03f;

	const float BULLET_PERIOD = 0.05f;


	Player::Player(DrawContext& drawContext, DrawContext& bulletDrawContext, const Model& model, float speed, const Camera& camera):
			Entity(drawContext),
			bulletDrawContext(bulletDrawContext),
			model(model),
			speed(speed),
			camera(camera),
			pos(TILE_SIZE * 8, TILE_SIZE / 2, TILE_SIZE * 8) {
		
		this->camera.move(pos);
	}

	void Player::updateAngle(float newTargetAngle) {
		targetAngle = newTargetAngle;

		if (angle > targetAngle) {
			if (angle - targetAngle > radians(180.f)) {
				angle -= radians(360.f);
			}

		} else {
			if (targetAngle - angle > radians(180.f)) {
				angle += radians(360.f);
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

			case GLFW_KEY_UP:    updateAngle(radians(0.f));    break;
			case GLFW_KEY_LEFT:  updateAngle(radians(90.f));   break;
			case GLFW_KEY_DOWN:  updateAngle(radians(180.f));  break;
			case GLFW_KEY_RIGHT: updateAngle(radians(270.f));  break;

			// case GLFW_KEY_Z:
			// 	printf("(%.2f, %.2f, %.2f)\n", camera.target.x, camera.target.y, camera.target.z);
			// 	printf("(%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);
			// 	camera.move(-pos);
			// 	pos = vec3(0.0f);
			// 	angle = 0.0f;
			// 	break;
		}
	}


	struct box2 {
		float sx, sy, ex, ey;

		box2(float sx, float sy, float ex, float ey):
			sx(sx), sy(sy), ex(ex), ey(ey) {}

		box2(const vec2& start, const vec2& end):
			sx(start.x), sy(start.y), ex(end.x), ey(end.y) {}
	};


	static vec2 getOffset(const vec2& center, float radius, const box2& cube) {
		float dx = center.x - clamp(center.x, cube.sx, cube.ex);
		float dy = center.y - clamp(center.y, cube.sy, cube.ey);

		// printf("center.x = %.4f, cube.sx = %.4f, cube.ex = %.4f\n", center.x, cube.sx, cube.ex);
		// printf("center.y = %.4f, cube.sy = %.4f, cube.ey = %.4f\n", center.y, cube.sy, cube.ey);

		// printf("dx = %.4f, dy = %.4f, radius = %.4f\n", dx, dy, radius);

		if (dx*dx + dy*dy < radius * radius) {
			// printf("collision\n");

			// printf("dx = %.4f, dy = %.4f, radius = %.4f\n", dx, dy, radius);

			if (dx == 0 && dy == 0) {
				// printf("ZERO\n");
				return vec2(0.0f); // TODO
			}

			vec2 dist(dx, dy);
			return normalize(dist) * radius - dist;
		}

		// printf("no collision\n");
		return vec2(0.0f);
	}


	static vec2 moveWithCollisions(const vec3& playerPos, vec2 offset, const map_t& map) {
		const int32_t minMapX = clamp((playerPos.x - PLAYER_RADIUS / 2) / TILE_SIZE, 0.0f, float(map.size() - 1));
		const int32_t minMapZ = clamp((playerPos.z - PLAYER_RADIUS / 2) / TILE_SIZE, 0.0f, float(map[0].size() - 1));
		const int32_t maxMapX = clamp((playerPos.x + PLAYER_RADIUS / 2) / TILE_SIZE, 0.0f, float(map.size() - 1));
		const int32_t maxMapZ = clamp((playerPos.z + PLAYER_RADIUS / 2) / TILE_SIZE, 0.0f, float(map[0].size() - 1));

		vector<ivec2> mapPositions;
		mapPositions.reserve(4);

		mapPositions.emplace_back(minMapX, minMapZ);
		if (maxMapZ != minMapZ) mapPositions.emplace_back(minMapX, maxMapZ);

		if (maxMapX != minMapX) {
			mapPositions.emplace_back(maxMapX, minMapZ);
			if (maxMapZ != minMapZ) mapPositions.emplace_back(maxMapX, maxMapZ);
		}

		// printf("GG\n");

		for (const ivec2& mapPos : mapPositions) {
			if (map[mapPos.x][mapPos.y] == nullptr) continue;

			box2 cube(
				vec2(mapPos) * TILE_SIZE,
				vec2(mapPos + 1) * TILE_SIZE
			);

			vec2 newPos = vec2(playerPos.x, playerPos.z) + offset;
			vec2 off = getOffset(newPos, PLAYER_RADIUS, cube);
			offset += off;
			// printf("off = (%.4f, %.4f)\n", off.x, off.y);
		}

		return offset;
	}


	void Player::move(TickContext& context) {
		vec2 offset(
			left ? -1 : right ? 1 : 0,
			up ? -1 : down ? 1 : 0
		);

		if (length(offset) == 0) return;

		offset = normalize(offset) * speed * context.deltaTime;
		offset = moveWithCollisions(pos, offset, context.map);

		// printf("player: (%.4f, %.4f, %.4f)\n", pos.x, pos.y, pos.z);
		// printf("offset: (%.4f, %.4f, %.4f)\n", offset.x, 0.0f, offset.y);

		vec3 offset3d(offset.x, 0.0f, offset.y);
		pos += offset3d;

		vec3 oldPos = pos;

		const float PAD = PLAYER_RADIUS / 2;
		const float endX = context.map.size() * TILE_SIZE;
		const float endZ = context.map[0].size() * TILE_SIZE;

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
			float delta = radians(ROTATE_SPEED) * context.deltaTime;

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