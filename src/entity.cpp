#include "entity.h"

namespace hack_game {

	using std::find;
	using std::make_move_iterator;

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

	glm::vec4 TickContext::getBlockHitbox(uint32_t x, uint32_t y) {
		return glm::vec4(
			(x + 0.5f) * TILE_SIZE - BLOCK_SIZE / 2,
			(y + 0.5f) * TILE_SIZE - BLOCK_SIZE / 2,
			(x + 0.5f) * TILE_SIZE + BLOCK_SIZE / 2,
			(y + 0.5f) * TILE_SIZE + BLOCK_SIZE / 2
		);
	}
}