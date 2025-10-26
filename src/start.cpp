#include "main.h"
#include "shader/shader_loader.h"
#include "shader/shader_manager.h"

#include <GLFW/glfw3.h>

namespace hack_game {
	static bool profile = false;

	static void parse_args(int argc, const char* argv[]) {
		for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];

			if (arg == "--lines") {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			} else if (arg == "--profile") {
				profile = true;
			}
		}
	}
}


#define ANIMATION_SHADER(name, vertexShader, fragmentShader) Shader(name, createAnimationShaderProgram(vertexShader, fragmentShader))

int main(int argc, const char* argv[]) {
	using namespace hack_game;

	try {
		srand(time(nullptr));

		const WindowData& windowData = WindowData::getInstance();

		parse_args(argc, argv);
		

		ShaderManager shaderManager {
			windowData.getWindowWidth(),
			windowData.getWindowHeight(),
			Shader("null"),
			Shader("main",           createShaderProgram("main.vert",           "main.frag")),
			Shader("light",          createShaderProgram("light.vert",          "light.frag")),
			Shader("postprocessing", createShaderProgram("postprocessing.vert", "postprocessing.frag")),
			ANIMATION_SHADER("enemyDamage",            "animation.vert",          "enemy-damage.frag"),
			ANIMATION_SHADER("enemyDestroyBillboard",  "textured-animation.vert", "enemy-destroy-billboard.frag"),
			ANIMATION_SHADER("enemyDestroyFlat",       "animation.vert",          "enemy-destroy-flat.frag"),
			ANIMATION_SHADER("particleCube",           "animation.vert",          "particle-cube.frag"),
			ANIMATION_SHADER("minionDestroyBillboard", "textured-animation.vert", "minion-destroy-billboard.frag"),
			ANIMATION_SHADER("minionDestroyFlat",      "animation.vert",          "minion-destroy-flat.frag"),
			ANIMATION_SHADER("playerDamage",           "animation.vert",          "player-damage.frag"),
		};

		onShadersLoaded();
		mainLoop(windowData, shaderManager, profile);

		return 0;
		
	} catch (...) {
		throw;
	}
}