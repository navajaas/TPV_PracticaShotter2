#include "SceneObject.h"
#include "Texture.h"

SceneObject::SceneObject(Game* g, Rectangle r, Texture* tex)
	: GameObject(g),rect(r),texture(tex),pos({0,0})
{
}

SceneObject::~SceneObject()
{

}

