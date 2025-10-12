#ifndef HACK_GAME__ENTITY__PLAYER_H
#define HACK_GAME__ENTITY__PLAYER_H

#include "damageable.h"
#include "entity_with_pos.h"
#include "camera.h"

namespace hack_game {

	class Animation;

	class Player final: public Damageable, public EntityWithPos {
		DrawContext& drawContext;
		Camera camera;
		const float speed;

		std::shared_ptr<Animation> animation = nullptr;
		glm::vec3 pos;

		float angle = 0.0f;
		float targetAngle = 0.0f;
		float timeSinceLastBullet = 0.0f;
		
		bool up    = false;
		bool down  = false;
		bool left  = false;
		bool right = false;
		bool fire  = false;

	public:
		Player(DrawContext&, const Camera&, float speed);

		const glm::vec3& getPos() const noexcept override {
			return pos;
		}
		
		const Camera& getCamera() const noexcept {
			return camera;
		}

		GLuint getShaderProgram() const noexcept override;
		std::shared_ptr<const Player> shared_from_this() const;
		
		void updateKeys();
		void tick(TickContext&) override;
		void draw() const override;
		bool hasCollision(const glm::vec3&) const override;
		void damage(TickContext&, hp_t) override;
			
	protected:
		void onDestroy(TickContext&) override;

	private:
		void updateAngle(float targetAngle);
		void move(TickContext&);
	};
}

#endif
