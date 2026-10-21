#include "Arrow.h"
Arrow::Arrow(Game* game, Rectangle rect, Texture* textura)
	: game(game), rect(rect), textura(textura)
{
}
void Arrow::render() const
{
	if (textura)
	{
		textura->renderFrame(rect.to_sdl(), 0,0);
	}
}
bool Arrow::update(float delta) {
	// Actualiza la posición de la flecha
	rect.move(Vector2D<>(speed * delta, 0.0f));
	rectColision = Rectangle(rect.getTopLeftCorner(), rect.getWidth()-20, rect.getHeight());
	return true;
}
