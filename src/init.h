#ifndef HACK_GAME__INIT_H
#define HACK_GAME__INIT_H

#include "level/level.h"

class GLFWwindow;

namespace hack_game {
	class ShaderManager;

	struct FramebufferInfo {
		GLuint msFramebuffer;
		GLuint msTexture;
		GLuint msRenderbuffer;

		GLuint framebuffer;
		GLuint texture;
	};


	class WindowData {
		GLFWwindow* window;
		FramebufferInfo fbInfo;
		int windowWidth;
		int windowHeight;
		int refreshRate;

		WindowData();
		~WindowData();

		WindowData(const WindowData&) = delete;

	public:
		static const WindowData& getInstance();

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
}

#endif