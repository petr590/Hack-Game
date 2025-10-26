#ifndef HACK_GAME__RENDER_H
#define HACK_GAME__RENDER_H

#include <ostream>
#include <memory>

namespace hack_game {

	class WindowData;
	class ShaderManager;
	class Menu;

	void render(const WindowData&, ShaderManager&, Menu&, const std::unique_ptr<std::ostream>& fpsFile, float deltaTime, float endGameTime);
}

#endif