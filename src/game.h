#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <array>
#include <istream>
#include <string>
#include <vector>
#include "rectangle.h"
#include <random>

// Declaraciones anticipadas
class Texture;
class Bow;
class Balloon;

/**
 * Clase principal del juego.
 */
class Game
{
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
		NUM_TEXTURES
	};

	enum LevelName
	{
		LEVEL1 = 0,
		LEVEL2,
		NUM_LEVELS
	};

private:
	SDL_Window* window;
	SDL_Renderer* renderer;
	std::array<Texture*, NUM_TEXTURES> textures;
	
	// Sistema de niveles y puntuación
	int currentLevel = 1;
	int score = 0;
	int targetScore = 10;  // Puntuación necesaria para pasar de nivel
	int balloonPoints = 1;

	bool gameWon = false;

	void checkLevelProgress();

	void render() const;
	void update(float delta);
	void handleEvents();

	bool exit;
	void spawnBalloon();
	float spawnTimer = 0.0f; // Temporizador para el spawn de globos
	float spawnInterval = 1.5; // Intervalo de tiempo entre spawns de
	int _arrows;
	std::mt19937_64 randomGenerator{ std::random_device{}() };

	// Elemento del juego
	// TODO: añadir atributos para los objetos del juego
	Bow* bow = nullptr;
	std::vector<class Arrow*> arrows; // Vector para almacenar las flechas disparadas
	std::vector<class Balloon*> balloons; // Vector para almacenar los globos
public:
	Game();
	~Game();

	// Obtiene una textura por su nombre
	Texture* getTexture(TextureName name) const;

	// Ejecuta el bucle principal del juego
	void run();
	void addArrow(Arrow* arrow);
	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);
	void loadMap(const std::string& file);
	void addScore(int points);
};

inline Texture*
Game::getTexture(TextureName name) const
{
	return textures[name];
}

#endif // GAME_H
