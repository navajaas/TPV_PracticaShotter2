#include "game.h"

#include <string>
#include <fstream>

#include <random>

#include <SDL3_image/SDL_image.h>

#include "texture.h"
#include "Bow.h"
#include "Balloon.h"
#include "InfoBar.h"
#include "SceneObject.h"
#include "Butterfly.h"
#include "GameError.h"
#include "PlayState.h"
#include "MainMenuState.h"
#include "PauseState.h"
#include "EndState.h"

using namespace std;

// Constantes
constexpr const char* const WINDOW_TITLE = "Bow & Arrow 1.0";
constexpr const char* IMAGE_ROOT = "../assets/images/";


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
	{"butterfly.png",4,10},
	{"menuBackground.png"},
	{"texts/nuevaPartida.png"},
	{"texts/cargarPartida.png"},
	{"texts/salir.png"}
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

	pushState(std::make_shared<MainMenuState>(this));
	
}

Game::~Game()
{

	for(Texture* t : textures) {
		delete t;
	}

	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}

void Game::run()
{
	const Uint64 frameTime = 16; // 16 ms ≈ 60 FPS

	Uint64 lastTime = SDL_GetTicks();

	while (!exit) {

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

void Game::render() const
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	GameStateMachine::render();

	SDL_RenderPresent(renderer);
}

void Game::handleEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_EVENT_QUIT)
		{
			exit = true;
		}
		else
		{
			GameStateMachine::handleEvent(event);
		}
	}
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
