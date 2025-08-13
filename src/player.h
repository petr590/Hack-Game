#ifndef HACK_GAME_PLAYER_H
#define HACK_GAME_PLAYER_H

#include "entity.h"
#include "model.h"
#include "camera.h"

namespace hack_game {

	class Player: public Entity {
		DrawContext& drawContext;
		DrawContext& bulletDrawContext;
		const float speed;
		Camera camera;

		glm::vec3 pos;
		bool up    = false;
		bool down  = false;
		bool left  = false;
		bool right = false;
		bool fire  = false;
		float angle = 0.0f;
		float targetAngle = 0.0f;
		float timeSinceLastBullet = 0.0f;
		int32_t hitpoints = 3;

	public:
		Player(DrawContext& drawContext, DrawContext& bulletDrawContext, float speed, const Camera&);

		const glm::vec3& getPos() const noexcept {
			return pos;
		}
		
		const Camera& getCamera() const noexcept {
			return camera;
		}
		
		void onKey(int key, int action);
		void tick(TickContext&) override;
		void draw() const override;
		void damage(TickContext&, int32_t damage);

	private:
		void updateAngle(float targetAngle);
		void move(TickContext&);
	};
}

#endif