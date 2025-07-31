#include "models.h"
#include "block.h"

namespace hack_game {
	static const GLuint PLATFORM_COLOR = 0xB5AD8D;
	static const GLuint CUBE_COLOR = 0xCCCCCC;
	static const GLuint MC_COLOR = 0xE7E7E7;

	static const GLfloat EDGE = Block::EDGE;


	Model platform(PLATFORM_COLOR, "models/platform.obj");
	Model cube(CUBE_COLOR, "models/cube.obj");
	Model mc(MC_COLOR, "models/mc.obj");
}