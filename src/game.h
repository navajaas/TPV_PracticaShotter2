#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <string>
#include <vector>
#include "rectangle.h"
#include <functional>
#include "gameStateMachine.h"
#include "Button.h"

// Declaraciones anticipadas
class Texture;

/**
 * Clase principal del juego.
 */
class Game : public GameStateMachine
{
	using callback = std::function<void()>;
public:
	// Número de actualizaciones por segundo
	static constexpr int FRAME_RATE = 30;
	// Periodo entre actualizaciones en milisegundos
	static constexpr int FRAME_PERIOD = 1000 / FRAME_RATE;
	// Tamaño real de la ventana
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;

	enum TextureName
	{
		BOW = 0,
		ARROW,
		BALLOON,
		BACKGROUND,
		MARIPOSA,
		MAINMENU_BACKGROUND,
		NUEVA_PARTIDA,
		CARGAR_PARTIDA,
		SALIR,
		NUM_TEXTURES
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;

	bool exit;
	std::vector<callback> _callbacks;


public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();
	void render() const override;

	void handleEvents();
	void addCallback(callback callback);
	void processCallbacks();

	SDL_Window* getWindow() const;
	SDL_Renderer* getRenderer();


};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

inline SDL_Renderer*
Game::getRenderer()
{
	return renderer;
}

inline SDL_Window*
Game::getWindow() const
{
	return window;
}



#endif // GAME_H
