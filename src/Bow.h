#ifndef BOW_H
#define BOW_H

#include "vector2D.h"
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
	Game* game = nullptr;
	Rectangle rect;
	Texture* textura = nullptr;
	Vector2D<> velocity;


	bool arrowArmad = false;
	float timeSinceLastShot = 0.0f; // Tiempo transcurrido desde el último disparo
	Arrow* arrow = nullptr; // Puntero a la flecha asociada al arco

};
#endif // BOW_H
