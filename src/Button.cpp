#include "Button.h"
#include "texture.h"
#include "GameState.h"
#include <SDL3/SDL.h>

Button::Button(GameState* state, Texture* tex, Point2D<float> pos) :
	Label(state, tex, pos)
{
	state->addEventHandler(this);
}

void Button::handleEvents(const SDL_Event& event) {

	if (event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
		float mouseX = event.button.x;
		float mouseY = event.button.y;
		SDL_FRect rect{ position.getX(),position.getY(), texture->getFrameWidth(), texture->getFrameHeight()};

		if (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
			mouseY >= rect.y && mouseY <= rect.y + rect.h) {
			if (callback) {
				callback();
			}
		}
	}
}

void Button::render() const {

	float mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);

	SDL_FRect rect{ position.getX(),position.getY(), texture->getFrameWidth(), texture->getFrameHeight() };

	SDL_Color color = (mouseX >= rect.x && mouseX <= rect.x + rect.w &&
		mouseY >= rect.y && mouseY <= rect.y + rect.h)
		? SDL_Color{ 0, 255, 75, 255 }
	: SDL_Color{ 255, 255, 255, 255 };

	texture->render(rect, color);

}

void Button::connect(Button::Callback&& cb)
{
	callback = cb;
}




