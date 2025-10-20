#include <SDL3/SDL.h>
#include <iostream>
#include "Vector2D.h"
#include "Body.h"
#include "World.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

int main(int argc, char* argv[]) {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cout << "Failed to initialize SDL3: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("Physics Engine", SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!window) {
        // ... manejo de error ...
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, nullptr); 
    if (!renderer) {
        std::cout << "Failed to create renderer: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return -1;
    }

    // --- CONFIGURACIÓN DE LA SIMULACIÓN ---
    World world;
    const float dt = 1.0f / 60.0f; // Delta time: simulamos a 60 fotogramas por segundo

    // Creamos un cuerpo que caerá
    Body fallingBody(Vector2D(SCREEN_WIDTH / 2.0f, 5.0f), 1.0f); // Posición inicial y masa
    world.AddBody(fallingBody);

    const Vector2D gravity(0.0f, 1.0f); // Gravedad (multiplicamos para que sea más visible)
    // --- FIN DE LA CONFIGURACIÓN ---

    bool isRunning = true;
    SDL_Event event;

    while (isRunning) {
        // --- 1. MANEJO DE EVENTOS ---
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT) {
                isRunning = false;
            }
        }

        // --- 2. ACTUALIZACIÓN (UPDATE) ---
        // Aplicamos la fuerza de gravedad a todos los cuerpos
        for (auto& body : world.GetBodies()) { // Necesitaremos un GetBodies() no-const para esto
             body.AddForce(gravity);
        }
        world.Update(dt);

        // --- 3. RENDERIZADO (DRAW) ---
        SDL_SetRenderDrawColor(renderer, 20, 20, 40, 255); // Fondo azul
        SDL_RenderClear(renderer);

        // Imprimimos la posición del cuerpo que cae, solo como debug en consola
        std::cout << "Posición del cuerpo: " << world.GetBodies()[0].position << std::endl;

        // Dibujamos todos los cuerpos del mundo
        SDL_SetRenderDrawColor(renderer, 200, 200, 50, 255); // Color amarillo para el objeto
        for (const auto& body : world.GetBodies()) {
            SDL_FRect bodyRect = {
                body.position.x - 12.5f, // Centramos el rectángulo en la posición
                body.position.y - 12.5f,
                25.0f, // ¡MÁS GRANDE! Ancho del rectángulo
                25.0f  // ¡MÁS GRANDE! Alto del rectángulo
            };
            SDL_RenderFillRect(renderer, &bodyRect);
        }

        SDL_RenderPresent(renderer);
    }

    // Limpieza
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}