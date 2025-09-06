#include "tick_context.h"
#include "entity/damageable.h"
#include "entity/player.h"
#include "entity/enemy.h"

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
		
		opaqueEntityMap[player->getShaderProgram()].push_back(player);
		opaqueEntityMap[enemy->getShaderProgram()].push_back(enemy);
		damageableEnemyEntities.push_back(enemy);
	}


	uvec2 TickContext::getMapPos(const vec2& pos) const noexcept {
		return uvec2(
			clamp(pos.x * (1.0f / TILE_SIZE), 0.0f, float(map.width() - 1)),
			clamp(pos.y * (1.0f / TILE_SIZE), 0.0f, float(map.height() - 1))
		);
	}


	void TickContext::addEntity(const shared_ptr<Entity>& entity) {
		addedEntities.push_back(entity);

		auto damageable = dynamic_pointer_cast<Damageable>(entity);

		if (damageable != nullptr && damageable->getSide() == Side::ENEMY && !damageable->invulnerable()) {
			damageableEnemyEntities.push_back(move(damageable));
		}
	}


	void TickContext::removeEntity(const shared_ptr<Entity>& entity) {
		removedEntities.push_back(entity);

		auto damageable = dynamic_pointer_cast<Damageable>(entity);

		if (damageable != nullptr && damageable->getSide() == Side::ENEMY) {
			auto it = find(damageableEnemyEntities.begin(), damageableEnemyEntities.end(), damageable);

			if (it != damageableEnemyEntities.end()) {
				damageableEnemyEntities.erase(it);
			}
		}
	}


	TickContext::EntityVector& TickContext::getVector(const shared_ptr<Entity>& entity) noexcept {
		EntityMap& map = entity->isTransparent() ? transparentEntityMap : opaqueEntityMap;
		return map[entity->getShaderProgram()];
	}

	void TickContext::updateEntities() {
		if (!removedEntities.empty()) {
			for (const auto& entity : removedEntities) {
				EntityVector& vector = getVector(entity);

				const auto it = find(vector.cbegin(), vector.cend(), entity);
				if (it != vector.cend()) {
					vector.erase(it);
				}
			}

			removedEntities.clear();
		}

		if (!addedEntities.empty()) {
			for (auto& entity : addedEntities) {
				getVector(entity).push_back(move(entity));
			}

			addedEntities.clear();
		}
	}
}
