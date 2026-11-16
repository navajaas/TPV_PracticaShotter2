#ifndef BALLOON_H
#define BALLOON_H

#include "vector2D.h"
#include "rectangle.h"
#include "texture.h"
#include "MobileObject.h"

class PlayState;

class Balloon : public MobileObject
{
public:
	Balloon(PlayState*,Rectangle, Texture*, Vector2D<float>, int);
	
	void render() const override;
	bool update(float delta) override;

	bool hit(const Rectangle& rect) override;

	void setPoints(int p) { points = p; }

	void save(std::ostream& os) const override;
	void load(std::istream& is, int line) override;

	std::string getType() const override { return "BALLOON"; }

private:
	int color;
	bool exploding = false;

	int explosionFrame = 0;
	float animationTimer = 0.0f;

	int points;
};
#endif // BALLOON_H
