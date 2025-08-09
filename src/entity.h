#ifndef HACK_GAME_ENTITY_H
#define HACK_GAME_ENTITY_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <vector>
#include <memory>

namespace hack_game {

	const float TILE_SIZE = 0.05f;
	const float BLOCK_SIZE = 0.04f;

	class Entity;
	class Block;

	using map_t = std::vector<std::vector<Block*>>;

	struct TickContext {
		float deltaTime = 0;
		map_t& map;

	private:
		std::vector<std::shared_ptr<Entity>> entities;
		std::vector<std::shared_ptr<Entity>> addedEntities;
		std::vector<std::shared_ptr<Entity>> removedEntities;

	public:
		TickContext(map_t& map): map(map) {}

		float maxX() const {
			return map.size() - 1;
		}

		float maxY() const {
			return map[0].size() - 1;
		}

		static glm::vec4 getBlockHitbox(uint32_t x, uint32_t y);


		const std::vector<std::shared_ptr<Entity>>& getEntities() const {
			return entities;
		}

		void addEntity(const std::shared_ptr<Entity>& entity) {
			addedEntities.push_back(entity);
		}

		void removeEntity(const std::shared_ptr<Entity>& entity) {
			removedEntities.push_back(entity);
		}

		void updateEntities();
	};

	struct DrawContext {
		const GLuint shaderProgram;
		const GLuint modelLocation;
		const GLuint modelColorLocation;
	};


	class Entity {
	protected:
		DrawContext& drawContext;

		Entity(DrawContext& drawContext):
				drawContext(drawContext) {}

	public:
		virtual ~Entity() {}

		virtual void tick(TickContext&) = 0;
		virtual void draw() const {
			glUseProgram(drawContext.shaderProgram);
		}
	};
}

#endif