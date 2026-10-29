#include "Bow.h"
#include "MobileObject.h"

constexpr float BOW_SPEED = 200.0f; // Velocidad de movimiento del arco
constexpr float SHOOT_CD = 0.2f; // Tiempo de espera entre disparos

Bow::Bow(Game* game, Rectangle rect, Texture* tex)
	: SceneObject(game,rect,tex), arrowArmad(false), timeSinceLastShot(0.0f)
{
}

void Bow::render() const
{
	if (texture)
	{
		texture->renderFrame(rect.to_sdl(), 0, arrowArmad ? 0 : 1);
	}
}

void Bow::handleEvents(const SDL_Event& event) {
	if (event.type == SDL_EVENT_KEY_DOWN) {
		switch (event.key.key) {
		case SDLK_UP:
			velocity.setY(-1.0f); // Mover hacia arriba
			break;
		case SDLK_DOWN:
			velocity.setY(1.0f); // Mover hacia abajo
			break;
		case SDLK_SPACE:
			if (timeSinceLastShot <= 0.0f) {
				if (!arrowArmad) {
					arrowArmad = true; // Preparar la flecha para disparar
				}
				else {
					shootArrow(); // Disparar la flecha
				}
				timeSinceLastShot = SHOOT_CD;
			}
			
			break;
		}
	}
	else if (event.type == SDL_EVENT_KEY_UP) {
		switch (event.key.key) {
		case SDLK_UP:
		case SDLK_DOWN:
			velocity.setY(0.0f); // Detener el movimiento vertical
			break;
		}
	}
	
}

bool Bow::update(float delta) {
	if (timeSinceLastShot > 0.0f) {
		timeSinceLastShot -= delta;
	}
	if (velocity.getY() != 0.0f) {
		float nextY = rect.getTopLeftCorner().getY() + velocity.getY() * BOW_SPEED * delta;

		float minY = 0.0f;
		float maxY = game->WINDOW_HEIGHT - rect.getHeight();

		if (nextY >= minY && nextY <= maxY) {
			rect.move(Vector2D(0.0f, velocity.getY() * BOW_SPEED * delta));
		}
	}
	return true;
}

void Bow::shootArrow() {
	if (arrowArmad) {
		Point2D<> arrowPosition(rect.getTopLeftCorner().getX() + rect.getWidth(), rect.getTopLeftCorner().getY() + rect.getHeight() / 2-10);
		arrow = new Arrow(game, Rectangle(arrowPosition, 60, 15), game->getTexture(Game::ARROW)); 
		arrowArmad = false; // Resetear el estado del arco
		game->updateArrows();
		game->addObject(arrow); // Agregar la flecha al juego
	}
}
