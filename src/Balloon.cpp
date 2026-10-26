#include "Balloon.h"

constexpr float FRAME_DURATION = 0.08f;
constexpr int EXPLOSION_FRAMES = 6;


Balloon::Balloon(Rectangle rect, Texture* tex, float speed, int color)
	: rect(rect), textura(tex), speed(speed), color(color)
{
}

void Balloon::render() const
{
	if (textura)
	{
		int frame = exploding ? explosionFrame : 0;
		textura->renderFrame(rect.to_sdl(), color, frame);
	}
}
bool Balloon::update(float delta) {
    if (exploding) {
        animationTimer += delta;

        if (animationTimer >= FRAME_DURATION) {
            animationTimer -= FRAME_DURATION;
            explosionFrame++;

            if (explosionFrame >= EXPLOSION_FRAMES) {
                return false; 
            }
        }

        return true; 
    }

    // Si está vivo, continúa subiendo
    rect.move(Vector2D<>(0.0f, -speed * delta));

    // Si sale de la pantalla, se elimina
    if (rect.getTopLeftCorner().getY() + rect.getHeight() < 0) {
        return false;
    }

    return true;
}


bool Balloon::hit(const Rectangle& other)
{
    if (!exploding && rect.hasIntersection(other)) {
        exploding = true;
        explosionFrame = 0;
        animationTimer = 0.0f;
        return true;
    }

    return false;
}
