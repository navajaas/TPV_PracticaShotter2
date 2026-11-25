#ifndef PAUSESTATE_H
#define PAUSESTATE_H

#include "GameState.h"
#include "PlayState.h"

class PauseState : public GameState
{
public:
	PauseState(Game* game, PlayState* playState);
	//~PauseState();
	void render() const override;
	//void update(float delta) override;
	//void handleEvent(const SDL_Event& event) override;
private:
	PlayState* playstate=nullptr; // Puntero al estado de juego activo
};

#endif // PAUSESTATE_H
