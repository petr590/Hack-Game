#include "models.h"

namespace hack_game {
	namespace models {
		SimpleModel plane    (0x000000, "models/plane.obj");
		SimpleModel platform (0xd0c8b5, "models/platform.obj");

		SimpleModel breakableCube   (0x41403B, "models/cube.obj");
		SimpleModel unbreakableCube (0xE0DAD1, breakableCube);
		SimpleModel lightCube       (0xFFFFFF, breakableCube);

		SimpleModel playerBase   (0xE0DAD1, "models/player/base.obj");
		SimpleModel playerCenter (0x41403B, "models/player/center.obj");
		SimpleModel playerLeft   (0xE0DAD1, "models/player/left.obj");
		SimpleModel playerRight  (0xE0DAD1, "models/player/right.obj");

		SimpleModel playerBullet (0xFFFFFF, "models/player-bullet.obj");

		SimpleModel sphere            (0x7A7876, "models/sphere.obj");
		SimpleModel breakableSphere   (0xF68118, sphere);
		SimpleModel unbreakableSphere (0x3E0065, sphere);

		SimpleModel shooter (0x7d746e, "models/shooter.obj");

		CompositeModel player3hp { &playerBase, &playerCenter, &playerLeft, &playerRight };
		CompositeModel player2hp { &playerBase, &playerCenter, &playerLeft };
		CompositeModel player1hp { &playerBase, &playerCenter };
	}
}
