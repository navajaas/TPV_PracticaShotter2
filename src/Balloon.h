#ifndef BALLOON_H
#define BALLOON_H

#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "MobileObject.h"

class Game;

class Balloon : public MobileObject
{
public:
	Balloon(Game*,Rectangle, Texture*, Vector2D<float>, int);
	
	void render() const override;
	bool update(float delta) override;
	bool hit(const Rectangle& rect) override;

private:
	int color;
	bool exploding = false;

	int explosionFrame = 0;
	float animationTimer = 0.0f;
};
#endif // BALLOON_H
