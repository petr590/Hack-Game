#include "enemy_destroy.h"
#include "entity/player.h"
#include "model/models.h"
#include "context/draw_context.h"
#include "context/tick_context.h"
#include "globals.h"
#include "util.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/gtc/matrix_transform.hpp>

namespace hack_game {
	using std::vector;

	using glm::vec3;
	using glm::mat4;

	using Cube = EnemyDestroyAnimation::Cube;


	// -------------------------------------- util functions --------------------------------------

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
	

	// ---------------------------------------- constants -----------------------------------------

	static const float SIZE = 10;
	static const float DURATION = 1;

	static const float CUBES_START = 0.1f * DURATION;
	static const float CUBES_END   = 0.7f * DURATION;

	static const float MIN_SPAWN_SIZE = 4 * TILE_SIZE;
	static const float MAX_SPAWN_SIZE = 10 * TILE_SIZE;


	// ------------------------------------------- Cube -------------------------------------------

	static mat4 cubeModelTransform(const vec3& pos, float angle, const vec3& axis) {
		mat4 modelMat(1.0f);
		modelMat = glm::translate(modelMat, pos);
		return     glm::rotate(modelMat, angle, axis);
	}


	class EnemyDestroyAnimation::Cube {
	public:
		mat4 modelMat;
		float maxLifetime;
		float lifetime = 0;
		float scale;

		Cube(const vec3& pos, float angle, const vec3& axis, float maxLifetime, float scale):
			modelMat(cubeModelTransform(pos, angle, axis)),
			maxLifetime(maxLifetime),
			scale(scale) {}
		
		Cube(Cube&&) = default;
		Cube& operator=(Cube&&) = default;
	};


	// ---------------------------------- EnemyDestroyAnimation -----------------------------------

	EnemyDestroyAnimation::EnemyDestroyAnimation(std::shared_ptr<const EntityWithPos>&& entity, DrawContext& drawContext) noexcept:
			BillboardAnimation(std::move(entity), drawContext.nullShader, models::texturedPlane, SIZE, DURATION),
			billboardShader (drawContext.shaders.at("enemyDestroyBillboard")),
			flatShader      (drawContext.shaders.at("enemyDestroyFlat")),
			particleShader  (drawContext.shaders.at("enemyDestroyParticle")),
			seed(randomInt()),
			view(1.0f) {
		
		destroyAnimationCount += 1;
	}

	EnemyDestroyAnimation::~EnemyDestroyAnimation() noexcept {}


	void EnemyDestroyAnimation::onRemove() {
		destroyAnimationCount -= 1;
	}


	// ------------------------------------------- tick -------------------------------------------

	static void updateCubes(vector<Cube>& cubes, float deltaTime) {
		if (cubes.empty()) return;

		for (Cube& cube : cubes) {
			cube.lifetime += deltaTime;
		}

		std::erase_if(cubes, [&] (const Cube& cube) { return cube.lifetime >= cube.maxLifetime; });
	}


	static void addCube(float time, const vec3& pos, vector<Cube>& fadingCubes, vector<Cube>& solidCubes, vector<Cube>& frameCubes) {
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

		float lifetime;
		vector<Cube>* cubes;

		switch ((rand() >> 8) & 0x7) {
			case 0: case 1: case 2: case 3: case 4:
				lifetime = randomBetween(0.05f, 0.3f);
				cubes = &fadingCubes;
				break;
			
			case 5: case 6:
				lifetime = randomBetween(0.05f, 0.2f);
				cubes = &solidCubes;
				break;
			
			case 7:
				lifetime = randomBetween(0.05f, 0.1f);
				cubes = &frameCubes;
				break;
			
			default:
				return;
		}

		cubes->emplace_back(cubePos, angle, axis, lifetime, scale);
	}
	

	void EnemyDestroyAnimation::tick(TickContext& context) {
		BillboardAnimation::tick(context);
		view = context.player->getCamera().getView();

		if (time >= CUBES_START && time <= CUBES_END) {

			const int newCubes = static_cast<int>(randomBetween(1, 20) * randomBetween(1, 20) * context.deltaTime);

			for (int i = 0; i < newCubes; i++) {
				addCube(time, pos, fadingCubes, solidCubes, frameCubes);
			}
		}
		
		updateCubes(fadingCubes, context.deltaTime);
		updateCubes(solidCubes, context.deltaTime);
		updateCubes(frameCubes, context.deltaTime);
	}


	// ------------------------------------------- draw -------------------------------------------

	enum Mode: GLuint {
		MODE_FADING = 0,
		MODE_SOLID  = 1,
	};


	static void drawCubes(Shader& particleShader, const vector<Cube>& cubes, GLuint mode, Model& model, float minScale) {
		particleShader.setMode(mode);
			
		for (const Cube& cube : cubes) {
			float progress = cube.lifetime / cube.maxLifetime;
			mat4 modelMat = glm::scale(cube.modelMat, vec3(cube.scale * std::lerp(1.0f, minScale, progress)));
			
			particleShader.setModel(modelMat);
			particleShader.setProgress(progress);

			model.draw(particleShader);
		}
	}
	

	void EnemyDestroyAnimation::draw() const {
		const float progress = time / duration;

		glUseProgram(flatShader.getId());
		flatShader.setView(view);
		flatShader.setModel(Animation::getModelTransform());
		flatShader.setCenterPos(pos);
		flatShader.setProgress(progress);
		flatShader.setSeed(seed);
		model.draw(flatShader);
		

		glUseProgram(billboardShader.getId());
		billboardShader.setView(view);
		billboardShader.setModel(getModelTransform());
		billboardShader.setCenterPos(pos);
		billboardShader.setProgress(progress);
		model.draw(billboardShader);


		if (time >= CUBES_START && (!fadingCubes.empty() || !solidCubes.empty() || !frameCubes.empty())) {
			glUseProgram(particleShader.getId());
			particleShader.setView(view);

			drawCubes(particleShader, fadingCubes, MODE_FADING, models::blackCube, 1.0f);
			drawCubes(particleShader, solidCubes,  MODE_SOLID,  models::blackCube, 0.9f);
			drawCubes(particleShader, frameCubes,  MODE_SOLID,  models::cubeFrame, 0.8f);
		}
	}
}