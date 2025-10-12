#include "player.h"
#include "enemy.h"
#include "bullet.h"
#include "animation/player_damage.h"
#include "model/models.h"
#include "context/draw_context.h"
#include "context/tick_context.h"
#include "scancodes.h"
#include "globals.h"
#include "util.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "nowarn_imgui.h"

namespace hack_game {
	using std::max;
	using std::min;
	using std::clamp;
	using std::pow;
	using std::sqrt;
	using std::isnan;
	using std::make_shared;

	using glm::vec2;
	using glm::vec3;
	using glm::mat4;
	using glm::quat;

	static const float PAD = 0.015f;
	static const float ROTATE_SPEED = 360 * 3;
	static const float BULLET_SPEED = 15 * TILE_SIZE;
	static const float BULLET_PERIOD = 0.1f;

	static const float DARK_DURATION = 0.5f;
	static const float FADE_DURATION = 0.3f;


	// ------------------------------------------ Player ------------------------------------------

	Player::Player(DrawContext& drawContext, const Camera& camera, float speed):
			Damageable(Side::PLAYER, 3),
			drawContext(drawContext),
			camera(camera),
			speed(speed),
			pos(TILE_SIZE * 8, 0.0f, TILE_SIZE * 8) {
		
		this->camera.move(pos);
	}

	GLuint Player::getShaderProgram() const noexcept {
		return drawContext.mainShader.getId();
	}

	std::shared_ptr<const Player> Player::shared_from_this() const {
		return std::dynamic_pointer_cast<const Player>(std::enable_shared_from_this<Entity>::shared_from_this());
	}

	void Player::updateAngle(float newTargetAngle) {
		targetAngle = newTargetAngle;

		if (angle > targetAngle) {
			if (angle - targetAngle > glm::radians(180.f)) {
				angle -= glm::radians(360.f);
			}

		} else {
			if (targetAngle - angle > glm::radians(180.f)) {
				angle += glm::radians(360.f);
			}
		}
	}

	void Player::updateKeys() {
		up    = ImGui::IsKeyDown(ImGuiKey_W);
		left  = ImGui::IsKeyDown(ImGuiKey_A);
		down  = ImGui::IsKeyDown(ImGuiKey_S);
		right = ImGui::IsKeyDown(ImGuiKey_D);
		fire  = ImGui::IsKeyDown(ImGuiKey_LeftShift);

		if (ImGui::IsKeyPressed(ImGuiKey_UpArrow, false))    updateAngle(glm::radians(0.f));
		if (ImGui::IsKeyPressed(ImGuiKey_LeftArrow, false))  updateAngle(glm::radians(90.f));
		if (ImGui::IsKeyPressed(ImGuiKey_DownArrow, false))  updateAngle(glm::radians(180.f));
		if (ImGui::IsKeyPressed(ImGuiKey_RightArrow, false)) updateAngle(glm::radians(270.f));
	}


	// -------------------------------------- collisions ---------------------------------------

	static constexpr float INF = std::numeric_limits<float>::infinity();
	static constexpr float NaN = std::numeric_limits<float>::quiet_NaN();


	/**
	 * Ищет точку пересечения отрезка и окружности. Если таких две, возвращает любую из них.
	 * @param[in] pos Первая точка отрезка
	 * @param[in] offset Смещение от первой точки отрезка до второй
	 * @param[in] center Центр окружности
	 * @param[in] center Радиус окружности
	 * @return Точку пересечения отрезка и окружности. Если её нет, возвращает `vec2(NaN)`.
	 *         Точка может быть за пределами отрезка максимум на @ref EPSILON по обоим осям x и y.
	 */
	static constexpr vec2 getIntersectPoint(const vec2& pos, const vec2& offset, const vec2& center, float radius) {
		const float cx = center.x;
		const float cy = center.y;

		if (offset.x == 0) {
			const float A = 1;
			const float B = -2 * cy;
			const float C = pow(pos.x - cx, 2) + cy*cy - radius*radius;

			const float D = B*B - 4*A*C;
			if (D < 0) {
				return vec2(NaN);
			}

			const float sqrtD = sqrt(D);

			const float y1 = (-B + sqrtD) / (2*A);
			const float y2 = (-B - sqrtD) / (2*A);

			const vec2 newPos = pos + offset;

			if (y1 >= min(pos.y, newPos.y) && y1 <= max(pos.y, newPos.y)) {
				return vec2(pos.x, y1);
			}

			if (y2 >= min(pos.y, newPos.y) && y2 <= max(pos.y, newPos.y)) {
				return vec2(pos.x, y2);
			}

			return vec2(NaN);
		}

		const float a = offset.x == 0 ? INF : offset.y / offset.x;
		const float b = pos.y - a * pos.x;

		const float A = a*a + 1;
		const float B = 2 * (a * (b - cy) - cx);
		const float C = pow(b - cy, 2) + cx*cx - radius*radius;

		const float D = B*B - 4*A*C;
		if (D < 0) {
			return vec2(NaN);
		}

		const float sqrtD = sqrt(D);

		const float x1 = (-B + sqrtD) / (2*A);
		const float x2 = (-B - sqrtD) / (2*A);

		const float y1 = a * x1 + b;
		const float y2 = a * x2 + b;

		const vec2 newPos = pos + offset;
		const float minX = min(pos.x, newPos.x) - EPSILON;
		const float minY = min(pos.y, newPos.y) - EPSILON;
		const float maxX = max(pos.x, newPos.x) + EPSILON;
		const float maxY = max(pos.y, newPos.y) + EPSILON;

		if (x1 >= minX && x1 <= maxX &&
			y1 >= minY && y1 <= maxY) {
			return vec2(x1, y1);
		}

		if (x1 != x2 &&
			x2 >= minX && x2 <= maxX &&
			y2 >= minY && y2 <= maxY) {
			return vec2(x2, y2);
		}

		return vec2(NaN);
	}

	static constexpr bool all_isnan(const vec2& v) {
		return isnan(v.x) && isnan(v.y);
	}

	static_assert(getIntersectPoint(vec2(1, 1), vec2(0, -2), vec2(0, 0), 1) == vec2(1, 0));
	static_assert(all_isnan(getIntersectPoint(vec2(2, 1), vec2(0, -2), vec2(0, 0), 1)));

	static_assert(getIntersectPoint(vec2(0, 0), vec2(1, 1), vec2(0, 0), 1) == vec2(sqrt(0.5f), sqrt(0.5f)));
	static_assert(all_isnan(getIntersectPoint(vec2(1, 1), vec2(2, 2), vec2(0, 0), 1)));


	static constexpr vec2 vec3ToVec2(const vec3& pos) {
		return vec2(pos.x, pos.z);
	}

	static vec2 resolveEnemyCollision(const TickContext& context, const vec2& pos, vec2 offset) {
		if (context.enemy->destroyed()) {
			return offset;
		}

		const vec2 newPos = pos + offset;

		const vec2 enemyPos = vec3ToVec2(context.enemy->getPos());
		const vec2 diff = newPos - enemyPos;

		const float DIST = Enemy::RADIUS + PAD;

		if (glm::dot(diff, diff) <= DIST * DIST) {
			vec2 point = getIntersectPoint(pos, offset, enemyPos, DIST + EPSILON);

			if (!isnan(point.x) && !isnan(point.y)) {
				offset = point - pos;
			}
		}

		return offset;
	}


	static vec2 moveWithCollisions(const TickContext& context, const vec2& pos, vec2 offset) {
		offset = resolveBlockCollision(context, pos, offset);
		return resolveEnemyCollision(context, pos, offset);
	}


	void Player::move(TickContext& context) {
		vec2 offset(
			left ? -1 : right ? 1 : 0,
			up ? -1 : down ? 1 : 0
		);

		if (offset.x == 0 && offset.y == 0) return;

		offset = glm::normalize(offset) * (speed * context.getDeltaTime());
		offset = moveWithCollisions(context, vec2(pos.x, pos.z), offset);

		vec3 offset3d(offset.x, 0.0f, offset.y);
		pos += offset3d;

		vec3 oldPos = pos;

		const float endX = context.map.width() * TILE_SIZE;
		const float endZ = context.map.height() * TILE_SIZE;

		pos = glm::clamp(pos,
				vec3(       PAD, -INFINITY,        PAD),
				vec3(endX - PAD,  INFINITY, endZ - PAD)
		);

		offset3d += pos - oldPos;
		camera.move(offset3d);
	}


	void Player::tick(TickContext& context) {
		move(context);

		if (angle != targetAngle) {
			float delta = glm::radians(ROTATE_SPEED) * context.getDeltaTime();

			if (angle > targetAngle) {
				angle = max(angle - delta, targetAngle);
			} else {
				angle = min(angle + delta, targetAngle);
			}
		}

		timeSinceLastBullet += context.getDeltaTime();

		if (fire && timeSinceLastBullet >= BULLET_PERIOD) {
			timeSinceLastBullet = fmodf(timeSinceLastBullet, BULLET_PERIOD);

			quat rotateQuat = glm::angleAxis(angle, vec3(0.0f, 1.0f, 0.0f));
			vec3 velocity = rotateQuat * vec3(0.0f, 0.0f, -1.0f) * BULLET_SPEED;
			vec3 bulletPos = pos + velocity * (TILE_SIZE * 0.5f);

			context.addEntity(make_shared<PlayerBullet>(
				drawContext.getShader("light"), angle, velocity, bulletPos
			));
		}
	}


	bool Player::hasCollision(const vec3& point) const {
		return isPointInsideSphere(point, pos, Enemy::RADIUS);
	}


	// ------------------------------------------- draw -------------------------------------------

	void Player::draw() const {
		mat4 modelMat(1.0f);
		modelMat = translate(modelMat, pos);
		modelMat = rotate(modelMat, angle, vec3(0.0f, 1.0f, 0.0f));

		Shader& mainShader = drawContext.mainShader;

		mainShader.setModel(modelMat);

		bool isDark = animation != nullptr && !animation->isFinished() && animation->getTime() <= DARK_DURATION + FADE_DURATION;

		if (isDark) {
			mainShader.setUniform("modelBrightness", clamp(invLerp(animation->getTime(), DARK_DURATION, DARK_DURATION + FADE_DURATION), 0.0f, 1.0f));
		}

		if (hitpoints >= 3) models::player3hp.draw(mainShader);
		if (hitpoints == 2) models::player2hp.draw(mainShader);
		if (hitpoints <= 1) models::player1hp.draw(mainShader);

		if (isDark) {
			mainShader.setUniform("modelBrightness", 1.0f);
		}
	}


	// ------------------------------------------ damage ------------------------------------------

	void Player::damage(TickContext& context, hp_t damage) {
		Damageable::damage(context, damage);

		if (!destroyed() && (animation == nullptr || animation->isFinished())) {
			animation = make_shared<PlayerDamageAnimation>(std::move(shared_from_this()), drawContext);
			context.addEntity(animation);
		}
	}


	void Player::onDestroy(TickContext&) {
		playerDestroyed = true;
	}
}