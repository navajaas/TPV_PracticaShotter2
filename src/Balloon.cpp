#include "Balloon.h"
#include <iostream>

constexpr float FRAME_DURATION = 0.08f;
constexpr int EXPLOSION_FRAMES = 6;


Balloon::Balloon(Game* game, Rectangle rect, Texture* tex, Vector2D<float> speed, int color)
	: MobileObject(game,rect,tex,speed), color(color)
{
}

void Balloon::render() const
{
	if (texture)
	{
		int frame = exploding ? explosionFrame : 0;
		texture->renderFrame(rect.to_sdl(), color, frame);
	}
}

bool Balloon::update(float delta) {
   
    MobileObject::update(delta);

    if (exploding) {
        animationTimer += delta;
            explosionFrame++;

            if (explosionFrame >= EXPLOSION_FRAMES) {
                return false;
            }

        return true;
    }


    // Si está vivo, continúa subiendo
    rect.move(Vector2D<>(speed*delta));

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
