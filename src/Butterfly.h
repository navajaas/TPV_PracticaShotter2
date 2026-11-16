#ifndef BUTTERFLY_H
#define BUTTERFLY_H

#include "vector2D.h"
#include "rectangle.h"
#include <random>
#include "GameState.h"
#include "texture.h"
#include "MobileObject.h"

class PlayState;

class Butterfly: public MobileObject
{
public:
	Butterfly(PlayState*, Rectangle, Texture*, Vector2D<float>);

	void render() const override;
	bool update(float) override;

	bool hit(const Rectangle&) override;

	void save(std::ostream& os) const override;
	void load(std::istream& is, int line) override;
	std::string getType() const override { return "BUTTERFLY"; }

private:
	std::mt19937_64 randomGenerator{ std::random_device{}() };
	float randomSpeedY = 0;
	float randomSpeedX = 0;

	int currentFrame = 0;
	float animationTimer = 0.0f;
	int frameDirection = 1;

	bool hitByArrow = false;

	int getDirectionRow() const;
};

#endif // BUTTERFLY_H
