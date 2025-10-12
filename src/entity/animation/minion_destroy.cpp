#include "minion_destroy.h"
#include "entity/player.h"
#include "entity/entity_with_pos.h"
#include "model/models.h"
#include "context/draw_context.h"
#include "context/tick_context.h"
#include "cube_particle_mode.h"
#include "util.h"

namespace hack_game {
	using std::sin;
	using std::cos;
	using std::shared_ptr;
	using std::make_shared;

	using glm::vec3;
	using glm::mat4;

	using Cube = MinionDestroyAnimation::Cube;


	static const float DURATION = 0.35f * 5; // DEBUG
	static const float SIZE     = 1.0f;
	static const float Y_OFFSET = 0.5f * TILE_SIZE;

	static const int MAX_CUBES = 10;

	static const float SKIP_CUBE_CHANCE = 0.15f;
	static const float FRAME_MODE_CHANCE = 0.15f;

	static const float MIN_CUBE_DISTANCE = TILE_SIZE * 0.5f;
	static const float MAX_CUBE_DISTANCE = TILE_SIZE;
	
	static const float MIN_CUBE_SPEED = TILE_SIZE;
	static const float MAX_CUBE_SPEED = TILE_SIZE * 3.0f;

	static constexpr float CUBE_ROTATE_AXIS_ANGLE = glm::radians(60.0f);
	static constexpr vec3 CUBE_ROTATE_AXIS_NORMAL (0.0f, sin(CUBE_ROTATE_AXIS_ANGLE), cos(CUBE_ROTATE_AXIS_ANGLE));


	class MinionDestroyAnimation::Cube: public Entity {
	public:
		MinionDestroyAnimation& parent;
		vec3 offset;
		vec3 speed;
		float scale;
		bool isFrame;

		Cube(MinionDestroyAnimation& parent, const vec3& offset, float speed, float scale, bool isFrame) noexcept:
				parent(parent), offset(offset), speed(glm::normalize(offset) * speed), scale(scale), isFrame(isFrame) {}
		
		mat4 getModelTransform() const {
			mat4 modelMat(1.0f);
			modelMat = glm::translate(modelMat, parent.entity->getPos() + offset);

			const float angle = parent.time * (glm::radians(360.0f) / DURATION);
			modelMat = glm::rotate(modelMat, angle, CUBE_ROTATE_AXIS_NORMAL);
			modelMat = glm::rotate(modelMat, CUBE_ROTATE_AXIS_ANGLE, vec3(-1.0f, 0.0f, 0.0f));
			modelMat = glm::scale(modelMat, vec3(scale));
			return     glm::translate(modelMat, vec3(0.0f, -0.02f, 0.0f));
		}

		GLuint getShaderProgram() const noexcept override {
			return parent.particleShader.getId();
		}

		void tick(TickContext& context) override {
			offset += speed * context.getDeltaTime();
		}

		void draw() const override {
			Shader& shader = parent.particleShader;
			shader.setModel(getModelTransform());
			shader.setUniform("mode", static_cast<GLuint>(Mode::SOLID));

			const Model& model = isFrame ? models::cubeFrame : models::blackCube;
			model.draw(shader);
		}
	};


	MinionDestroyAnimation::MinionDestroyAnimation(shared_ptr<const EntityWithPos>&& entity, TickContext& tickContext, DrawContext& drawContext) noexcept:
			BillboardAnimation(std::move(entity), drawContext.nullShader, DURATION, SIZE, Y_OFFSET),
			billboardShader (drawContext.getShader("minionDestroyBillboard")),
			flatShader      (drawContext.getShader("minionDestroyFlat")),
			particleShader  (drawContext.getShader("particleCube")),
			seed            (randomInt32()) {

		cubes.reserve(MAX_CUBES);

		for (int i = 0; i < MAX_CUBES; i++) {
			if (randomBetween(0.0f, 1.0f) < SKIP_CUBE_CHANCE) {
				continue;
			}

			const float angle    = i * (glm::radians(360.0f) / MAX_CUBES);
			const float distance = randomBetween(MIN_CUBE_DISTANCE, MAX_CUBE_DISTANCE);
			const float speed    = randomBetween(MIN_CUBE_SPEED, MAX_CUBE_SPEED);
			const float scale    = randomBetween(0.25f, 0.5f);
			const bool isFrame   = randomBetween(0.0f, 1.0f) < FRAME_MODE_CHANCE;

			const vec3 offset = vec3(0.0f, TILE_SIZE, 0.0f) + glm::rotate(vec3(distance, 0.0f, 0.0f), angle, vec3(0.0f, 1.0f, 0.0f));

			auto cube = make_shared<Cube>(*this, offset, speed, scale, isFrame);
			tickContext.addEntity(cube);
			cubes.push_back(std::move(cube));
		}
	}

	MinionDestroyAnimation::~MinionDestroyAnimation() noexcept {}


	void MinionDestroyAnimation::tick(TickContext& context) {
		BillboardAnimation::tick(context);

		const Camera& camera = context.player->getCamera();
		angleNormal = glm::rotate(glm::normalize(camera.getPos() - camera.getTarget()), glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));
	}

	void MinionDestroyAnimation::onRemove(TickContext& context) {
		for (const shared_ptr<Cube>& cube : cubes) {
			context.removeEntity(cube);
		}
	}

	void MinionDestroyAnimation::draw() const {
		const float progress = time / duration;
		
		flatShader.use();
		flatShader.setView(getView());
		flatShader.setModel(glm::scale(Animation::getModelTransform(), vec3(0.3f)));
		flatShader.setUniform("centerPos", getPos());
		flatShader.setUniform("progress", progress);
		model.draw(flatShader);
		
		billboardShader.use();
		billboardShader.setView(getView());
		billboardShader.setModel(BillboardAnimation::getModelTransform());
		billboardShader.setUniform("centerPos", getPos());
		billboardShader.setUniform("angleNormal", angleNormal);
		billboardShader.setUniform("progress", progress);
		billboardShader.setUniform("seed", seed);
		model.draw(billboardShader);
	}
}