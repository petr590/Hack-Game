#include "enemy_destroy.h"
#include "entity/player.h"
#include "model/models.h"
#include "context/draw_context.h"
#include "context/tick_context.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using glm::vec3;
	using glm::mat4;


	static GLint randomInt() {
		return  ((rand() >> 3) << 24) |
				((rand() >> 3) << 12) |
				(rand() >> 3);
	}


	static int randomBetween(int low, int high) {
		assert(low <= high);
		assert(high <= RAND_MAX);
		return low + rand() % (high - low + 1);
	}

	static float randomBetween(float low, float high) {
		return low + float(rand()) * (high - low) * (1.0f / RAND_MAX);
	}

	static vec3 randomBetween(const vec3& low, const vec3& high) {
		return vec3(
			randomBetween(low.x, high.x),
			randomBetween(low.y, high.y),
			randomBetween(low.z, high.z)
		);
	}

	static constexpr float zoom(float value, float srcStart, float srcEnd, float dstStart, float dstEnd) {
		return dstStart + (value - srcStart) * (dstEnd - dstStart) / (srcEnd - srcStart);
	}

	static_assert(zoom(1, 0, 2, 6, 8) == 7);
	static_assert(zoom(150, 100, 200, 0, 1) == 0.5f);
	


	static const float SIZE = 10;
	static const float DURATION = 1;

	static const float CUBES_START = 0.1f * DURATION;
	static const float CUBES_END   = 0.7f * DURATION;

	static const float MIN_SPAWN_SIZE = 4 * TILE_SIZE;
	static const float MAX_SPAWN_SIZE = 10 * TILE_SIZE;


	EnemyDestroyAnimation::EnemyDestroyAnimation(DrawContext& drawContext) noexcept:
			Animation(drawContext.nullShader, SIZE, DURATION),
			circleShader (drawContext.shaders["enemyDestroyCircle"]),
			squareShader (drawContext.shaders["enemyDestroySquare"]),
			cubeShader   (drawContext.shaders["enemyDestroyCube"]),
			seed(randomInt()),
			view(1.0f) {}
	

	void EnemyDestroyAnimation::tick(TickContext& context) {
		Animation::tick(context);
		view = context.player->getCamera().getView();

		if (time >= CUBES_START && time <= CUBES_END) {

			const int newCubes = static_cast<int>(randomBetween(1, 20) * randomBetween(1, 20) * context.deltaTime);

			for (int i = 0; i < newCubes; i++) {
				addCube();
			}
		}
		
		updateCubes(fadingCubes, context.deltaTime);
		updateCubes(solidCubes, context.deltaTime);
	}


	void EnemyDestroyAnimation::updateCubes(std::vector<Cube>& cubes, float deltaTime) {
		if (cubes.empty()) return;

		for (Cube& cube : cubes) {
			cube.lifetime += deltaTime;
		}

		std::erase_if(cubes, [&] (const Cube& cube) { return cube.lifetime >= cube.maxLifetime; });
	}


	void EnemyDestroyAnimation::addCube() {
		const float spawnSize = zoom(time, CUBES_START, CUBES_END, MIN_SPAWN_SIZE, MAX_SPAWN_SIZE);

		const vec3 cubePos = randomBetween(
				vec3(pos.x - spawnSize, pos.y, pos.z - spawnSize),
				pos + spawnSize
		);

		const float angle = randomBetween(0.0f, glm::radians(360.0f));
		const vec3 axis = glm::normalize(randomBetween(vec3(-1.0f), vec3(1.0f)));

		const float minScale = zoom(time, CUBES_START, CUBES_END, 0.25f, 0.05f);
		const float maxScale = zoom(time, CUBES_START, CUBES_END, 0.5f, 0.1f);
		const float scale = randomBetween(minScale, maxScale);
		const float lifetime = randomBetween(0.05f, 0.3f);

		std::vector<Cube>* cubes;

		switch ((rand() >> 8) & 0x1) {
			case 0: cubes = &fadingCubes; break;
			case 1: cubes = &solidCubes; break;
			default: return;
		}

		cubes->emplace_back(cubePos, angle, axis, lifetime, scale);
	}


	enum Mode: GLuint {
		MODE_FADING = 0,
		MODE_SOLID  = 1,
	};

	static mat4 getModelTransformWithoutRotation(const vec3& pos, float size) {
		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		return     glm::scale(modelMat, vec3(size));
	}

	
	void EnemyDestroyAnimation::draw() const {
		const float progress = time / duration;

		glUseProgram(squareShader.id);
		squareShader.setView(view);
		squareShader.setModel(getModelTransformWithoutRotation(pos, size));
		squareShader.setCenterPos(pos);
		squareShader.setProgress(progress);
		squareShader.setSeed(seed);
		model.draw(squareShader);
		

		glUseProgram(circleShader.id);
		circleShader.setView(view);
		circleShader.setModel(getModelTransform());
		circleShader.setCenterPos(pos);
		circleShader.setProgress(progress);
		model.draw(circleShader);


		if (time >= CUBES_START && (!fadingCubes.empty() || !solidCubes.empty())) {
			glUseProgram(cubeShader.id);
			cubeShader.setView(view);

			drawCubes(fadingCubes, MODE_FADING);
			drawCubes(solidCubes, MODE_SOLID);
		}
	}

	void EnemyDestroyAnimation::drawCubes(const std::vector<Cube>& cubes, GLuint mode) const {
		cubeShader.setMode(mode);
			
		for (const Cube& cube : cubes) {
			mat4 modelMat = glm::scale(cube.modelMat, vec3(cube.scale));
			
			cubeShader.setModel(modelMat);
			cubeShader.setProgress(cube.lifetime / cube.maxLifetime);

			models::blackCube.draw(cubeShader);
		}
	}


	static mat4 cubeModelTransform(const glm::vec3& pos, float angle, const glm::vec3& axis) {
		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		return     glm::rotate(modelMat, angle, axis);
	}

	EnemyDestroyAnimation::Cube::Cube(const glm::vec3& pos, float angle, const glm::vec3& axis, float maxLifetime, float scale):
			modelMat(cubeModelTransform(pos, angle, axis)),
			maxLifetime(maxLifetime),
			scale(scale) {}
}