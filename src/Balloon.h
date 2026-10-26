#ifndef BALLOON_H
#define BALLOON_H

#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"

class Game;

class Balloon
{
public:
	Balloon(Rectangle rect, Texture* textura, float speed, int color);
	
	void render() const;
	bool update(float delta);
	bool hit(const Rectangle& rect);
private:

	Rectangle rect;
	Texture* textura=nullptr;

	float speed; // Velocidad de movimiento del globo
	int color;
	bool exploding = false;

	int explosionFrame = 0;
	float animationTimer = 0.0f;
};
#endif // BALLOON_H
