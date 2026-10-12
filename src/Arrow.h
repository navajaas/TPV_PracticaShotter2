#ifndef ARROW_H
#define ARROW_H

#include "Vector2D.h"
#include "rectangle.h"
#include "game.h"
#include "texture.h"
class Game;
class Arrow
{
public:
	Arrow(Game* game, Rectangle rect, Texture* textura);
	
	void render() const;
	bool update(float delta);
private:
	Game* game=nullptr;
	Texture* textura=nullptr;
	Rectangle rect;
	float speed = 300.0f; // Velocidad de movimiento de la flecha
};
#endif // ARROW_H

