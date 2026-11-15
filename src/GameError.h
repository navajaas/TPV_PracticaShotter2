#ifndef GAME_ERROR_H
#define GAME_ERROR_H

#include <stdexcept>
#include <SDL3/SDL.h>
#include <string>

class GameError : public std::logic_error
{
public:
	GameError(const std::string& message) : std::logic_error(message) {}
	char const* what() const noexcept override {
		return std::logic_error::what();
	}
};

#endif // GAME_ERROR_H

#ifndef SDL_ERROR_H
#define SDL_ERROR_H

class SDLError : public GameError
{
public:
	SDLError() : GameError(SDL_GetError()) {}

};
#endif // SDL_ERROR_H

#ifndef FILE_NOT_FOUND_ERROR_H
#define FILE_NOT_FOUND_ERROR_H

class FileNotFoundError : public GameError
{
private:
	std::string message;
public:
	FileNotFoundError(const std::string& filename)
		: GameError("Archivo no encontrado: " + filename)
	{}

};
#endif // FILE_NOT_FOUND_ERROR_H

#ifndef FILE_FORMAT_ERROR_H
#define FILE_FORMAT_ERROR_H

class FileFormatError : public GameError
{
private:
	std::string message;
public:
	FileFormatError(const std::string& filename, int line,
		const std::string& message)
		: GameError("Error de formato en " + filename +
			", linea " + std::to_string(line) +
			": " + message)
	{}

};
#endif // FILE_FORMAT_ERROR_H
