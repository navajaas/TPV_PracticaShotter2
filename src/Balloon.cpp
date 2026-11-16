#include "Balloon.h"
#include "game.h"
#include <iostream>
#include "GameError.h"
#include "PlayState.h"

constexpr float FRAME_DURATION = 0.08f;
constexpr int EXPLOSION_FRAMES = 6;


Balloon::Balloon(PlayState* game, Rectangle rect, Texture* tex, Vector2D<float> speed, int color)
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

void Balloon::save(std::ostream& os) const
{
    MobileObject::save(os);

    os << color << ' '
        << exploding << ' '
        << points << ' ';
}

void Balloon::load(std::istream& is, int line)
{
    MobileObject::load(is, line);

    if (!(is >> color >> exploding >> points))
		throw FileFormatError("saved.txt", line, "Error al leer Balloon");

    explosionFrame = 0;
    animationTimer = 0.0f;
}
