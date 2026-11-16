#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <SDL3/SDL.h>

class GameState;

class GameObject
{
protected:
	GameState* game;
public:
	GameObject(GameState* g);

	virtual void render() const = 0;
	virtual bool update(float delta) = 0;
	virtual void handleEvents(const SDL_Event& event) {};
};

#endif //GAMEOBJECT_H
