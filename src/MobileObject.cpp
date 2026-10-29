#include "MobileObject.h"

MobileObject::MobileObject(Game* g, Rectangle r, Texture* tex, Vector2D<float> s)
	: SceneObject(g,r,tex), speed(s)
{
}

bool MobileObject::update(float delta)
{
	rect.move(Vector2D<>(speed*delta));
	return true;
}
