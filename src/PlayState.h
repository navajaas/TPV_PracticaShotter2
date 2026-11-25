#ifndef PLAYSTATE_H
#define PLAYSTATE_H

#include "GameState.h"
#include <vector>
#include <random>
#include <functional>
#include "rectangle.h"

class Bow;
class Balloon;
class Butterfly;
class InfoBar;
class SceneObject;

class PlayState : public GameState
{

private:
	float spawnTimer = 0.0f; // Temporizador para el spawn de globos
	float spawnInterval = 1.5; // Intervalo de tiempo entre spawns de
	int _arrows;

	std::vector<SceneObject*> sceneObjects;

	std::mt19937_64 randomGenerator{ std::random_device{}() };

	// Elemento del juego
	InfoBar* infoBar = nullptr;

	int r, g, b;
	int aliveButterflies = 0;

	// Sistema de niveles y puntuación
	int currentLevel = 1;
	int score = 0;
	int targetScore = 10;  // Puntuación necesaria para pasar de nivel
	int balloonPoints = 1;

	bool gameWon = false;
	bool gameLost = false;
	bool checkLevelProgress();
	void changeLevel();

	void handleEvent(const SDL_Event& event) override;

	void spawnBalloon();
	void spawnButterflyes(int cantidad);

	void saveGame() const;
	void loadGame();

public:
	PlayState(Game*);
	~PlayState();

	void update(float delta) override;
	void render() const override;

	// Comprueba si hay algún objeto colocado en ese rectángulo
	bool checkCollision(const Rectangle& rect);

	void loadMap(const std::string& file);
	void addScore(int points, int numBallons = 1);
	void addObject(SceneObject* obj) { sceneObjects.push_back(obj); }
	void updateArrows();
	void onButterflyKilled();

};

#endif // PLAYSTATE_H
