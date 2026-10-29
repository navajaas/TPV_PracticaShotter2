#pragma once
#include "vector2D.h"
#include "rectangle.h"
#include "game.h"
#include "texture.h"
#include "MobileObject.h"

class Butterfly: public MobileObject
{
public:
	Butterfly(Game* game, Rectangle rect, Texture* texture, Vector2D<float> dirSpeed);

	void render() const override;
	bool update(float delta) override;

	//void handleEvents(const SDL_Event& event) override;
	bool hit(const Rectangle& rect) override;
private:
	static constexpr int WINDOW_WIDTH = 800;
	static constexpr int WINDOW_HEIGHT = 600;
	bool dead = false;
	std::mt19937_64 randomGenerator{ std::random_device{}() };
	float randomSpeedY = 0;
	float randomSpeedX = 0;
};

