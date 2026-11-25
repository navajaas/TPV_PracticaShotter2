#include "MainMenuState.h"
#include "game.h"
#include "Texture.h"
#include "Button.h"
#include "PlayState.h"
#include <iostream>

MainMenuState::MainMenuState(Game* game)
	: GameState(game)
{
	std::cout << "Menú de inicio" << std::endl;
	
	Button* newGameButton = new Button(this, game->getTexture(Game::NUEVA_PARTIDA), Point2D<float>(250, 200));
	Button* loadGameButton = new Button(this, game->getTexture(Game::CARGAR_PARTIDA), Point2D<float>(250, 300));
	Button* exitButton = new Button(this, game->getTexture(Game::SALIR), Point2D<float>(300, 400));

	newGameButton->connect([game]() { game->replaceState(new PlayState(game));});
	/*loadGameButton->connect([game]() { game->pushState(new PlayState(game));});
	exitButton->connect([game]() { game->pushState(new PlayState(game));});*/

	addGameObject(newGameButton);
	addGameObject(loadGameButton);
	addGameObject(exitButton);

}

void MainMenuState::render() const
{

	game->getTexture(Game::MAINMENU_BACKGROUND)->render();
	GameState::render();

}

