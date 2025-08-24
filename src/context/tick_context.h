#ifndef HACK_GAME_TICK_CONTEXT_H
#define HACK_GAME_TICK_CONTEXT_H

#include "../gl_fwd.h"
#include <vector>
#include <map>
#include <memory>
#include <glm/vec2.hpp>

namespace hack_game {
	const float TILE_SIZE = 0.05f;

	class Entity;
	class Block;
	class Player;
	class Enemy;
	class EnemyBullet;


	class Map {
		std::vector<std::vector<Block*>> data;

	public:
		Map(size_t width, size_t height) noexcept:
				data(width, std::vector<Block*>(height, nullptr)) {}
		

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
		Map map;
		const std::shared_ptr<Player> player;
		const std::shared_ptr<Enemy> enemy;

	private:
		using EntityVector = std::vector<std::shared_ptr<Entity>>;

		std::map<GLuint, EntityVector> entityMap;
		EntityVector addedEntities;
		EntityVector removedEntities;

		std::vector<std::shared_ptr<EnemyBullet>> breakableEnemyBullets;

	public:
		TickContext(Map&& map, const std::shared_ptr<Player>& player, const std::shared_ptr<Enemy>& enemy) noexcept;

		glm::uvec2 getMapPos(const glm::vec2& pos) const noexcept;


		const std::map<GLuint, EntityVector>& getEntityMap() const noexcept {
			return entityMap;
		}

		const std::vector<std::shared_ptr<EnemyBullet>>& getBreakableEnemyBullets() const noexcept {
			return breakableEnemyBullets;
		}

		void addEntity(const std::shared_ptr<Entity>&);
		void removeEntity(const std::shared_ptr<Entity>&);
		void updateEntities();
	};
}

#endif
