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
			.nullShader               = Shader(),
			.mainShader               = Shader(createShaderProgram("main.vert",  "main.frag")),
			.lightShader              = Shader(createShaderProgram("light.vert", "light.frag")),
			.shaders = {
				{ "enemyDamage",        Shader(createAnimationShaderProgram("animation.vert", "enemy-damage.frag"))         },
				{ "enemyDestroyCircle", Shader(createAnimationShaderProgram("textured-animation.vert", "enemy-destroy-circle.frag")) },
				{ "enemyDestroySquare", Shader(createAnimationShaderProgram("animation.vert", "enemy-destroy-square.frag")) },
				{ "enemyDestroyCube",   Shader(createAnimationShaderProgram("animation.vert", "enemy-destroy-cube.frag"))   },
				{ "minionDestroy",      Shader(createAnimationShaderProgram("animation.vert", "minion-destroy.frag"))       },
			}
		};

		map<GLuint, Shader*> shaderById {
			{ -1,                         &drawContext.nullShader  },
			{ drawContext.mainShader.id,  &drawContext.mainShader  },
			{ drawContext.lightShader.id, &drawContext.lightShader },
		};

		for (auto& entry : drawContext.shaders) {
			shaderById[entry.second.id] = &entry.second;
		}


		initShaderUniforms(drawContext);


		const GLuint postprocessingShaderId = createShaderProgram("postprocessing.vert", "postprocessing.frag");

		glUseProgram(postprocessingShaderId);
		glUniform1i(glGetUniformLocation(postprocessingShaderId, "screenTexture"), 0);
		Shader postprocessingShader(postprocessingShaderId);


		TickContext tickContext	= createTickContext(drawContext);
		mainLoop(window, fbInfo, postprocessingShader, tickContext, shaderById);

		glfwTerminate();
		return 0;
		
	} catch (...) {
		throw;
	}
}