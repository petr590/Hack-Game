#ifndef HACK_GAME__MODEL__MODELS_H
#define HACK_GAME__MODEL__MODELS_H

#include "simple_model.h"
#include "composite_model.h"

namespace hack_game {
	namespace models {

		extern SimpleModel
				plane,
				platform,

				breakableCube,
				unbreakableCube,
				lightCube,

				playerBase,
				playerCenter,
				playerLeft,
				playerRight,

				playerBullet,

				sphere,
				breakableSphere,
				unbreakableSphere,

				shooter;
		

		extern CompositeModel
				player3hp,
				player2hp,
				player1hp;
	}
}

#endif