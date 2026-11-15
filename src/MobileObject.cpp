#include "MobileObject.h"
#include <stdexcept>
#include "GameError.h"

MobileObject::MobileObject(Game* g, Rectangle r, Texture* tex, Vector2D<float> s)
	: SceneObject(g,r,tex), speed(s)
{
}

bool MobileObject::update(float delta)
{
	rect.move(Vector2D<>(speed*delta));
	return true;
}


void MobileObject::save(std::ostream& os) const
{
    SceneObject::save(os);

    os << speed.getX() << ' '
        << speed.getY() << ' ';

    if (!os)
		throw GameError("Error al guardar MobileObject");
}

void MobileObject::load(std::istream& is)
{
    SceneObject::load(is);

    float speedX, speedY;

    if (!(is >> speedX >> speedY))
		throw FileFormatError("saved.txt", 0, "Error al leer MobileObject");

    speed = Vector2D<float>(speedX, speedY);
}
