#include "Butterfly.h"

constexpr float BUTTERFLY_SIZE = 64.0f;

Butterfly::Butterfly(Game* game,Rectangle rect, Texture* tex, Vector2D<float> dirSpeed):
	MobileObject(game, rect, tex, dirSpeed)
{}

void Butterfly::render() const {
	if (texture) {
		texture->renderFrame(rect.to_sdl(), 0, 0);

	}
}
bool Butterfly::update(float delta) {
	//si se va a salir de la pantalla cambia su direccion de manera aleatoria
	if (rect.getTopLeftCorner().getX() < 0) {
		 randomSpeedX = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);
		 speed.setX(randomSpeedX);
	}
	if (rect.getTopLeftCorner().getX() > WINDOW_WIDTH-BUTTERFLY_SIZE) {
		 randomSpeedX = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);
		speed.setX(-randomSpeedX);
	}
	if (rect.getTopLeftCorner().getY() < 0) {
		 randomSpeedY = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);
		speed.setY(randomSpeedY);
	}
	if (rect.getTopLeftCorner().getY() > WINDOW_HEIGHT-BUTTERFLY_SIZE ) {
		 randomSpeedY = std::uniform_real_distribution<float>(50.0f, 150.0f)(randomGenerator);
		speed.setY(-randomSpeedY);
	}

	rect.move(Vector2D<>(speed * delta));
	//rectColision = Rectangle(rect.getTopLeftCorner(), rect.getWidth() - 20, rect.getHeight());
	//game->checkCollision(rectColision);

	return true;
}
bool Butterfly::hit(const Rectangle& rect) {
	return false;
}

