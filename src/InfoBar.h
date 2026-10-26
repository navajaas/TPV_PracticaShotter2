#include "Vector2D.h"
#include "rectangle.h"
#include "game.h"
#include "texture.h"
class Game;
class InfoBar {
public: 
	InfoBar(Game* game, Rectangle rect, int arrowsLeft, Texture* textura) :
		game(game),rect(rect),arrowsLeft(arrowsLeft), textura (textura)
	{

	}
	void render() const{
		std::cout << arrowsLeft;
		if (textura) {
			// Ancho del rectángulo base de cada flecha
			int arrowWidth = rect.getWidth();
			// Espaciado opcional entre flechas (en píxeles)
			int spacing = 10;

			for (int i = 0; i < arrowsLeft; i++) {
				// Calculamos la nueva posición X desplazada para cada flecha
				float newX = rect.getTopLeftCorner().getX() + i * (arrowWidth + spacing);
				float newY = rect.getTopLeftCorner().getY();

				// Creamos un rectángulo posicionado para la flecha actual
				Rectangle currentRect(Point2D<>(newX, newY), arrowWidth, rect.getHeight());
				// Renderizamos la flecha en su nueva posición
				textura->renderFrame(currentRect.to_sdl(), 0, 0);
			}
		}
		
	}
	void updateArrows(int arrows) {
		arrowsLeft = arrows;
	}
private:
	Game* game = nullptr;
	Texture* textura = nullptr;
	Rectangle rect;
	int arrowsLeft;
};


