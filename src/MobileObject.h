#ifndef MOBILEOBJECT_H
#define MOBILEOBJECT_H

#include "SceneObject.h"

class MobileObject : public SceneObject
{
protected:
	Vector2D<float> speed;
public:
	MobileObject(Game* g, Rectangle r, Texture* tex,Vector2D<float> s);
	~MobileObject() {};

	bool update(float delta) override;
};

#endif //MOBILEOBJECT_H
