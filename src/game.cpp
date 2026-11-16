#include "game.h"

#include <string>
#include <fstream>
#include <sstream>
#include <random>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "Bow.h"
#include "Balloon.h"
#include "InfoBar.h"
#include "SceneObject.h"
#include "Butterfly.h"
#include "GameError.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";
constexpr float BALLOON_SIZE = 60.0f;
constexpr float BOW_SIZE = 64.0f;
constexpr float BUTTERFLY_WIDTH = 64.0f;
constexpr float BUTTERFLY_HEIGHT = 44.0f;

// Estructura para especificar las texturas que hay que
// cargar y el tamaño de su matriz de frames
struct TextureSpec
{
	const char* name;
	int nrows = 1;
	int ncols = 1;
};

constexpr array<TextureSpec, Game::NUM_TEXTURES> textureList{
	TextureSpec{"bow.png", 1, 2},
	{"arrow.png"},
	{"balloons.png",7,6},
	{"background1.png"},
	{"butterfly.png",4,10}
};


Game::Game()
  : exit(false)
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw SDLError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw SDLError();

	// Carga las texturas al inicio
	try {
		textures.fill(nullptr);
		string imgRoot = IMAGE_ROOT;
		string lvlRoot = LEVEL_ROOT;

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {

		for (Texture* t : textures) {
			delete t;
		}

		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	infoBar = new InfoBar(this, Rectangle(Point2D<>(50, 30), 60, 15), _arrows, getTexture(ARROW));

	loadMap(LEVEL_ROOT + std::string("1.txt"));
	
}

Game::~Game()
{
	for (SceneObject* o : objects) {
		delete o;
	}

	for(Texture* t : textures) {
		delete t;
	}
	delete infoBar;
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	SDL_SetRenderDrawColor(renderer, r, g, b, 255);

	for (SceneObject* o : objects) {
		o->render();
	}

	infoBar->render();

	SDL_RenderPresent(renderer);
}

void Game::update(float delta)
{
	if (checkLevelProgress()) {
		changeLevel();
	}
	spawnTimer += delta;

	if (spawnTimer >= spawnInterval)
	{
		spawnBalloon();
		spawnTimer = 0.0f;
	}

	for (auto it = objects.begin(); it != objects.end(); )
	{
		SceneObject* obj = *it;

		if (!obj->update(delta))
		{
			delete obj;
			it = objects.erase(it);
		}
		else
		{
			++it;
		}
	}

	processCallbacks();
}

void Game::run()
{
	const Uint64 frameTime = 16; // 16 ms ≈ 60 FPS

	Uint64 lastTime = SDL_GetTicks();

	while (!exit && !gameLost) {

		Uint64 frameStart = SDL_GetTicks();

		float delta = (frameStart - lastTime) / 1000.0f;
		lastTime = frameStart;

		handleEvents();
		update(delta);
		render();

		Uint64 frameEnd = SDL_GetTicks();
		Uint64 elapsed = frameEnd - frameStart;

		if (elapsed < frameTime) {
			SDL_Delay(frameTime - elapsed);
		}
	}
}

void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		const size_t count = objects.size();

		for (size_t i = 0; i < count; ++i) {
			objects[i]->handleEvents(event);
		}

		if (event.type == SDL_EVENT_KEY_DOWN &&
			(event.key.mod & SDL_KMOD_CTRL))
		{
			if (event.key.key == SDLK_S)
			{
				saveGame();
			}
			else if (event.key.key == SDLK_O)
			{
				loadGame();
			}
		}
	}
}

bool Game::checkCollision(const Rectangle& rect)
{
	bool hitBallon = false;
	for (SceneObject* o : objects)
	{
		if (o->hit(rect)) {
			if (o->getType() == "BALLOON") {
				hitBallon = true;
			}
		}
	}

	return hitBallon;
}
void Game::loadMap(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
		throw FileNotFoundError(filename);

	Point2D position(50.0f, 200.0f);

	addObject(new Bow(this, Rectangle(position, BOW_SIZE, BOW_SIZE), getTexture(BOW)));

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

void Game::spawnBalloon()
{

    int minX = WINDOW_WIDTH / 2;
    int maxX = WINDOW_WIDTH - BALLOON_SIZE;

    std::uniform_int_distribution<int> xDistribution(minX, maxX);
    float randomX = xDistribution(randomGenerator);
    float startY = WINDOW_HEIGHT;

    int numColors = getTexture(BALLOON)->getNumRows();

    int randomColorIndex = std::uniform_int_distribution<int>(0, numColors - 1)(randomGenerator);
	float randomSpeedY = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

	Balloon* b = new Balloon(this, { { randomX, startY }, BALLOON_SIZE, BALLOON_SIZE }, getTexture(BALLOON),
		{ 0,-randomSpeedY }, randomColorIndex);
	addObject(b);
	b->setPoints(balloonPoints);


}

void Game::spawnButterflyes(int cantidad) {
	aliveButterflies = cantidad;
	for (int i = 0; i < cantidad; i++) {
		int minX = 10;
		int maxX = WINDOW_WIDTH - BUTTERFLY_WIDTH;

		int minY = 10;
		int maxY = WINDOW_HEIGHT - BUTTERFLY_HEIGHT;

		std::uniform_int_distribution<int> xDistribution(minX, maxX);
		std::uniform_int_distribution<int> yDistribution(minY, maxY);

		float randomX = xDistribution(randomGenerator);
		float randomY = yDistribution(randomGenerator);

		float randomSpeedY = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);
		float randomSpeedX = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

		addObject(new Butterfly(this, { { randomX, randomY }, BUTTERFLY_WIDTH, BUTTERFLY_HEIGHT }, getTexture(MARIPOSA),
				{ randomSpeedX,randomSpeedY })
		);
	}
	
}

void Game::saveGame() const
{
	std::ofstream file("saved.txt");

	if (!file)
		throw FileNotFoundError("saved.txt");

	file << "GAME 1\n";

	file << currentLevel << ' '
		<< score << ' '
		<< gameWon << ' '
		<< objects.size() << '\n';

	for (const auto* obj : objects)
	{
		file << obj->getType() << ' ';
		obj->save(file);
		file << '\n';
	}
}

void Game::loadGame()
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
				obj = new Bow(
					this,
					Rectangle(),
					getTexture(Game::BOW)
				);
			}
			else if (type == "BALLOON")
			{
				obj = new Balloon(
					this,
					Rectangle(),
					getTexture(Game::BALLOON),
					Vector2D<float>(),
					0
				);
			}
			else if (type == "BUTTERFLY")
			{
				obj = new Butterfly(
					this,
					Rectangle(),
					getTexture(Game::MARIPOSA),
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
				obj->load(file,lineNumber);
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


	for (SceneObject* obj : objects) {
		delete obj;
	}

	objects.clear();
	objects = std::move(loadedObjects);

	currentLevel = savedLevel;
	score = savedScore;
	gameWon = savedGameWon;
}

void Game::addScore(int points, int numBallons)
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

void Game::addObject(SceneObject* object)
{
	objects.push_back(object);
}

bool Game::checkLevelProgress()
{
	if (score < targetScore)
		return false;
	else if (score >= targetScore) {
		return true;
	}
}

void Game::changeLevel() {
	// El siguiente nivel debe tener un número consecutivo.
	const int nextLevel = currentLevel + 1;

	const std::string nextFile =
		std::string(LEVEL_ROOT) +
		std::to_string(nextLevel) + ".txt";

	std::ifstream file(nextFile);

	if (!file.is_open()) {
		gameWon = true;
		exit = true;

		std::cout << "¡Enhorabuena! Has superado todos los niveles.\n";
		return;
	}

	file.close();

	currentLevel = nextLevel;
	score = 0;

	for (SceneObject* o : objects) {
		delete o;
	}

	objects.clear();

	spawnTimer = 0.0f;

	// Carga la configuración del nuevo nivel.
	loadMap(nextFile);

	std::cout << "¡Nivel " << currentLevel << "!\n";
}
	

void Game::updateArrows() {
	_arrows--;
	if (_arrows < 0) gameLost = true;
}

void Game::addCallback(std::function<void()> callback)
{
	_callbacks.push_back(std::move(callback));
}

void Game::processCallbacks()
{
	while (!_callbacks.empty())
	{
		auto pending = std::move(_callbacks);
		_callbacks.clear();

		for (auto& callback : pending)
			callback();
	}
}
void Game::onButterflyKilled() {
	aliveButterflies--;
	if (aliveButterflies <= 0) {
		std::cout << "¡Has matado a todas las mariposas!\n";
		gameLost = true;
	}
}
