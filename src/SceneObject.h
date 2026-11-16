#ifndef SCENEOBJECT_H
#define SCENEOBJECT_H

#include "GameObject.h"
#include "rectangle.h"
#include "vector2D.h"
#include <iostream>
#include <string>

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

	virtual void save(std::ostream& os) const;
	virtual void load(std::istream& is,int line);
	virtual std::string getType() const = 0;
};

#endif //SCENEOBJECT_H
