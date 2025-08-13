#include "tick_context.h"
#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include <algorithm>

namespace hack_game {
	using std::shared_ptr;
	using std::clamp;
	using std::move;
	using std::find;
	using std::make_move_iterator;
	using std::dynamic_pointer_cast;

	using glm::uvec2;
	using glm::vec2;

	TickContext::TickContext(Map& map, const shared_ptr<Player>& player, const shared_ptr<Enemy>& enemy) noexcept:
				map(map), player(player), enemy(enemy), entities{player, enemy} {}


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
				auto it = find(entities.begin(), entities.end(), entity);
				if (it != entities.end())
					entities.erase(it);
			}

			removedEntities.clear();
		}

		if (!addedEntities.empty()) {
			entities.reserve(entities.size() + addedEntities.size());

			entities.insert(
				entities.end(),
				make_move_iterator(addedEntities.begin()),
				make_move_iterator(addedEntities.end())
			);
			
			addedEntities.clear();
		}
	}
}