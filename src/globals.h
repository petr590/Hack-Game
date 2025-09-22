#ifndef HACK_GAME__GLOBALS_H
#define HACK_GAME__GLOBALS_H

namespace hack_game {
	
	extern bool enemyDestroyed, playerDestroyed;

	/// Количество анимаций уничтожения Player и Enemy.
	/// Пока оно больше 0, экран конца игрыне показывается
	extern int destroyAnimationCount;
}

#endif