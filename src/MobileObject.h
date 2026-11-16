#ifndef MOBILEOBJECT_H
#define MOBILEOBJECT_H

#include "SceneObject.h"

class MobileObject : public SceneObject
{
protected:
	Vector2D<float> speed;
public:
	MobileObject(PlayState* g, Rectangle r, Texture* tex,Vector2D<float> s);
	~MobileObject() {};

	bool update(float delta) override;
	void save(std::ostream& os) const override;
	void load(std::istream& is, int line) override;
};

#endif //MOBILEOBJECT_H
