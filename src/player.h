#ifndef HACK_GAME_PLAYER_H
#define HACK_GAME_PLAYER_H

#include "camera.h"
#include "model.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <boost/dynamic_bitset.hpp>

namespace hack_game {
	class Player {
		const Model& model;
		const float speed;
		Camera camera;

		glm::vec3 pos;
		bool up    = false;
		bool down  = false;
		bool left  = false;
		bool right = false;
		float angle = 0.0f;

	public:
		Player(const Model& model, float speed, const Camera& camera);
		
		Camera& getCamera() {
			return camera;
		}
		
		void onKey(int key, int action);
		void move(float deltaTime, const std::vector<boost::dynamic_bitset<>>& map);
		void draw(GLuint modelLocation, GLuint modelColorLocation);
	};
}

#endif