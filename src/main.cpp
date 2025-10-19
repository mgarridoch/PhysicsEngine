#include <SDL3/SDL.h>
#include <iostream>

int main(int argc, char* argv[]) {
    // CAMBIO 1: SDL_Init ahora devuelve un bool. Comprobamos el fallo con !
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        // Pequeña mejora: Usar SDL_GetError() para obtener un mensaje más específico.
        std::cout << "Failed to initialize SDL3: " << SDL_GetError() << std::endl;
        return -1;
    }

    // CAMBIO 2: SDL_CreateWindow ya no acepta posición (x, y). Ahora son 4 argumentos.
    // La ventana aparecerá centrada por defecto.
    SDL_Window* window = SDL_CreateWindow("Physics Engine", 800, 600, 0);
    if (!window) {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    // CAMBIO 3: SDL_RENDERER_ACCELERATED ya no existe. Pasamos 0 para los flags.
    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr); // También es mejor práctica usar nullptr en vez de -1 para el driver.
    if (!renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);

    SDL_Delay(5000);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}