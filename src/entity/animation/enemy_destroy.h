#ifndef HACK_GAME__ENTITY_ANIMATION__ENEMY_DESTROY_H
#define HACK_GAME__ENTITY_ANIMATION__ENEMY_DESTROY_H

#include "billboard_animation.h"
#include <vector>

namespace hack_game {

	class EnemyDestroyAnimation: public BillboardAnimation {
	public:
		class Cube;

	private:
		Shader& billboardShader;
		Shader& flatShader;
		Shader& particleShader;
		const GLint seed;
		glm::mat4 view;

		std::vector<Cube> fadingCubes, solidCubes, frameCubes;

	public:
		EnemyDestroyAnimation(std::shared_ptr<const EntityWithPos>&&, DrawContext&) noexcept;
		~EnemyDestroyAnimation() noexcept;

		void tick(TickContext&) override;
		void draw() const override;
	
	protected:
		void onRemove() override;
	};
}

#endif