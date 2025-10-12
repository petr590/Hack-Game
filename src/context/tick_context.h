#ifndef HACK_GAME_TICK_CONTEXT_H
#define HACK_GAME_TICK_CONTEXT_H

#include "gl_fwd.h"
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
	class Damageable;


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
		Map map;
		const std::shared_ptr<Player> player;
		const std::shared_ptr<Enemy> enemy;

		using EntityVector = std::vector<std::shared_ptr<Entity>>;
		using EntityMap = std::map<GLuint, EntityVector>;

	private:
		EntityMap opaqueEntityMap;
		EntityMap transparentEntityMap;

		EntityVector addedEntities;
		EntityVector removedEntities;

		std::vector<std::shared_ptr<Damageable>> damageableEnemyEntities;

		float deltaTime = 0;

		EntityVector& getVector(const std::shared_ptr<Entity>&) noexcept;

	public:
		TickContext(Map&& map, const std::shared_ptr<Player>& player, const std::shared_ptr<Enemy>& enemy) noexcept;

		float getDeltaTime() const noexcept {
			return deltaTime;
		}

		void setDeltaTime(float deltaTime) noexcept {
			this->deltaTime = deltaTime;
		}

		glm::uvec2 getMapPos(const glm::vec2& pos) const noexcept;


		const EntityMap& getOpaqueEntityMap() const noexcept {
			return opaqueEntityMap;
		}

		const EntityMap& getTransparentEntityMap() const noexcept {
			return transparentEntityMap;
		}

		const std::vector<std::shared_ptr<Damageable>>& getDamageableEnemyEntities() const noexcept {
			return damageableEnemyEntities;
		}

		void addEntity(const std::shared_ptr<Entity>&);
		void removeEntity(const std::shared_ptr<Entity>&);
		void updateEntities();
	};
}

#endif
