#include "GameState.h"
#include "GameObject.h"

GameState::~GameState()
{
	for(GameObject* obj : gameObjects) {
		delete obj;
	}
}

void GameState::render() const
{
	for (const GameObject* obj : gameObjects) {
		obj->render();
	}
}

void GameState::update(float delta)
{
	for (GameObject* obj : gameObjects) {
		obj->update(delta);
	}
}

void GameState::handleEvent(const SDL_Event& event)
{
	for (EventHandler* eh : eventHandlers) {
		eh->handleEvents(event);
	}
}

