#include "game.h"

#include <string>
#include <fstream>
#include <sstream>
#include <random>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "Bow.h"
#include "Balloon.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";
constexpr float width = 60.0f;
constexpr float height = 60.0f;

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

constexpr array<const char*, Game::NUM_LEVELS> levelList{
	"1.txt",
	"2.txt"
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
		for (size_t i = 0; i < levels.size(); i++) {
			std::string name = levelList[i];
			levels[i] = lvlRoot + name;
		}
	}
	catch (...) {

		for (Balloon* b : balloons) {
			delete b;
		}
		for (Arrow* a : arrows) {
			delete a;
		}
		for (Texture* t : textures) {
			delete t;
		}
		delete bow;

		SDL_DestroyRenderer(renderer);
		SDL_DestroyWindow(window);
		SDL_Quit();

		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	Point2D position(50.0f, 200.0f);  
	float width = 64.0f;            
	float height = 64.0f;             

	bow = new Bow(this, Rectangle(position, width, height), getTexture(BOW));

	loadMap(levels[0]);
	
}

Game::~Game()
{
	for(Balloon* b : balloons) {
		delete b;
	}
	for (Arrow* a : arrows) {
		delete a;
	}
	for(Texture* t : textures) {
		delete t;
	}
	delete bow;

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void
Game::render() const
{
	SDL_RenderClear(renderer);

	getTexture(BACKGROUND)->render();

	if(bow) bow->render();

	for (const auto& balloon : balloons) {
		balloon->render();
	}

	for (const auto& arrow : arrows) {
		arrow->render();
	}

	SDL_RenderPresent(renderer);
}

void
Game::update(float delta)
{
	spawnTimer += delta;
	if (spawnTimer >= spawnInterval) {
		spawnBalloon();
		spawnTimer = 0.0f;
	}
	
	if (bow) bow->update(delta);

	for (const auto& arrow : arrows) {
		arrow->update(delta);
	}

	for (auto it = balloons.begin(); it != balloons.end(); )
	{
		if (!(*it)->update(delta))
		{
			delete* it;
			it = balloons.erase(it);
		}
		else ++it;
		
	}

	if (_arrows <= 0) exit = true;
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

void Game::addArrow(Arrow* arrow)
{
	if (_arrows > 0) {
		std::cout << "Arrows left: " << _arrows << endl;
		_arrows--;
		arrows.push_back(arrow);
	}
	
}

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		bow->handleEvents(event);
		
	}
}

bool Game::checkCollision(const Rectangle& rect)
{
	for (Balloon* b : balloons)
	{
		if (b->hit(rect))
			return true;
	}

	return false;
}

void Game::loadMap(const std::string& filename)
{
	std::ifstream file(filename);

	if (!file.is_open())
		throw std::string("Error al abrirse el archivo: ") + filename;

	std::string line;

	while (std::getline(file, line)) {
		if (line.empty() || line[0] == '#')
			continue;

		std::istringstream iss(line);
		std::string key;
		iss >> key;

		if (key == "arrows") { 

			if (!(iss >> _arrows) || _arrows < 0)
				throw std::string("Error leyendo la cantidad de flechas");
		}
		else if (key == "baldelay") {
			int milliseconds;

			if (!(iss >> milliseconds) || milliseconds <= 0)
				throw std::string("Error leyendo el intervalo de spawn");

			spawnInterval = milliseconds / 1000.0f;
		}
	}

	
}


void Game::spawnBalloon()
{

    int minX = WINDOW_WIDTH / 2;
    int maxX = WINDOW_WIDTH - width;

    std::uniform_int_distribution<int> xDistribution(minX, maxX);
    float randomX = xDistribution(randomGenerator);
    float startY = WINDOW_HEIGHT;

    int numColors = getTexture(BALLOON)->getNumRows();

    int randomColorIndex = std::uniform_int_distribution<int>(0, numColors - 1)(randomGenerator);
    float randomSpeed = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

    balloons.push_back(
        new Balloon(this, { { randomX, startY }, width, height }, getTexture(BALLOON),
                    randomSpeed, randomColorIndex)
    );
}
