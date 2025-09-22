#include "main.h"
#include "shaders.h"
#include "context/draw_context.h"

#include <GLFW/glfw3.h>

namespace hack_game {
	static FramebufferInfo staticFbInfo;

	static void parse_args(int argc, const char* argv[]) {
		for (int i = 1; i < argc; i++) {
			std::string arg = argv[i];

			if (arg == "--lines") {
				glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			}
		}
	}
}


#define ANIMATION_SHADER(name, vertexShader, fragmentShader) { name, Shader(name, createAnimationShaderProgram(vertexShader, fragmentShader)) }

int main(int argc, const char* argv[]) {
	using namespace hack_game;
	using std::map;

	try {
		srand(time(nullptr));

		GLFWwindow* const window = initGLFW();
		FramebufferInfo fbInfo = initGL();
		staticFbInfo = fbInfo;

		parse_args(argc, argv);
		

		DrawContext drawContext {
			.nullShader               = Shader("null"),
			.mainShader               = Shader("main",  createShaderProgram("main.vert",  "main.frag")),
			.lightShader              = Shader("light", createShaderProgram("light.vert", "light.frag")),
			.shaders = {
				ANIMATION_SHADER("enemyDamage",           "animation.vert",          "enemy-damage.frag"),
				ANIMATION_SHADER("enemyDestroyBillboard", "textured-animation.vert", "enemy-destroy-billboard.frag"),
				ANIMATION_SHADER("enemyDestroyFlat",      "animation.vert",          "enemy-destroy-flat.frag"),
				ANIMATION_SHADER("enemyDestroyParticle",  "animation.vert",          "enemy-destroy-particle.frag"),
				ANIMATION_SHADER("minionDestroy",         "animation.vert",          "minion-destroy.frag"),
				ANIMATION_SHADER("playerDamage",          "animation.vert",          "player-damage.frag"),
			}
		};

		map<GLuint, Shader*> shaderById {
			{ -1,                              &drawContext.nullShader  },
			{ drawContext.mainShader.getId(),  &drawContext.mainShader  },
			{ drawContext.lightShader.getId(), &drawContext.lightShader },
		};

		for (auto& entry : drawContext.shaders) {
			shaderById[entry.second.getId()] = &entry.second;
		}


		initShaderUniforms(drawContext);


		const GLuint postprocessingShaderId = createShaderProgram("postprocessing.vert", "postprocessing.frag");

		glUseProgram(postprocessingShaderId);
		glUniform1i(glGetUniformLocation(postprocessingShaderId, "screenTexture"), 0);
		Shader postprocessingShader("postprocessing", postprocessingShaderId);

		onShadersLoaded();


		TickContext tickContext	= createTickContext(drawContext);
		mainLoop(window, fbInfo, postprocessingShader, tickContext, shaderById);

		glfwTerminate();
		return 0;
		
	} catch (...) {
		throw;
	}
}