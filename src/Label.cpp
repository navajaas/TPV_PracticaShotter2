#include "Label.h"
#include "Texture.h"

Label::Label(GameState* state, Texture* tex, Point2D<float> pos)
	: GameObject(state), texture(tex),position(pos)
{
}

void Label::render() const {
	if (texture) {
		texture->render();
	}
}
