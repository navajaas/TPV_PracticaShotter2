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
	std::string getType() const override { return "ARROW"; }
private:
	Rectangle rectColision;
	int balloonHitCount = 0; // Contador de globos alcanzados por la flecha
};
#endif // ARROW_H

