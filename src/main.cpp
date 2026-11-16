//
// Tecnología de la Programación de Videojuegos 1
// Facultad de Informática UCM
//
// Plantilla de proyecto con SDL
//

#include "game.h"
#include "GameError.h"

int main(int argc, char* argv[])
{
    try {
        Game game;
        game.run();
    }
    catch (const GameError& e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error", e.what(), nullptr);
        return 1;
    }
    catch (const std::exception& e) {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "Error inesperado", e.what(), nullptr);
        return 1;
    }

    return 0;
}
