#include "PauseState.h"
#include "MainMenuState.h"
#include "game.h"
#include "Texture.h"
#include "Button.h"
#include "PlayState.h"
#include <iostream>

PauseState::PauseState(Game* game)
	: GameState(game)
{
	std:: cout << "Pausa" << std::endl;
	Button* resumeButton = new Button(this, game->getTexture(Game::REANUDAR), Point2D<float>(250, 200));
	Button* saveButton = new Button(this, game->getTexture(Game::GUARDAR_PARTIDA), Point2D<float>(250, 280));
	Button* cargarButton = new Button(this, game->getTexture(Game::CARGAR_PARTIDA), Point2D<float>(250, 360));
	Button* mainMenuButton = new Button(this, game->getTexture(Game::VOLVER_AL_MENU), Point2D<float>(250, 440));

	mainMenuButton->connect([game]() { game->replaceState(new MainMenuState(game)); });
	
	addGameObject(resumeButton);
	addGameObject(saveButton);
	addGameObject(cargarButton);
	addGameObject(mainMenuButton);
}

void PauseState::render() const
{
	game->getTexture(Game::MAINMENU_BACKGROUND)->render();
	GameState::render();
}
