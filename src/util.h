#ifndef HACK_GAME__UTIL_H
#define HACK_GAME__UTIL_H

#include <glm/vec3.hpp>
#include <glm/gtx/vector_angle.hpp>

namespace hack_game {

	class TickContext;


	/// Вектор нормали угла в 2d. От него сичитается угол
	constexpr glm::vec2 ANGLE_NORMAL{0.0f, 1.0f};

	/// Эпсилон для рассчётов коллизии
	constexpr float EPSILON = 1e-6f;


	constexpr glm::vec3 colorAsVec3(uint32_t color) {
		return glm::vec3(
			((color >> 16) & 0xFF) * (1.f / 255.f),
			((color >>  8) & 0xFF) * (1.f / 255.f),
			((color      ) & 0xFF) * (1.f / 255.f)
		);
	}

	constexpr bool isPointInsideSphere(const glm::vec3& point, const glm::vec3& center, float radius) {
		const glm::vec3 d = point - center;
		return d.x * d.x + d.y * d.y + d.z * d.z <= radius * radius;
	}

	/**
	 * @return Угол в 2d от ANGLE_NORMAL до вектора между pos и lookAt.
	 * Если pos и lookAt находятся в одной точке, то возвращает NaN
	 */
	float horizontalAngleBetween(const glm::vec3& pos, const glm::vec3& lookAt);


	/**
	 * Разрешает коллизию точки с блоками на карте
	 * @param context контекст для получения блока
	 * @param pos позиция точки на плоскости
	 * @param offset смещение точки
	 * @return Новое смещение
	 */
	glm::vec2 resolveBlockCollision(const TickContext& context, const glm::vec2& pos, glm::vec2 offset);
}

#endif