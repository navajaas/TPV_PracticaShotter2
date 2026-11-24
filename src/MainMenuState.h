#ifndef MAINMENUSTATE_H
#define MAINMENUSTATE_H

#include "GameState.h"

class MainMenuState : public GameState
{
public:
	MainMenuState(Game* game);
	//~MainMenuState();
	void render() const override;
	//void update(float delta) override;
	//void handleEvent(const SDL_Event& event) override;

};

#endif // MAINMENUSTATE_H
