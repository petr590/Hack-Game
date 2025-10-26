#ifndef HACK_GAME__LEVEL__INVALID_LEVEL_EXCEPTION_H
#define HACK_GAME__LEVEL__INVALID_LEVEL_EXCEPTION_H

#include <stdexcept>

namespace hack_game {

	class InvalidLevelException: public std::exception {
		const std::string message;
		
	public:
		InvalidLevelException(std::string message) noexcept: message(message) {}
		
		const char* what() const noexcept {
			return message.c_str();
		}
	};
}

#endif