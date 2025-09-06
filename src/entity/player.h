#ifndef HACK_GAME__ENTITY__PLAYER_H
#define HACK_GAME__ENTITY__PLAYER_H

#include "damageable.h"
#include "camera.h"

namespace hack_game {

	class Player final: public Damageable {
		DrawContext& drawContext;
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

	public:
		Player(DrawContext& drawContext, float speed, const Camera&);

		const glm::vec3& getPos() const noexcept {
			return pos;
		}
		
		const Camera& getCamera() const noexcept {
			return camera;
		}

		GLuint getShaderProgram() const noexcept override;
		
		void onKey(int key, int action);
		void tick(TickContext&) override;
		void draw() const override;
		bool hasCollision(const glm::vec3& point) const override;
	
	protected:
		void onDestroy(TickContext&) override;

	private:
		void updateAngle(float targetAngle);
		void move(TickContext&);
	};
}

#endif
