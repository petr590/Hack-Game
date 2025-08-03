#ifndef HACK_GAME_PLAYER_H
#define HACK_GAME_PLAYER_H

#include "entity.h"
#include "model.h"
#include "camera.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <boost/dynamic_bitset.hpp>

namespace hack_game {

	class Player: public Entity {
		DrawContext& bulletDrawContext;
		const Model& model;
		const float speed;
		Camera camera;

		glm::vec3 pos;
		bool up    = false;
		bool down  = false;
		bool left  = false;
		bool right = false;
		bool fire  = false;
		float angle = 0.0f;

		float timeSinceLastBullet = 0.0f;

	public:
		Player(DrawContext& drawContext, DrawContext& bulletDrawContext, const Model&, float speed, const Camera&);
		
		Camera& getCamera() {
			return camera;
		}
		
		void onKey(int key, int action);
		void tick(TickContext&) override;
		void draw() const override;

	private:
		void move(TickContext&);
	};
}

#endif