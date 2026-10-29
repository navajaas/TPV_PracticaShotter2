#ifndef GAMEOJECT_H
#define GAMEOBJECT_J

#include <SDL3/SDL.h>

class Game;


class GameObject
{
protected:
	Game* game;
public:
	GameObject(Game* g);
	~GameObject();

	virtual void render() const = 0;
	virtual bool update(float delta) = 0;
	virtual void handleEvents(const SDL_Event& event) {};
};

#endif //GAMEOBJECT_H
