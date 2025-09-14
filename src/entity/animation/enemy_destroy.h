#ifndef HACK_GAME__ENTITY_ANIMATION__ENEMY_DESTROY_H
#define HACK_GAME__ENTITY_ANIMATION__ENEMY_DESTROY_H

#include "animation.h"
#include <vector>

namespace hack_game {

	class EnemyDestroyAnimation: public Animation {
		Shader& circleShader;
		Shader& squareShader;
		Shader& cubeShader;
		const GLint seed;
		glm::mat4 view;

		class Cube {
		public:
			glm::mat4 modelMat;
			float maxLifetime;
			float lifetime = 0;
			float scale;

			Cube(const glm::vec3& pos, float angle, const glm::vec3& axis, float maxLifetime, float scale);
			Cube(Cube&&) = default;
			Cube& operator=(Cube&&) = default;
		};

		std::vector<Cube> fadingCubes, solidCubes, frameCubes;

	public:
		explicit EnemyDestroyAnimation(DrawContext&) noexcept;

		void tick(TickContext&) override;
		void draw() const override;
	
	protected:
		void onRemove() override;
	
	private:
		void updateCubes(std::vector<Cube>&, float);
		void addCube();
		void drawCubes(const std::vector<Cube>&, GLuint, Model&, float) const;
	};
}

#endif