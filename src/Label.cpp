#include "Label.h"
#include "Texture.h"

Label::Label(Texture* tex, Point2D<float> pos)
	: texture(tex),position(pos)
{
}

void Label::render() const {
	texture->render();
}
