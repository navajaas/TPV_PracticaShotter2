#include "SceneObject.h"
#include "Texture.h"
#include <stdexcept>

SceneObject::SceneObject(Game* g, Rectangle r, Texture* tex)
    : GameObject(g), rect(r), texture(tex), pos({ 0, 0 })
{
}

SceneObject::~SceneObject()
{
}

void SceneObject::save(std::ostream& os) const
{
    Point2D<> corner = rect.getTopLeftCorner();

    os << corner.getX() << ' '
        << corner.getY() << ' '
        << rect.getWidth() << ' '
        << rect.getHeight() << ' '
        << pos.getX() << ' '
        << pos.getY() << ' ';

    if (!os)
        throw std::runtime_error("Error al guardar SceneObject");
}

void SceneObject::load(std::istream& is)
{
    float x, y, width, height, posX, posY;

    if (!(is >> x >> y >> width >> height >> posX >> posY))
        throw std::runtime_error("Error al leer SceneObject");

    if (width <= 0 || height <= 0)
        throw std::runtime_error("Dimensiones incorrectas en SceneObject");

    rect = Rectangle(Point2D<>(x, y), width, height);
    pos = Point2D<>(posX, posY);
}
