#include "tick_context.h"
#include <algorithm>

namespace hack_game {
	using std::clamp;
	using std::find;
	using std::make_move_iterator;

	using glm::uvec2;
	using glm::vec2;

	uvec2 TickContext::getMapPos(const vec2& pos) const noexcept {
		return uvec2(
			clamp(pos.x * (1.0f / TILE_SIZE), 0.0f, float(map.width() - 1)),
			clamp(pos.y * (1.0f / TILE_SIZE), 0.0f, float(map.height() - 1))
		);
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