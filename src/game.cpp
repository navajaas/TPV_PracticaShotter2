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

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";
constexpr float BALLOON_SIZE = 60.0f;
constexpr float BOW_SIZE = 64.0f;

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
	{"background1.png"}
};


Game::Game()
  : exit(false)
{
	// Carga SDL y crea la ventana con su renderizador
	if (!SDL_Init(SDL_INIT_VIDEO))
		throw "SDL_Init: "s + SDL_GetError();

	if (!SDL_CreateWindowAndRenderer(WINDOW_TITLE,
	                                 WINDOW_WIDTH, WINDOW_HEIGHT,
	                                 0, &window, &renderer))
		throw "window or renderer: "s + SDL_GetError();

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

		for (SceneObject* o : objects) {
			delete o;
		}

		delete infoBar;

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Point2D position(50.0f, 200.0f);              

	addObject(new Bow(this, Rectangle(position, BOW_SIZE, BOW_SIZE), getTexture(BOW)));
	//infoBar = new InfoBar(Rectangle(Point2D<>(50, 30), 60, 15), _arrows, getTexture(ARROW));

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

	getTexture(BACKGROUND)->render();

	for (SceneObject* o : objects) {
		o->render();
	}

	//infoBar->render();
	SDL_RenderPresent(renderer);
}

void Game::update(float delta)
{
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

void
Game::run()
{
	Uint64 lastTime = SDL_GetTicks();
	while (!exit) {
		Uint64 currentTime = SDL_GetTicks();
		float delta = (currentTime - lastTime) / 1000.0f; 
		lastTime = currentTime;

		render();
		handleEvents();
		update(delta);
		SDL_Delay(16); 
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
	}
}

bool Game::checkCollision(const Rectangle& rect)
{
	for (SceneObject* o : objects)
	{
		if (o->hit(rect)) {
			addScore(balloonPoints);
			return true;
		}
	}

	return false;
}

void Game::loadMap(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
		throw std::string("Error al abrirse el archivo: ") + filename;

	// Valores por defecto para este nivel.
	int newArrows = -1;
	int newTargetScore = 10;
	float newSpawnInterval = 1.5f;
	int newBalloonPoints = 1;

	std::string line;

	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "arrows") {
			if (!(iss >> newArrows) || newArrows < 0)
				throw std::string(
					"Error leyendo la cantidad de flechas");
		}
		else if (key == "baldelay") {
			int milliseconds;

			if (!(iss >> milliseconds) || milliseconds <= 0)
				throw std::string(
					"Error leyendo el intervalo de spawn");

			newSpawnInterval = milliseconds / 1000.0f;
		}
		else if (key == "goal") {
			if (!(iss >> newTargetScore) || newTargetScore <= 0)
				throw std::string(
					"Error leyendo la puntuacion objetivo");
		}
		else if (key == "balpoints") {
			if (!(iss >> newBalloonPoints) || newBalloonPoints <= 0)
				throw std::string(
					"Error leyendo el número de puntos que aporta cada globo explotado");
		}
	}

	if (newArrows < 0)
		throw std::string(
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

    objects.push_back(
        new Balloon(this,{ { randomX, startY }, BALLOON_SIZE, BALLOON_SIZE }, getTexture(BALLOON),
			{0,-randomSpeedY}, randomColorIndex)
    );

}

void Game::addScore(int points)
{
	if (points <= 0 || gameWon || exit)
		return;

	score += points;

	std::cout << "Puntuacion: " << score
		<< " / " << targetScore << '\n';

	checkLevelProgress();
}

void Game::addObject(SceneObject* object)
{
	objects.push_back(object);
}

void Game::checkLevelProgress()
{
	if (score < targetScore)
		return;

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
