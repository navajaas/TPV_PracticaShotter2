#ifndef LABEL_H
#define LABEL_H

#include "vector2D.h"
#include "GameObject.h"

class Texture;
class GameState;

class Label : public GameObject
{
protected:
	Texture* texture;
	Point2D<float> position;
public:
	Label(GameState* state, Texture* tex, Point2D<float> pos);
	virtual void render() const;
};

#endif LABEL_H

