#ifndef HACK_GAME_TICK_CONTEXT_H
#define HACK_GAME_TICK_CONTEXT_H

#include <memory>
#include <vector>
#include <glm/vec2.hpp>

namespace hack_game {
	const float TILE_SIZE = 0.05f;

	class Entity;
	class Block;


	class Map {
		std::vector<std::vector<Block*>> data;

	public:
		Map(size_t width, size_t height) noexcept:
				data(width, std::vector<Block*>(height)) {}
		

		Block*& operator[](const glm::uvec2& p) {
			return data[p.x][p.y];
		}

		Block* operator[](const glm::uvec2& p) const {
			return data[p.x][p.y];
		}

		std::vector<Block*>& operator[](size_t x) {
			return data[x];
		}

		const std::vector<Block*>& operator[](size_t x) const {
			return data[x];
		}


		size_t width() const noexcept {
			return data.size();
		}

		size_t height() const noexcept {
			return data[0].size();
		}
	};

	struct TickContext {
		float deltaTime = 0;
		Map& map;

	private:
		std::vector<std::shared_ptr<Entity>> entities;
		std::vector<std::shared_ptr<Entity>> addedEntities;
		std::vector<std::shared_ptr<Entity>> removedEntities;

	public:
		TickContext(Map& map) noexcept: map(map) {}

		glm::uvec2 getMapPos(const glm::vec2& pos) const noexcept;


		const std::vector<std::shared_ptr<Entity>>& getEntities() const noexcept {
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
}

#endif