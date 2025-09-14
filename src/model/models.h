#ifndef HACK_GAME__MODEL__MODELS_H
#define HACK_GAME__MODEL__MODELS_H

#include "colored_model.h"

namespace hack_game {
	namespace models {

		extern ColoredModel
				plane,
				platform,

				blackCube,
				breakableCube,
				unbreakableCube,

				playerBase,
				playerCenter,
				playerLeft,
				playerRight,
				playerBullet,

				sphere,
				breakableSphere,
				unbreakableSphere,
				minion;

		extern Model& player3hp;
		extern Model& player2hp;
		extern Model& player1hp;
		
		extern Model& cubeFrame;
		extern Model& texturedPlane;
		extern Model& postprocessingModel;
	}
}

#endif