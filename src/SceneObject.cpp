#include "SceneObject.h"
#include "texture.h"
#include <stdexcept>
#include "GameError.h"
#include "GameState.h"
#include "PlayState.h"

SceneObject::SceneObject(PlayState* g, Rectangle r, Texture* tex)
    : GameObject(g), game(g), rect(r), texture(tex), pos({ 0, 0 })
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
        throw GameError("Error al guardar SceneObject");
}

void SceneObject::load(std::istream& is,int line)
{
    float x, y, width, height, posX, posY;

    if (!(is >> x >> y >> width >> height >> posX >> posY))
		throw FileFormatError("saved.txt", line, "Error al leer SceneObject");

    if (width <= 0 || height <= 0)
        throw FileFormatError("saved.txt",0,"Dimensiones incorrectas.");

    rect = Rectangle(Point2D<>(x, y), width, height);
    pos = Point2D<>(posX, posY);
}
