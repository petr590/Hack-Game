#include "util.h"

namespace hack_game {
	using glm::vec2;
	using glm::vec3;

	static_assert(isPointInsideSphere(vec3(0), vec3(0), 0));
	static_assert(isPointInsideSphere(vec3(0), vec3(1), 1.7320508f)); // sqrt(3)
	static_assert(!isPointInsideSphere(vec3(0), vec3(1), 1.7320507f));

	float horizontalAngleBetween(const vec3& pos, const vec3& lookAt) {
		vec2 dir(
			lookAt.x - pos.x,
			lookAt.z - pos.z
		);

		if (glm::length(dir) == 0) {
			return std::numeric_limits<float>::quiet_NaN();
		}

		return glm::orientedAngle(vec2(0.0f, 1.0f), glm::normalize(dir));
	}
}