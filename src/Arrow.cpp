#include "Arrow.h"

constexpr float ARROW_SPEED = 300.0f;

Arrow::Arrow(Game* game, Rectangle rect, Texture* textura)
	: MobileObject(game, rect, textura, { ARROW_SPEED , 0 })
{
}
void Arrow::render() const
{
	if (texture)
	{
		texture->renderFrame(rect.to_sdl(), 0,0);
	}
}

bool Arrow::update(float delta) {

	if (rect.getTopLeftCorner().getX() > game->WINDOW_WIDTH)
		return false;

	// Actualiza la posición de la flecha
	rect.move(Vector2D<>(speed*delta));
	rectColision = Rectangle(rect.getTopLeftCorner(), rect.getWidth()-20, rect.getHeight());
	int score = game->getScore();

	if(game->checkCollision(rectColision))
	{
		balloonHitCount++;
		game->addScore(10, balloonHitCount);
	}

	// Si sale de la pantalla, se elimina
	if (rect.getTopLeftCorner().getX() + rect.getWidth() < 0) {
		return false;
	}

	return true;
}
