#include "tick_context.h"
#include "../entity/player.h"
#include "../entity/enemy.h"
#include "../entity/bullet.h"

namespace hack_game {
	using std::shared_ptr;
	using std::clamp;
	using std::move;
	using std::find;
	using std::dynamic_pointer_cast;

	using glm::uvec2;
	using glm::vec2;

	TickContext::TickContext(Map&& map, const shared_ptr<Player>& player, const shared_ptr<Enemy>& enemy) noexcept:
				map(map), player(player), enemy(enemy) {
		
		entityMap[player->getShaderProgram()].push_back(player);
		entityMap[enemy->getShaderProgram()].push_back(enemy);
	}


	uvec2 TickContext::getMapPos(const vec2& pos) const noexcept {
		return uvec2(
			clamp(pos.x * (1.0f / TILE_SIZE), 0.0f, float(map.width() - 1)),
			clamp(pos.y * (1.0f / TILE_SIZE), 0.0f, float(map.height() - 1))
		);
	}


	void TickContext::addEntity(const shared_ptr<Entity>& entity) {
		addedEntities.push_back(entity);

		auto bullet = dynamic_pointer_cast<EnemyBullet>(entity);

		if (bullet != nullptr && !bullet->unbreakable) {
			breakableEnemyBullets.push_back(move(bullet));
		}
	}


	void TickContext::removeEntity(const shared_ptr<Entity>& entity) {
		removedEntities.push_back(entity);

		auto bullet = dynamic_pointer_cast<EnemyBullet>(entity);

		if (bullet != nullptr) {
			auto it = find(breakableEnemyBullets.begin(), breakableEnemyBullets.end(), bullet);

			if (it != breakableEnemyBullets.end()) {
				breakableEnemyBullets.erase(it);
			}
		}
	}


	void TickContext::updateEntities() {
		if (!removedEntities.empty()) {
			for (const auto& entity : removedEntities) {
				EntityVector& entityVec = entityMap[entity->getShaderProgram()];

				const auto it = find(entityVec.cbegin(), entityVec.cend(), entity);
				if (it != entityVec.cend()) {
					entityVec.erase(it);
				}
			}

			removedEntities.clear();
		}

		if (!addedEntities.empty()) {
			for (auto& entity : addedEntities) {
				entityMap[entity->getShaderProgram()].push_back(move(entity));
			}

			addedEntities.clear();
		}
	}
}
