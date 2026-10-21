//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"

int main(int argc, char* argv[])
{
    try {
        Game game;
        game.run();
    }
    catch (const std::string& error) {
        std::cerr << error << '\n';
        return 1;
    }
	return 0;
}
