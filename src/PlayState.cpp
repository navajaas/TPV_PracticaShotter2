#include "PlayState.h"
#include "rectangle.h"
#include "GameObject.h"
#include "SceneObject.h"
#include "Bow.h"
#include "Balloon.h"
#include "Butterfly.h"
#include "game.h"
#include "GameError.h"
#include <fstream>
#include <sstream>

constexpr const char* LEVEL_ROOT = "../assets/levels/";
constexpr float BALLOON_SIZE = 60.0f;
constexpr float BOW_SIZE = 64.0f;
constexpr float BUTTERFLY_WIDTH = 64.0f;
constexpr float BUTTERFLY_HEIGHT = 44.0f;

PlayState::PlayState(Game* game) : GameState(game)
{
	loadMap(std::string(LEVEL_ROOT) + "1.txt");
}

PlayState::~PlayState()
{
	for (SceneObject* o : sceneObjects) {
		delete o;
	}
}

void PlayState::update(float delta)
{
	GameState::update(delta);

	if (checkLevelProgress()) {
		changeLevel();
	}
	spawnTimer += delta;

	if (spawnTimer >= spawnInterval)
	{
		spawnBalloon();
		spawnTimer = 0.0f;
	}

	for (auto it = sceneObjects.begin(); it != sceneObjects.end(); )
	{
		SceneObject* obj = *it;

		if (!obj->update(delta))
		{
			delete obj;
			it = sceneObjects.erase(it);
		}
		else
		{
			++it;
		}
	}

	game->processCallbacks();
}

void PlayState::render() const
{

	SDL_RenderClear(game->getRenderer());

	SDL_SetRenderDrawColor(game->getRenderer(), r, g, b, 255);
	
	GameState::render();

	for (SceneObject* o : sceneObjects) {
		o->render();
	}

	//infoBar->render();

	SDL_RenderPresent(game->getRenderer());

}

void PlayState::onButterflyKilled() {
	aliveButterflies--;
	if (aliveButterflies <= 0) {
		std::cout << "¡Has matado a todas las mariposas!\n";
		gameLost = true;
	}
}

void PlayState::updateArrows() {
	_arrows--;
	if (_arrows < 0) gameLost = true;
}


void PlayState::changeLevel() {
	// El siguiente nivel debe tener un número consecutivo.
	const int nextLevel = currentLevel + 1;

	const std::string nextFile =
		std::string(LEVEL_ROOT) +
		std::to_string(nextLevel) + ".txt";

	std::ifstream file(nextFile);

	if (!file.is_open()) {
		gameWon = true;

		std::cout << "¡Enhorabuena! Has superado todos los niveles.\n";
		return;
	}

	file.close();

	currentLevel = nextLevel;
	score = 0;

	for (SceneObject* o : sceneObjects) {
		delete o;
	}

	sceneObjects.clear();

	spawnTimer = 0.0f;

	// Carga la configuración del nuevo nivel.
	loadMap(nextFile);

	std::cout << "¡Nivel " << currentLevel << "!\n";
}


bool PlayState::checkLevelProgress()
{
	if (score < targetScore)
		return false;
	else if (score >= targetScore) {
		return true;
	}
}

void PlayState::addScore(int points, int numBallons)
{
	if (gameWon || gameLost || exit)
		return;
	if (numBallons <= 1) {
		score += points;

	}
	else {
		score += points * numBallons;
	}

	std::cout << "Puntuacion: " << score
		<< " / " << targetScore << '\n';

}

void PlayState::loadGame()
{
	const std::string filename = "saved.txt";
	std::ifstream file(filename);

	if (!file.is_open())
		throw FileNotFoundError(filename);

	int lineNumber = 1;

	std::string header;
	int version;

	if (!(file >> header >> version) ||
		header != "GAME" || version != 1)
	{
		throw FileFormatError(filename, lineNumber,
			"Cabecera o version incorrecta");
	}

	int savedLevel;
	int savedScore;
	bool savedGameWon;
	size_t numObjects;

	lineNumber++;

	if (!(file >> savedLevel >> savedScore
		>> savedGameWon >> numObjects))
	{
		throw FileFormatError(filename, lineNumber,
			"Datos generales incorrectos");
	}

	if (savedLevel < 1 || numObjects > 10000)
		throw FileFormatError(filename, lineNumber,
			"Datos de partida no validos");

	std::vector<SceneObject*> loadedObjects;

	try
	{
		for (size_t i = 0; i < numObjects; ++i)
		{
			lineNumber++;

			std::string type;

			if (!(file >> type))
				throw FileFormatError(filename, lineNumber,
					"Falta el tipo de objeto");

			SceneObject* obj = nullptr;

			if (type == "BOW")
			{
				Bow* bow = new Bow(
					this,
					Rectangle(),
					game->getTexture(Game::BOW)
				);
				addObject(bow);
				addEventHandler(bow);
			}
			else if (type == "BALLOON")
			{
				obj = new Balloon(
					this,
					Rectangle(),
					game->getTexture(Game::BALLOON),
					Vector2D<float>(),
					0
				);
			}
			else if (type == "BUTTERFLY")
			{
				obj = new Butterfly(
					this,
					Rectangle(),
					game->getTexture(Game::MARIPOSA),
					Vector2D<float>()
				);
			}
			else
			{
				throw FileFormatError(filename, lineNumber,
					"Tipo de objeto desconocido: " + type);
			}

			try
			{
				obj->load(file, lineNumber);
				loadedObjects.push_back(obj);
			}
			catch (...)
			{
				delete obj;
				throw;
			}
		}
	}
	catch (...)
	{
		for (SceneObject* obj : loadedObjects)
			delete obj;

		throw;
	}

	file >> std::ws;
	if (!file.eof())
	{
		for (SceneObject* obj : loadedObjects)
			delete obj;

		throw FileFormatError(filename, lineNumber,
			"Hay datos adicionales al final del archivo");
	}


	for (SceneObject* obj : sceneObjects) {
		delete obj;
	}

	sceneObjects.clear();
	sceneObjects = std::move(loadedObjects);

	currentLevel = savedLevel;
	score = savedScore;
	gameWon = savedGameWon;
}


void PlayState::saveGame() const
{
	std::ofstream file("saved.txt");

	if (!file)
		throw FileNotFoundError("saved.txt");

	file << "GAME 1\n";

	file << currentLevel << ' '
		<< score << ' '
		<< gameWon << ' '
		<< sceneObjects.size() << '\n';

	for (SceneObject* obj : sceneObjects)
	{
		file << obj->getType() << ' ';
		obj->save(file);
		file << '\n';
	}
}


void PlayState::spawnButterflyes(int cantidad) {

	aliveButterflies = cantidad;
	for (int i = 0; i < cantidad; i++) {
		int minX = 10;
		int maxX = Game::WINDOW_WIDTH - BUTTERFLY_WIDTH;

		int minY = 10;
		int maxY = Game::WINDOW_HEIGHT - BUTTERFLY_HEIGHT;

		std::uniform_int_distribution<int> xDistribution(minX, maxX);
		std::uniform_int_distribution<int> yDistribution(minY, maxY);

		float randomX = xDistribution(randomGenerator);
		float randomY = yDistribution(randomGenerator);

		float randomSpeedY = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);
		float randomSpeedX = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

		addObject(new Butterfly(this, { { randomX, randomY }, BUTTERFLY_WIDTH, BUTTERFLY_HEIGHT }, game->getTexture(Game::MARIPOSA),
			{ randomSpeedX,randomSpeedY })
		);
	}

}

bool PlayState::checkCollision(const Rectangle& rect)
{
	bool hitBallon = false;
	for (SceneObject* o : sceneObjects)
	{
		if (o->hit(rect)) {
			if (o->getType() == "BALLOON") {
				hitBallon = true;
			}
		}
	}

	return hitBallon;
}


void PlayState::loadMap(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
		throw FileNotFoundError(filename);

	Point2D position(50.0f, 200.0f);

	addObject(new Bow(this, Rectangle(position, BOW_SIZE, BOW_SIZE), game->getTexture(Game::BOW)));

	// Valores por defecto para este nivel
	int newArrows = -1;
	int newTargetScore = 10;
	float newSpawnInterval = 1.5f;
	int newBalloonPoints = 1;
	aliveButterflies = 0;
	std::string line;
	int lineNumber = 0;

	while (std::getline(file, line))
	{
		lineNumber++;

		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream iss(line);
		std::string key;

		if (!(iss >> key))
			continue;

		if (key == "arrows")
		{
			if (!(iss >> newArrows) || newArrows < 0)
				throw FileFormatError(filename, lineNumber,
					"Error leyendo la cantidad de flechas");
		}
		else if (key == "baldelay")
		{
			int milliseconds;

			if (!(iss >> milliseconds) || milliseconds <= 0)
				throw FileFormatError(filename, lineNumber,
					"Error leyendo el intervalo de spawn");

			newSpawnInterval = milliseconds / 1000.0f;
		}
		else if (key == "goal")
		{
			if (!(iss >> newTargetScore) || newTargetScore <= 0)
				throw FileFormatError(filename, lineNumber,
					"Error leyendo la puntuacion objetivo");
		}
		else if (key == "balpoints")
		{
			if (!(iss >> newBalloonPoints) || newBalloonPoints <= 0)
				throw FileFormatError(filename, lineNumber,
					"Error leyendo los puntos de cada globo");
		}
		else if (key == "butterflies")
		{
			int numButterflies;

			if (!(iss >> numButterflies) || numButterflies < 0)
				throw FileFormatError(filename, lineNumber,
					"Error leyendo la cantidad de mariposas");

			spawnButterflyes(numButterflies);
		}
		else if (key == "bgcolor")
		{
			if (!(iss >> r >> g >> b) ||
				r < 0 || r > 255 ||
				g < 0 || g > 255 ||
				b < 0 || b > 255)
			{
				throw FileFormatError(filename, lineNumber,
					"Error leyendo el color de fondo");
			}
		}
	}

	if (newArrows < 0)
		throw FileFormatError(filename, lineNumber,
			"El nivel no especifica una cantidad valida de flechas");

	_arrows = newArrows;
	targetScore = newTargetScore;
	spawnInterval = newSpawnInterval;
	balloonPoints = newBalloonPoints;
}


void PlayState::spawnBalloon()
{

	int minX = Game::WINDOW_WIDTH / 2;
	int maxX = Game::WINDOW_WIDTH - BALLOON_SIZE;

	std::uniform_int_distribution<int> xDistribution(minX, maxX);
	float randomX = xDistribution(randomGenerator);
	float startY = Game::WINDOW_HEIGHT;

	int numColors = game->getTexture(Game::BALLOON)->getNumRows();

	int randomColorIndex = std::uniform_int_distribution<int>(0, numColors - 1)(randomGenerator);
	float randomSpeedY = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

	Balloon* b = new Balloon(this, { { randomX, startY }, BALLOON_SIZE, BALLOON_SIZE }, game->getTexture(Game::BALLOON),
		{ 0,-randomSpeedY }, randomColorIndex);
	addObject(b);
	b->setPoints(balloonPoints);


}
