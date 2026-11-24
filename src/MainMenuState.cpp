#include "MainMenuState.h"
#include "game.h"
#include "Texture.h"
#include <iostream>

MainMenuState::MainMenuState(Game* game)
	: GameState(game)
{
	std::cout << "Menú de inicio" << std::endl;
}

void MainMenuState::render() const
{
	game->getTexture(Game::MAINMENU_BACKGROUND)->render();
	GameState::render();
}

