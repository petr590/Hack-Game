#ifndef HACK_GAME__INIT_H
#define HACK_GAME__INIT_H

#include "context/tick_context.h"

class GLFWwindow;

namespace hack_game {
	class DrawContext;

	struct FramebufferInfo {
		GLFWwindow* window;

		GLuint msFramebuffer;
		GLuint msTexture;
		GLuint msRenderbuffer;

		GLuint framebuffer;
		GLuint texture;
	};


	class Initializer {
		GLFWwindow* window;
		FramebufferInfo fbInfo;
		int windowWidth;
		int windowHeight;
		int refreshRate;

		Initializer();
		~Initializer();

	public:
		static const Initializer& getInstance();

		GLFWwindow* getWindow() const noexcept {
			return window;
		}

		const FramebufferInfo& getFbInfo() const noexcept {
			return fbInfo;
		}

		int getWindowWidth() const noexcept {
			return windowWidth;
		}

		int getWindowHeight() const noexcept {
			return windowHeight;
		}

		int getRefreshRate() const noexcept {
			return refreshRate;
		}

		void setWindowWidth(int windowWidth) noexcept {
			this->windowWidth = windowWidth;
		}

		void setWindowHeight(int windowHeight) noexcept {
			this->windowHeight = windowHeight;
		}
	};


	TickContext createTickContext(DrawContext&);
}

#endif