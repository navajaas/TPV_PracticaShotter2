#ifndef ARROW_H
#define ARROW_H

#include "vector2D.h"
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

	Game* game = nullptr;
	Rectangle rect;
	Texture* textura = nullptr;
	Rectangle rectColision;
};
#endif // ARROW_H

