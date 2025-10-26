#ifndef HACK_GAME__ENTITY__ANIMATION__MINION_DESTROY_H
#define HACK_GAME__ENTITY__ANIMATION__MINION_DESTROY_H

#include "billboard_animation.h"
#include <vector>

namespace hack_game {

	class MinionDestroyAnimation: public BillboardAnimation {
	public:
		class Cube;
	
	private:
		std::vector<std::shared_ptr<Cube>> cubes;
		Shader& billboardShader;
		Shader& flatShader;
		Shader& particleShader;
		glm::vec3 angleNormal;
		int32_t seed;

	public:
		MinionDestroyAnimation(std::shared_ptr<const EntityWithPos>&&, Level&, ShaderManager&) noexcept;
		~MinionDestroyAnimation() noexcept;

		void tick(Level&) override;
		void draw() const override;
	
	protected:
		void onRemove(Level&) override;
	};
}

#endif