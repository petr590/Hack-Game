#include "main.h"
#include "shader_loader.h"
#include "context/draw_context.h"

#include <GLFW/glfw3.h>

namespace hack_game {
	static void parse_args(int argc, const char* argv[]) {
		for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];

			if (arg == "--lines") {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		}
	}
}


#define ANIMATION_SHADER(name, vertexShader, fragmentShader) Shader(name, createAnimationShaderProgram(vertexShader, fragmentShader))

int main(int argc, const char* argv[]) {
	using namespace hack_game;
	using std::map;

	try {
		srand(time(nullptr));

		const Initializer& initializer = Initializer::getInstance();

		parse_args(argc, argv);
		

		DrawContext drawContext {
			initializer.getWindowWidth(),
			initializer.getWindowHeight(),
			Shader("null"),
			Shader("main",           createShaderProgram("main.vert",           "main.frag")),
			Shader("light",          createShaderProgram("light.vert",          "light.frag")),
			Shader("postprocessing", createShaderProgram("postprocessing.vert", "postprocessing.frag")),
			ANIMATION_SHADER("enemyDamage",            "animation.vert",          "enemy-damage.frag"),
			ANIMATION_SHADER("enemyDestroyBillboard",  "textured-animation.vert", "enemy-destroy-billboard.frag"),
			ANIMATION_SHADER("enemyDestroyFlat",       "animation.vert",          "enemy-destroy-flat.frag"),
			ANIMATION_SHADER("particleCube",           "animation.vert",          "particle-cube.frag"),
			ANIMATION_SHADER("minionDestroyBillboard", "animation.vert",          "minion-destroy-billboard.frag"),
			ANIMATION_SHADER("minionDestroyFlat",      "animation.vert",          "minion-destroy-flat.frag"),
			ANIMATION_SHADER("playerDamage",           "animation.vert",          "player-damage.frag"),
		};

		map<GLuint, Shader*> shaderById {
			{ -1,                              &drawContext.nullShader  },
			{ drawContext.mainShader.getId(),  &drawContext.mainShader  },
		};

		for (auto& entry : drawContext.getShaders()) {
			shaderById[entry.second.getId()] = &entry.second;
		}

		onShadersLoaded();

		TickContext tickContext	= createTickContext(drawContext);
		mainLoop(initializer, drawContext.getShader("postprocessing"), tickContext, shaderById);

		return 0;
		
	} catch (...) {
		throw;
	}
}