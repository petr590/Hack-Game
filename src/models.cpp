#include "models.h"

namespace hack_game {
	static const GLuint PLATFORM_COLOR = 0xB5AD8D;
	static const GLuint CUBE_COLOR = 0xD2D2D2;
	static const GLuint PLAYER_COLOR = 0xE7E7E7;
	static const GLuint PLAYER_BULLET_COLOR = 0xFFFFFF;

	Model platformModel(PLATFORM_COLOR, "models/platform.obj");
	Model cubeModel(CUBE_COLOR, "models/cube.obj");
	Model playerModel(PLAYER_COLOR, "models/player.obj");
	Model playerBulletModel(PLAYER_BULLET_COLOR, "models/player-bullet.obj");

	const std::vector<Model*> models {
		&platformModel,
		&cubeModel,
		&playerModel,
		&playerBulletModel,
	};
}