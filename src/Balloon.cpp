#include "Balloon.h"
Balloon::Balloon(Game* game, Rectangle rect, Texture* tex, float speed, int color)
	: game(game), rect(rect), textura(tex), speed(speed), color(color), alive(true)
{
}

void Balloon::render() const
{
	if (textura && alive)
	{
		textura->renderFrame(rect.to_sdl(), color, 0);
	}
}
bool Balloon::update(float delta) {
	// Actualiza la posición del globo
	rect.move(Vector2D<>(0.0f, -speed * delta));

	// Si sale de la pantalla por arriba, se destruye
	if (rect.getTopLeftCorner().getY() + rect.getHeight() < 0 || !alive) {
		return false;
	}

	return true;
}

bool Balloon::hit(const Rectangle& other)
{
	if (alive && rect.hasIntersection(other))
	{
		alive = false;
		return true;
	}

	return false;
}
