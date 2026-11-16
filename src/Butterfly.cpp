#include "Butterfly.h"
#include "GameError.h"
#include <cmath>

constexpr int BUTTERFLY_ANIMATION_ROWS = 4;
constexpr int BUTTERFLY_ANIMATION_COLUMNS = 10;
constexpr int BUTTERFLY_TOTAL_FRAMES = 36;
constexpr float BUTTERFLY_FRAME_DURATION = 0.12f;
constexpr float FALL_SPEED = 200.0f;

constexpr float BUTTERFLY_WIDTH = 64.0f;
constexpr float BUTTERFLY_HEIGHT = 44.0f;


Butterfly::Butterfly(Game* game,Rectangle rect, Texture* tex, Vector2D<float> dirSpeed):
	MobileObject(game, rect, tex, dirSpeed)
{

}

void Butterfly::render() const
{
    if (texture)
    {
        if (hitByArrow)
        {
            texture->renderFrame(rect.to_sdl(), 0, 0);
        }
        else
        {
            int row = getDirectionRow();
            int col = currentFrame;

            texture->renderFrame(rect.to_sdl(), row, col);
        }
    }
}

bool Butterfly::update(float delta)
{

    // Si ha recibido un impacto, cae hacia abajo
    if (hitByArrow)
    {
        rect.move(Vector2D<>(speed * delta));

        // Eliminar cuando salga por la parte inferior
        if (rect.getTopLeftCorner().getY() > game->WINDOW_HEIGHT)
            return false;

        return true;
    }

        animationTimer += delta;

        while (animationTimer >= BUTTERFLY_FRAME_DURATION)
        {
            animationTimer -= BUTTERFLY_FRAME_DURATION;

            currentFrame += frameDirection;

            // Llegamos al último fotograma
            if (currentFrame >= BUTTERFLY_ANIMATION_COLUMNS - 1)
            {
                currentFrame = BUTTERFLY_ANIMATION_COLUMNS - 1;
                frameDirection = -1;
            }
            // Llegamos al primer fotograma
            else if (currentFrame <= 0)
            {
                currentFrame = 0;
                frameDirection = 1;
            }
        }

        if (rect.getTopLeftCorner().getX() < 0)
        {
            randomSpeedX =
                std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

            speed.setX(randomSpeedX);
        }

        if (rect.getTopLeftCorner().getX() > game->WINDOW_WIDTH - BUTTERFLY_WIDTH)
        {
            randomSpeedX =
                std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

            speed.setX(-randomSpeedX);
        }

        if (rect.getTopLeftCorner().getY() < 0)
        {
            randomSpeedY =
                std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

            speed.setY(randomSpeedY);
        }

        if (rect.getTopLeftCorner().getY() > game->WINDOW_HEIGHT - BUTTERFLY_HEIGHT)
        {
            randomSpeedY =
                std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);

            speed.setY(-randomSpeedY);
        }

        // Movimiento
        rect.move(Vector2D<>(speed * delta));

    return true;
   
}

bool Butterfly::hit(const Rectangle& other) {

    if (!hitByArrow && rect.hasIntersection(other)) {
        game->addScore(-10);
        hitByArrow = true;
        speed.setX(0);
        speed.setY(FALL_SPEED);
		game->onButterflyKilled(); 
        return true;
    }

    return false;
}

int Butterfly::getDirectionRow() const
{
    float vx = speed.getX();
    float vy = speed.getY();

    if (std::abs(vx) >= std::abs(vy))
    {
        return (vx >= 0) ? 0 : 1;
    }

    return (vy < 0) ? 2 : 3;
}

void Butterfly::save(std::ostream& os) const
{
    MobileObject::save(os);

    os << randomSpeedY << ' '
        << randomSpeedX << ' '
        << hitByArrow << ' ';
}

void Butterfly::load(std::istream& is, int line)
{
    MobileObject::load(is, line);

    if (!(is >> randomSpeedY >> randomSpeedX >> hitByArrow))
		throw FileFormatError("saved.txt", line, "Error al leer Butterfly");

    currentFrame = 0;
    animationTimer = 0.0f;
    frameDirection = 1;
}
