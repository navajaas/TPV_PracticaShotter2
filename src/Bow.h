#ifndef BOW_H
#define BOW_H

#include "Vector2D.h"
#include "rectangle.h"
#include <SDL3/SDL.h>
#include "texture.h"
#include "Arrow.h"

class Game;
class Arrow;
class Bow
{
public:
	Bow(Game* game, Rectangle rect, Texture* textura);
	
	void render() const;
	bool update(float delta);
	void handleEvents(const SDL_Event& event);
	void shootArrow();
private:
	Game* game=nullptr;
	Texture* textura=nullptr;
	Rectangle rect;
	Vector2D<> velocity;
	float speed = 200.0f; // Velocidad de movimiento del arco
	float arrowsCoolDown = 0.2f; // Tiempo de espera entre disparos
	float timeSinceLastShot = 0.0f; // Tiempo transcurrido desde el último disparo
	bool arrowArmad= false;
	Arrow* arrow = nullptr; // Puntero a la flecha asociada al arco
};
#endif // BOW_H
