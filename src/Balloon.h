#ifndef BALLOON_H
#define BALLOON_H

#include "Vector2D.h"
#include "rectangle.h"
#include "texture.h"
class Game;
class Balloon
{
public:
	Balloon(Game* game, Rectangle rect, Texture* textura, float speed, int color);
	
	void render() const;
	bool update(float delta);
	//bool hit(const Rectangle& rect);
private:
	Game* game=nullptr;
	Rectangle rect;
	Texture* textura=nullptr;

	float speed; // Velocidad de movimiento del globo
	int color;
};
#endif // BALLOON_H
