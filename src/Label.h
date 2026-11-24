#ifndef LABEL_H
#define LABEL_H

#include "vector2D.h"

class Texture;

class Label
{
protected:
	Texture* texture;
	Point2D<float> position;
public:
	Label(Texture* tex, Point2D<float> pos);
	virtual void render() const;
};

#endif LABEL_H

