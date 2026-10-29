#ifndef BOW_H
#define BOW_H

#include "vector2D.h"
#include "rectangle.h"
#include <SDL3/SDL.h>
#include "texture.h"
#include "Arrow.h"
#include "SceneObject.h"

class Game;
class Arrow;

class Bow : public SceneObject
{
public:
	Bow(Game* game, Rectangle rect, Texture* textura);
	
	void render() const override;
	bool update(float delta) override;

	void handleEvents(const SDL_Event& event) override;
	void shootArrow();
	bool hit(const Rectangle&) override { return false; }

private:
	Vector2D<> velocity;

	bool arrowArmad = false;
	float timeSinceLastShot = 0.0f; // Tiempo transcurrido desde el último disparo
	Arrow* arrow = nullptr; // Puntero a la flecha asociada al arco

};
#endif // BOW_H
