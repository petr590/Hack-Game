#ifndef HACK_GAME__LEVEL__LEVEL_H
#define HACK_GAME__LEVEL__LEVEL_H

#include "gl_fwd.h"
#include <vector>
#include <map>
#include <memory>

#include <nlohmann/json_fwd.hpp>
#include <glm/vec2.hpp>

namespace hack_game {
	const float TILE_SIZE = 0.05f;

	class ShaderManager;
	class Entity;
	class Block;
	class Player;
	class Enemy;
	class Damageable;


	class Map {
		using ptr_t = std::shared_ptr<Block>;
		std::vector<std::vector<ptr_t>> data;

	public:
		Map() noexcept = default;
		Map(size_t width, size_t height);

		void allocate(size_t width, size_t height);
		

		ptr_t& operator[](const glm::uvec2& p) {
			return data[p.x][p.y];
		}

		const ptr_t& operator[](const glm::uvec2& p) const {
			return data[p.x][p.y];
		}

		std::vector<ptr_t>& operator[](size_t x) {
			return data[x];
		}

		const std::vector<ptr_t>& operator[](size_t x) const {
			return data[x];
		}


		size_t width() const noexcept {
			return data.size();
		}

		size_t height() const noexcept {
			return data[0].size();
		}
	};

	class Level {
	public:
		Map map;

		using EntityVector = std::vector<std::shared_ptr<Entity>>;
		using EntityMap = std::map<GLuint, EntityVector>;

	private:
		std::shared_ptr<Player> player;
		std::shared_ptr<Enemy> enemy;

		EntityMap opaqueEntityMap;
		EntityMap transparentEntityMap;

		EntityVector addedEntities;
		EntityVector removedEntities;

		std::vector<std::shared_ptr<Damageable>> damageableEnemyEntities;

		float deltaTime = 0;

		EntityVector& getVector(const std::shared_ptr<Entity>&) noexcept;
		void addEntityDirect(std::shared_ptr<Entity>&&);

		void readMap(ShaderManager& shaderManager, const std::string& path, const nlohmann::json&);
		void readEntities(ShaderManager& shaderManager, const std::string& path, const nlohmann::json&);

	public:
		Level(ShaderManager&, const std::string& path);

		float getDeltaTime() const noexcept {
			return deltaTime;
		}

		void setDeltaTime(float deltaTime) noexcept {
			this->deltaTime = deltaTime;
		}

		const std::shared_ptr<Player>& getPlayer() const noexcept {
			return player;
		}

		const std::shared_ptr<Enemy>& getEnemy() const noexcept {
			return enemy;
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
