#ifndef BUTTON_H
#define BUTTON_H

#include "Label.h"
#include "EventHandler.h"
#include <functional>

class Texture;

class Button : public Label, public EventHandler
{
public:
	using Callback = std::function<void()>;

private:
	Callback callback;

public:
	Button(GameState* state, Texture* tex, Point2D<float> position);
	bool update(float delta) override { return true; }
	void render() const override;
	void handleEvents(const SDL_Event& event) override;
	void connect(Button::Callback&& cb);
};

#endif //BUTTON_H

