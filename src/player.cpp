#include "player.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace hack_game {
	using std::max;
	using std::min;
	using std::clamp;
	using std::vector;
	using boost::dynamic_bitset;

	using glm::ivec2;
	using glm::vec2;
	using glm::vec4;
	using glm::vec3;
	using glm::mat4;
	using glm::radians;

	Player::Player(const Model& model, float speed, const Camera& camera):
				model(model), speed(speed), camera(camera), pos(0.0f, 0.0f, 0.0f) {}

	void Player::onKey(int key, int action) {
		if (action != GLFW_PRESS && action != GLFW_RELEASE) return;

		bool value = action == GLFW_PRESS;
		switch (key) {
			case GLFW_KEY_W: up = value; break;
			case GLFW_KEY_S: down = value; break;
			case GLFW_KEY_A: left = value; break;
			case GLFW_KEY_D: right = value; break;

			case GLFW_KEY_UP:    angle = radians(0.0f);   break;
			case GLFW_KEY_LEFT:  angle = radians(90.0f);  break;
			case GLFW_KEY_DOWN:  angle = radians(180.0f); break;
			case GLFW_KEY_RIGHT: angle = radians(270.0f); break;

			case GLFW_KEY_Z:
				printf("(%.2f, %.2f, %.2f)\n", camera.target.x, camera.target.y, camera.target.z);
				printf("(%.2f, %.2f, %.2f)\n", pos.x, pos.y, pos.z);
				camera.move(-pos);
				pos = vec3(0.0f);
				angle = 0.0f;
				break;
		}
	}


	const float TILE_SIZE = 0.05f;
	const float BLOCK_SIZE = 0.04f;
	const float PLAYER_SIZE = 0.03f;


	static int hasCollision(float playerStart, float playerEnd, float cubeStart, float cubeEnd) {
		int startIdx = playerStart < cubeStart ? -1 : playerStart > cubeEnd ? 1 : 0;
		int endIdx   = playerEnd   < cubeStart ? -1 : playerEnd   > cubeEnd ? 1 : 0;
		return startIdx == 0 || endIdx == 0 || startIdx != endIdx;
	}


	struct box2 {
		float sx, sy, ex, ey;

		box2(float sx, float sy, float ex, float ey):
			sx(sx), sy(sy), ex(ex), ey(ey) {}

		box2(const vec2& start, const vec2& end):
			sx(start.x), sy(start.y), ex(end.x), ey(end.y) {}
	};


	static vec3 moveWithCollisions(const vec3& playerPos, vec3 offset, const vector<dynamic_bitset<>>& map) {
		const int32_t minMapX = clamp((playerPos.x - PLAYER_SIZE / 2) / TILE_SIZE, 0.0f, float(map.size()));
		const int32_t minMapZ = clamp((playerPos.z - PLAYER_SIZE / 2) / TILE_SIZE, 0.0f, float(map[0].size()));
		const int32_t maxMapX = clamp((playerPos.x + PLAYER_SIZE / 2) / TILE_SIZE, 0.0f, float(map.size()));
		const int32_t maxMapZ = clamp((playerPos.z + PLAYER_SIZE / 2) / TILE_SIZE, 0.0f, float(map[0].size()));

		const ivec2 mapPositions[] = {
			ivec2(minMapX, minMapZ),
			ivec2(minMapX, maxMapZ),
			ivec2(maxMapX, minMapZ),
			ivec2(maxMapX, maxMapZ),
		};

		printf("(%.4f, %.4f)\n", offset.x, offset.z);

		for (const ivec2& mapPos : mapPositions) {
			if (!map[mapPos.x][mapPos.y]) continue;

			const vec3 target = playerPos + offset;

			const box2 playerBox(
				target.x - PLAYER_SIZE / 2,
				target.z - PLAYER_SIZE / 2,
				target.x + PLAYER_SIZE / 2,
				target.z + PLAYER_SIZE / 2
			);

			const box2 cubeBox(
				vec2(mapPos) * TILE_SIZE,
				vec2(mapPos + 1) * TILE_SIZE
			);

			bool xCollision = hasCollision(playerBox.sx, playerBox.ex, cubeBox.sx, cubeBox.ex);
			bool yCollision = hasCollision(playerBox.sy, playerBox.ey, cubeBox.sy, cubeBox.ey);

			printf("xCollision = %d, yCollision = %d, playerBox = (%.2f, %.2f, %.2f, %.2f), cubeBox = (%.2f, %.2f, %.2f, %.2f)\n",
					xCollision, yCollision,
					playerBox.sx, playerBox.sy, playerBox.ex, playerBox.ey,
					cubeBox.sx, cubeBox.sy, cubeBox.ex, cubeBox.ey);

			if (xCollision && yCollision) {
				box2 collisionBox(
					max(playerBox.sx, cubeBox.sx),
					max(playerBox.sy, cubeBox.sy),
					min(playerBox.ex, cubeBox.ex),
					min(playerBox.ey, cubeBox.ey)
				);

				float width = collisionBox.ex - collisionBox.sx;
				float height = collisionBox.ey - collisionBox.sy;

				if (offset.x == 0) width = 0;
				if (offset.z == 0) height = 0;

				if (offset.x != 0 && offset.z != 0) {
					width = min(width, height * offset.x / offset.z);
					height = min(height, width * offset.z / offset.x);
				}

				offset.x += offset.x > 0 ? -width : width;
				offset.z += offset.z > 0 ? -height : height;

				// if (fabs(offset.x) > fabs(offset.z)) {
				// 	offset.x += offset.x > 0 ? -width : width;
				// } else {
				// 	offset.z += offset.z > 0 ? -height : height;
				// }
			}
		}

		return offset;
	}


	void Player::move(float deltaTime, const vector<dynamic_bitset<>>& map) {
		vec3 offset(
			left ? -1 : right ? 1 : 0,
			0,
			up ? -1 : down ? 1 : 0
		);

		if (glm::length(offset) == 0) return;

		offset = glm::normalize(offset) * speed * deltaTime;
		offset = moveWithCollisions(pos, offset, map);
		pos += offset;
		camera.move(offset);
	}

	void Player::draw(GLuint modelLocation, GLuint modelColorLocation) {
		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		modelMat = glm::rotate(modelMat, angle, vec3(0.0f, 1.0f, 0.0f));
		modelMat = glm::scale(modelMat, vec3(0.1f));
		glUniformMatrix4fv(modelLocation, 1, GL_FALSE, value_ptr(modelMat));

		model.draw(modelColorLocation);
	}
}