#include "Bow.h"
Bow::Bow(Game* game, Rectangle rect, Texture* tex)
	: game(game), rect(rect), textura(tex), arrowArmad(false), timeSinceLastShot(0.0f)
{
}
void Bow::render() const
{
	if (textura)
	{
		textura->renderFrame(rect.to_sdl(), 0, arrowArmad ? 0 : 1);
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
				timeSinceLastShot=arrowsCoolDown;
			}
			
			break;
		}
	}
	
}

bool Bow::update(float delta) {
	if (timeSinceLastShot > 0.0f) {
		timeSinceLastShot -= delta;
	}
	if (velocity.getY() != 0.0f) {
		std::cout << "Moviendo arco Y: " << velocity.getY() << " rectY: " << rect.getTopLeftCorner().getY() << " Delta:	 " << delta << std::endl;
		rect.move(Vector2D(0.0f, velocity.getY() * speed * delta));
	}
	return true;
}

void Bow::shootArrow() {
	if (arrowArmad) {
		Point2D<> arrowPosition(rect.getTopLeftCorner().getX() + rect.getWidth(), rect.getTopLeftCorner().getY() + rect.getHeight() / 2-10);
		arrow = new Arrow(game, Rectangle(arrowPosition, 60, 15), game->getTexture(Game::ARROW)); 
		arrowArmad = false; // Resetear el estado del arco
		game->addArrow(arrow); // Agregar la flecha al juego
	}
}
