#ifndef ARROW_H
#define ARROW_H

#include "vector2D.h"
#include "rectangle.h"
#include "game.h"
#include "texture.h"
#include "MobileObject.h"

class Game;

class Arrow : public MobileObject
{
public:
	Arrow(Game* game, Rectangle rect, Texture* textura);
	
	void render() const override;
	bool update(float delta) override;
	bool hit(const Rectangle&) override { return false; }

private:
	Rectangle rectColision;
};
#endif // ARROW_H

