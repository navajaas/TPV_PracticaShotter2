#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "GameObject.h"
#include "rectangle.h"
#include "vector2D.h"

class Texture;

class SceneObject : public GameObject
{
protected:
	Rectangle rect;
	Texture* texture;
	Point2D<float> pos;

public:
	SceneObject(Game* g,Rectangle r, Texture* tex);
	~SceneObject();

	virtual bool hit(const Rectangle&) = 0;
};

#endif //SCENEOBJECT_H
