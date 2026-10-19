#include "game.h"

#include <string>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "Bow.h"
#include "Balloon.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";
constexpr const char* LEVEL_ROOT = "../assets/levels/";

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
	{"balloons.png",7,6}
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

		for (size_t i = 0; i < textures.size(); i++) {
			auto [name, nrows, ncols] = textureList[i];
			textures[i] = new Texture(renderer, (imgRoot + name).c_str(), nrows, ncols);
		}
	}
	catch (...) {
		// TODO: liberar la memoria reservada
		throw; // relanza la excepción
	}

	// Configura que se pueden utilizar capas translúcidas
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

	// TODO: crear los objetos del juego
	/*
	bow = new Bow(Rectangle(100, 100, 50, 50), Rectangle(0, 0, 50, 50), textures[0]);
	balloon = new Balloon(Rectangle(200, 200, 30, 30), Rectangle(0, 0, 30, 30), textures[1]);
	
	arrow = new Arrow(Rectangle(150, 150, 20, 20), Rectangle(0, 0, 20, 20), textures[2]);*/
	
	Point2D position(50.0f, 200.0f); // Posición a la izquierda
	float width = 64.0f;              // Ajusta según la anchura de tu textura de arco
	float height = 64.0f;             // Ajusta según la altura

	bow = new Bow(this, Rectangle(position, width, height), getTexture(BOW));
	
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
	// TODO: liberar la memoria reservada por la clase
}

void
Game::render() const
{
	SDL_RenderClear(renderer);
	if(bow) bow->render();
	//arrow.render();
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
	//if (balloon) balloon->update(delta);
	for (const auto& arrow : arrows) {
		arrow->update(delta);
	}
	for (const auto& balloon : balloons) {
		balloon->update(delta);
	}
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

void
Game::handleEvents()
{
	SDL_Event event;

	// Only quit is handled directly, everything else is delegated
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_EVENT_QUIT)
			exit = true;

		// TODO
		bow->handleEvents(event);
	}
}
void Game::spawnBalloon() {
	float width = 60.0f; 
	float height = 60.0f; 

	float minX = WINDOW_WIDTH / 2.0f; 
	float maxX = WINDOW_WIDTH - width; 
	float randomX = minX + (float)rand() / RAND_MAX * (maxX - minX);

	float startY = WINDOW_HEIGHT;

	int numColors = getTexture(BALLOON)->getNumRows(); 
	int randomColorIndex = rand() % numColors;

	float minSpeed = 50.0f;
	float maxSpeed = 150.0f; 
	float randomSpeed = minSpeed + (float)rand() / RAND_MAX * (maxSpeed - minSpeed);

	Point2D position(randomX, startY);
	Rectangle rect(position, width, height);

	balloons.push_back(new Balloon(this, rect, getTexture(BALLOON), randomSpeed, randomColorIndex));
}
// bool
// Game::checkCollision(const Rectangle& rect)
// {
//	// TODO
//	return false;
// }
