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
		if (textura) {
			// Ancho del rectángulo base de cada flecha (como esta girada se usa la altura)
			int arrowWidth = rect.getHeight();
			// Espaciado opcional entre flechas (en píxeles)
			int spacing = 10;

			for (int i = 0; i < arrowsLeft; i++) {
				// Calculamos la nueva posición X desplazada para cada flecha
				float newX = rect.getTopLeftCorner().getX() + i * (arrowWidth+ spacing);
				float newY = rect.getTopLeftCorner().getY();

				// Creamos un rectángulo posicionado para la flecha actual
				Rectangle currentRect(Point2D<>(newX, newY), rect.getWidth(), rect.getHeight());
				// Renderizamos la flecha en su nueva posición
				textura->renderFrame(currentRect.to_sdl(), 0, 0,-90.0);
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


