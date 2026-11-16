#ifndef GAMESTATE_H
#define GAMESTATE_H

#include <list>
#include <SDL3/SDL.h>
#include "EventHandler.h"

class Game;
class GameObject;

class GameState
{
protected:
	Game* game;
	std::list<GameObject*> gameObjects;
	std::list<EventHandler*> eventHandlers;

public:
	GameState(Game* game) : game(game) {}
	~GameState();

	virtual void render() const;
	virtual void update(float delta);
	virtual void handleEvent(const SDL_Event& event);

	void addGameObject(GameObject* obj) { gameObjects.push_back(obj); }
	void addEventHandler(EventHandler* handler) { eventHandlers.push_back(handler); }

	Game* getGame() const { return game; }

};

#endif // GAMESTATE_H
